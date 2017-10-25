#include "TcpServer.h"
#include <QDebug>
#include "AppHelper.h"

TcpServer *gTcpServer;
TcpServer::TcpServer(QThread *parent) : QThread(parent)
{
    ServerIP = new QHostAddress();
    TcpSocket = new QTcpSocket(this);
    isCanSendNextFrame_Flag = CurrentCMD_Resulted;

    initParam();

    //Tcp
    connect(TcpSocket,SIGNAL(connected()),this,SLOT(slt_Connected()), Qt::DirectConnection);
    connect(TcpSocket,SIGNAL(disconnected()),this,SLOT(slt_Disconnected()), Qt::DirectConnection);
    connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(slt_dataReceived()), Qt::DirectConnection);

    //应答&结果 计时器
    timerRespond = new QTimer; //指令应答计时器
    connect(this, SIGNAL(sig_timerRespond_start(int)), timerRespond,SLOT(start(int)));
    connect(this, SIGNAL(sig_timerRespond_stop()), timerRespond,SLOT(stop()));

    timerResult = new QTimer;  //指令结果计时器
    connect(this, SIGNAL(sig_timerResult_start(int)), timerResult,SLOT(start(int)));
    connect(this, SIGNAL(sig_timerResult_stop()), timerResult,SLOT(stop()));
}

//初始化参数
void TcpServer::initParam()
{
    isTcpSocketConnected = false;
    //OpenTCPServer(gParameter->getTCPServerIP(), gParameter->getTCPServerPort());

    //清空AD数据
    clearADMap();

    isRespnded = true;      //是否接收到应答帧
    isResulted = true;      //是否接收到结果帧

    m_ReadCardData = "";
    isDataReceived = true;

    taskList = new Task[TASKCOUNT];

    m_ReceiveData.clear();
}
//清空AD数据
void TcpServer::clearADMap()
{
    //初始化AD数据的映射表
    for(int i = 0; i <= REACTION_CUP_COUNT; i ++)
        ADMap.insert(i, 0); //保存AD数据的映射表（杯号，AD值）
}

//槽：连接&断开TCP服务器
void TcpServer::slt_TCP_Connect(QString ip, int port)
{
    if ( !isTcpConnected() )
    {
        if(ServerIP->setAddress(ip))
        {
            TcpSocket->connectToHost(*ServerIP, port);
            qDebug() << ip << ":" << port << "连接TCP服务器...";
        }
    }
}
void TcpServer::slt_TCP_disConnect()
{
    if( isTcpConnected() )
    {
        isTcpSocketConnected = false;
        TcpSocket->disconnectFromHost();
        qDebug() << "断开TCP服务器!";
    }
}

//开始监控网口返回数据
int TcpServer::StartMonitor()
{
    if(this->isRunning())
    {
        qDebug() << "****TcpServer is running...";
        return 0; //线程工作中(未执行)
    }
    this->start(QThread::HighestPriority);
    return 1;
}
//TCP服务器状态
bool TcpServer::isTcpConnected() {return isTcpSocketConnected;}

//是否能发送下一帧通讯指令
int TcpServer::get_isCanSendNextFrame_Flag() {return isCanSendNextFrame_Flag;}
void TcpServer::set_isCanSendNextFrame_Flag(int flag) {isCanSendNextFrame_Flag=flag;}
//是否接收到应答帧
bool TcpServer::get_isRespnd() {return isRespnded;}
void TcpServer::set_isRespnd(bool flag) {isRespnded = flag;}
//是否接收到结果帧
bool TcpServer::get_isResult() {return isResulted;}
void TcpServer::set_isResult(bool flag) {isResulted = flag;}

void TcpServer::run()
{
    while( 1 )
    {
        if( !m_ReceiveData.isEmpty() )
        {
            QString  data = m_ReceiveData.first(); //队首
            m_ReceiveData.pop_front();            //出队
            SortData(data);
        }
        else
        {
            msleep(10);
        }
    }
}

//16进制字符串转字节数组
QByteArray TcpServer::HexStrToByteArray(QString str)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len / 2);
    char lstr, hstr;
    for (int i = 0; i < len; )
    {
        hstr = str[i].toLatin1();
        if (hstr == ' ') {
            i++;
            continue;
        }
        i++;
        if (i >= len) {
            break;
        }
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if ((hexdata == 16) || (lowhexdata == 16)) {
            break;
        } else {
            hexdata = hexdata * 16 + lowhexdata;
        }
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

char TcpServer::ConvertHexChar(char ch)
{
    if ( (ch >= '0') && (ch <= '9') )
        return ch - 0x30;
    else if ( (ch >= 'A') && (ch <= 'F') )
        return ch - 'A' + 10;
    else if ( (ch >= 'a') && (ch <= 'f') )
        return ch - 'a' + 10;
    else
        return (-1);
}
//字节数组转16进制字符串(中间以空格间隔)
QString TcpServer::ByteArrayToHexStr2(QByteArray data)
{
    QString temp = "";
    QString hex = data.toHex();
    for (int i = 0; i < hex.length(); i = i + 2)
    {
        temp += hex.mid(i, 2) + " ";
    }
    return temp.trimmed().toUpper();
}
//字节数组转16进制字符串
QString TcpServer::ByteArrayToHexStr(QByteArray data)
{
    QString temp = "";
    QString hex = data.toHex();
    for (int i = 0; i < hex.length(); i = i + 2)
    {
        temp += hex.mid(i, 2);
    }
    return temp.trimmed().toUpper();
}

//发送数据
void TcpServer::slt_Send(QString data)
{
    Send(data);
}
int TcpServer::Send(QString data)
{
    //qDebug () << data << isTcpSocketConnected;
    QMutexLocker locker(&mutex); //
    if( !isTcpSocketConnected ) return 0;
    if ( data == "" ) return 0;        //发送数据为空
    if ( isCanSendNextFrame_Flag != CurrentCMD_Resulted )  return 0; //上一条指令已经返回结果

    if( !isRespnded || !isResulted )
    {
        qDebug() << "指令的应答未收到 & 指令的结果未收到 ！" << "isRespnded=" << isRespnded << ", isResulted=" << isResulted;
        return 0;
    }

    int size = TcpSocket->write(HexStrToByteArray(data), data.length()); //以十六进制格式发送，返回发送字节数
    emit sig_timerRespond_start(TIME_RESPONSE);//指令应答计时器开始计时
    //timerRespond->start( TIME_RESPONSE ); //指令应答计时器开始计时
    //timerResult->start( gParameter->getCycleTime() ); //指令结果计时器开始计时

    isCanSendNextFrame_Flag = CurrentCMD_WiatResult;//等待当前指令返回结果
    isRespnded = false;
    isResulted = false;

    QDateTime time = QDateTime::currentDateTime();                  //获取系统现在的时间
    QString date = time.toString("[yyyy-MM-dd hh:mm:ss.zzz]");      //设置显示格式
    qDebug() << "  Send:" << data << "   Date:"  << date;
    return size;
}

void TcpServer::slt_Send(char *data, int lenght)
{
    Send(data, lenght);
}
int TcpServer::Send(char *data, int lenght)
{   
    QMutexLocker locker(&mutex); //
    if( !isTcpSocketConnected ) return 0;
    if ( isCanSendNextFrame_Flag != CurrentCMD_Resulted )  return 0; //上一条指令已经返回结果

    if( !isRespnded || !isResulted )
    {
        qDebug() << "指令的应答未收到 & 指令的结果未收到 ！" << "isRespnded=" << isRespnded << ", isResulted=" << isResulted;
        return 0;
    }


    CheckSum(data, lenght); //计算校验和
    int size = TcpSocket->write(data, lenght); //以十六进制格式发送，返回发送字节数
    emit sig_timerRespond_start(TIME_RESPONSE);//指令应答计时器开始计时
    //timerRespond->start( TIME_RESPONSE ); //指令应答计时器开始计时
    //timerResult->start( gParameter->getCycleTime() ); //指令结果计时器开始计时

    isCanSendNextFrame_Flag = CurrentCMD_WiatResult;//等待当前指令返回结果
    isRespnded = false;
    isResulted = false;

    //将要发送到中位机的指令打印出来
    QDateTime time = QDateTime::currentDateTime();                  //获取系统现在的时间
    QString date = time.toString("[yyyy-MM-dd hh:mm:ss.zzz]");      //设置显示格式
    QByteArray byte(data, lenght);
    QString msg = ByteArrayToHexStr2(byte);
    qDebug() << "  Send:" << msg << "   Date:"  << date;

    return size;
}

//计算校验和(校验和是从帧头到校验位前一字节的累加和的低7位，高位固定为0)
int TcpServer::CheckSum(char *cmd, int lenght)
{
    int checkSum = 0; //校验和
    if( lenght<= 2 ) return checkSum;

    for(int i = 0; i < lenght-2; i ++)
        checkSum += cmd[i];

    cmd[lenght-2] = checkSum&0x7f; //取低7位
    return cmd[lenght-2];
}


//槽：连接TCP服务器成功
void TcpServer::slt_Connected()
{
    isTcpSocketConnected = true;
    qDebug() << "连接TCP服务器成功!";
}
//槽：断开服务器的连接
void TcpServer::slt_Disconnected()
{
    isTcpSocketConnected = false;
    qDebug() << "断开服务器的连接成功!";
}
//槽：接收服务器发送的数据
void TcpServer::slt_dataReceived()
{
    QByteArray datagram;
    QString data = "", temp = "";
    int byte;
    int headPt = -1;
    int tailPt = -1;
    int Count;
    while(TcpSocket->bytesAvailable()>0)
    {
        datagram.resize(TcpSocket->bytesAvailable());
        TcpSocket->read(datagram.data(),datagram.size());
        data += ByteArrayToHexStr(datagram);

       //qDebug() << "         " << data;

        Count = data.count();
       for(int i = 0; i < Count; i += 2)
       {
           byte =  data.mid(i, 2).toInt(0, 16);

           if( byte == FRAME_HEAD)
               headPt  = i;
           if( byte == FRAME_TAIL)
           {
                tailPt  = i;
                if( headPt != -1 && tailPt != -1 )
                {
                    if(headPt < tailPt)
                    {
                        m_ReceiveData.push_back(data.mid(headPt, tailPt-headPt+2));
                        if(Count - tailPt -1 != 0)
                            temp = data.right(Count - tailPt-2);
                        else
                            temp = "";
                    }
                }
           }
       }
       data = temp;
    }
}

//槽：指令应答计时器
void TcpServer::slt_timerRespond_timeout()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString date = time.toString("[yyyy-MM-dd hh:mm:ss.zzz]");      //设置显示格式

    if(timerRespond->isActive())
    {
        emit sig_timerRespond_stop();
        //timerRespond->stop();
    }
    if( !isRespnded )
    {
        if(timerResult->isActive())
        {
            emit sig_timerResult_stop();
            //timerResult->stop();
        }

        gScheduler->setWorkModeFlag(MV_State_Error);  //工作模式状态标志=故障【指令无应答/结果返回】
        gScheduler->slt_CycleTimer_Stop();   //槽：停止周期定时器计时
        emit sig_AlertMsg_show(1, "ERROR 指令应答超时..." );  //信号：显示错误信息

        qDebug() << "ERROR 指令应答计时器." << date << "指令应答超时。。。" << isRespnded;
    }
}
//槽：指令结果计时器
void TcpServer::slt_timerResult_timeout()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString date = time.toString("[yyyy-MM-dd hh:mm:ss.zzz] ");      //设置显示格式

    if(timerResult->isActive())
    {
        emit sig_timerResult_stop();
        //timerResult->stop();
    }
    if( !isResulted )
    {
        gScheduler->setWorkModeFlag(MV_State_Error);  //工作模式状态标志=故障【指令无应答/结果返回】
        gScheduler->slt_CycleTimer_Stop();   //槽：停止周期定时器计时
        emit sig_AlertMsg_show(1, "ERROR 指令结果超时..." );  //信号：显示错误信息
        qDebug() << "ERROR 指令结果计时器." << date << "指令结果超时。。。" << isResulted;
    }
}

//分类处理数据
void TcpServer::SortData(QString data)
{
    QDateTime time = QDateTime::currentDateTime();                  //获取系统现在的时间
    QString date = time.toString("[yyyy-MM-dd hh:mm:ss.zzz]");      //设置显示格式

    int count = data.count();  //返回帧的长度
    //qDebug() << " Receive:" << data << "count:" << count << "   Date:"  << date;
    if(count < 12) return;
    int frameHead = data.mid(0, 2).toInt(0, 16);  //帧头
    int frameMType = data.mid(2, 2).toInt(0, 16); //机器类型
    int frameType = data.mid(4, 2).toInt(0, 16);  //帧类型
    //int frameNo = data.mid(6, 2).toInt(0, 16);    //帧号
    int frameSid = data.mid(8, 2).toInt(0, 16);   //发送者id
    int frameRid = data.mid(10, 2).toInt(0, 16);  //接收者id
    int frameCmdID = data.mid(12, 2).toInt(0, 16);//命令id
    QVector<int> frameParamVec2;                   //参数列表
    QVector<QString> frameParamVec;                //参数列表
    for( int i = 0; i < (count-14-4); i = i+2) //注：(count-14-4)即帧长度 - 帧信息（即帧头到命令ID）的长度 - （检验和+帧尾）的长度
    {
        frameParamVec.append(data.mid(14+i, 2));
        if(frameType == FRAME_DATA)
            frameParamVec2.append(data.mid(14+i, 2).toInt(0, 16));
    }
    int frameCheckSum = data.mid(count-4, 2).toInt(0, 16); //检验和
    int frameTail = data.mid(count-2, 2).toInt(0, 16);     //帧尾

    //判断"机器类型"是否匹配
    if( frameMType != FRAME_MACHINETYPE )
    {
        if(gScheduler->getWorkModeFlag() != MV_STATE_ReadCard )
        {
            qDebug() << "机器类型不匹配, 错误类型=" << frameMType << "，正确类型=" << FRAME_MACHINETYPE;
        }
        else //读卡模式
        {
            //qDebug() << " Card Data:" << data  << "   Date:"  << date;
            SortOutCardData(data);
        }
        return;
    }

    //判断"接收者id"是否匹配
    if( frameRid != UNIT_PC )
    {
        qDebug() << "接收者id不匹配, 错误接收者id=" << frameRid << "，正确接收者id=" << UNIT_PC;
        return;
    }

    //验证帧头&帧尾
    if( frameHead == FRAME_HEAD && frameTail == FRAME_TAIL)
    {
        //验证校验和
        //if(1)
        if( frameCheckSum == CheckSum(data) )
        {
            //通讯帧类型
            switch( frameType )
            {
            case FRAME_RESPONSE: //命令应答帧
            {
                qDebug() << "收到-命令应答帧。。。" << " Receive:" << data << "   Date:"  << date;
                CmdRespondFrame(frameSid, frameCmdID, frameParamVec);
                break;
            }
            case FRAME_RESULT:   //命令结果帧
            {
                qDebug() << "收到-命令结果帧。。。" << " Receive:" << data << "   Date:"  << date;
                CmdResultFrame(frameSid, frameCmdID, frameParamVec);
                break;
            }
            case FRAME_DATA:     //数据帧
            {
                //qDebug() << "收到-数据帧。。。" << " Receive:" << data << "   Date:"  << date;
                CmdADDataFrame(frameSid, frameCmdID, frameParamVec2);
                break;
            }
            case FRAME_WARNING:  //警告信息帧
            {
                //qDebug() << "收到-警告信息帧。。。" << " Receive:" << data << "   Date:"  << date;
                CmdWarningFrame(frameSid, frameCmdID, frameParamVec);
                break;
            }
            default:
            {
                qDebug() << "收到-Default。。。" << " Receive:" << data << "   Date:"  << date;
            }
            }
        }
        else
        {
            //qDebug() << frameCheckSum << data;
            qDebug() << frameCheckSum << CheckSum(data) << data;
            qDebug() << "Error:" << " 验证校验和失败: CheckSum Error!!! "<< " function:" << __func__  << " ,Line:" << __LINE__;
        }
    }
    else
    {
        qDebug() << "Error:" << " 验证帧头&帧尾失败： 帧头="<<frameHead << ", 帧尾=" << frameTail
                 << " function:" << __func__  << ", Line:" << __LINE__;
    }
}
//计算校验和
int TcpServer::CheckSum(QString data)
{
    //校验和是从帧头到校验位前一字节的累加和的低7位，高位固定为0.
    long int checkSum = 0; //校验和
    for(int i = 0; i < data.count()-4; i=i+2)
    {
        checkSum += data.mid(i, 2).toInt(0, 16);
        //qDebug() << data.mid(i, 2).toInt(0, 16)  << checkSum;
    }
    //qDebug() << (checkSum&0x7f);
    return checkSum&0x7f; //取低7位
}

//命令应答帧（Sid:发送者ID，Cid：命令ID，ParamList：参数列表）
int TcpServer::CmdRespondFrame(int Sid, int Cid, QVector<QString> ParamList)
{
    isRespnded = true;
    //timerRespond->stop();
    emit sig_timerRespond_stop();
    //timerResult->start( gParameter->getCycleTime() ); //指令结果计时器开始计时
    emit sig_timerResult_start( gParameter->getCycleTime() );//指令结果计时器开始计时

    for(int i = 0; i < TASKCOUNT; i ++)
        taskList[i] = gScheduler->getTaskList(i);
    //qDebug() << "SendID:" << Sid << ", CMDid:" << Cid << ", Param:" << ParamList;
    return 1;
}
//命令结果帧
int TcpServer::CmdResultFrame(int Sid, int Cid, QVector<QString> ParamList)
{
    //参数列表（详细查阅通讯协议）
    //固定结果包含： 命令第一个参数、执行结果、结果细节、结果细节1、结果细节2、错误模块个数M（对下位机指令M= 0）
    //错误列表(结果正常则无错误表)： 模块ID、指令ID、指令的第一个参数、执行错误代号、错误细节、故障发生的位置(参见表4)、错误细节2
    //未定义： 保留
    emit sig_timerResult_stop();
    //timerResult->stop();
    isResulted = true;

    //判断结果是否有错(用返回的参数个数来判断)
    if(ParamList.count() > 7)
    {
        qDebug() << "SendID:" << Sid << ", CMDid:" << Cid << ", Param:" << ParamList;
        QString errNo = "";
        int id = ParamList.at(6).toInt(0, 16);  //模块ID
        errNo += QString::number( ParamList.at(9).toInt(0, 16), 16); //执行错误代号
        errNo += QString::number( ParamList.at(10).toInt(0, 16), 16); //错误细节
        errNo += QString::number( ParamList.at(11).toInt(0, 16), 16); //故障发生的位置(参见表4)
        errNo += QString::number( ParamList.at(12).toInt(0, 16), 16); //错误细节2


        //先查看错误级别（严重错误or普通错误，即是否要停机处理）
        QString ErrDescribe = gMV_ErrTable->getNormalErrDescribe(id, errNo);
        if( ErrDescribe == "" ) //在普通错误表中未能查到，即表示是严重错误或未定义的错误（停机）
        {
            isCanSendNextFrame_Flag = CurrentCMD_Error; //当前指令执行有错误返回
            ErrDescribe = gMV_ErrTable->getSeriousErrDescribe(id, errNo);
            if(ErrDescribe == "") ErrDescribe = "未定义错误";
            emit sig_AlertMsg_show(1, ErrDescribe+" 停机");

            qDebug() << "严重错误表：" << QString::number(id, 16) << ErrDescribe;
            return 1;
        }
        else //普通错误（不停机，继续工作）
        {
            qDebug() << "普通错误表：" << id << ErrDescribe;
            emit sig_AlertMsg_show(1, "警告:" + ErrDescribe);
        }
    }

    isCanSendNextFrame_Flag = CurrentCMD_Resulted ;  //是否能发送下一帧通讯指令
    return 1;
}
//数据帧
int TcpServer::CmdADDataFrame(int Sid, int Cid, QVector<int> ParamList)
{
    //AD数据只处理由“反应盘单元”返回的数据
    if( Sid != UNIT_REACTION_DISK) return 0;

    //qDebug() << "SendID:" << Sid << ", CMDid:" << Cid << ", Param:" << ParamList;

    switch (Cid) //命令ID
    {
    case CMDID_AD:
    {
        //中位机返回数据已经排序（2号杯开始）
        int cupS = 2; //= ParamList.at(0);  //采第一个杯的杯号
        clearADMap();

        //参数中包含了杯号+30个AD数据（每个AD数据有3位）
        if(ParamList.count() ==  REACTION_CUP_COUNT/2*3 +1)
        {
            int ad;
            int Hight2Bits, Mid7Bits, Low7Bits;  //数据高7位，中7位，低7位
            for( int i = 1; i < ParamList.count()-1; i ++)
            {
                Hight2Bits = ParamList.at(i ++)<<14;
                Mid7Bits = ParamList.at(i ++)<<7;
                Low7Bits = ParamList.at(i);

                if( ADMap.contains(cupS) )
                {
                    ad = Hight2Bits+Mid7Bits+Low7Bits;
                    //qDebug() << cupS << ad <<  "     " << Hight2Bits << Mid7Bits << Low7Bits;
                    ADMap.insert(cupS,ad);
                }
                else
                {
                    qDebug() << "Error: TcpServer::CmdADDataFrame 杯号位错误 key:" << cupS
                             << ", function:" << __func__  << ", Line:" << __LINE__;
                }

                cupS = cupS +2;
                if( cupS > REACTION_CUP_COUNT)
                    cupS = cupS - REACTION_CUP_COUNT;
            }

            //qDebug() << "";
            //qDebug() << "起始杯号:" << cupS;
            //保存AD数据到数据库中
            SaveADData();
        }
        else
        {
            qDebug() << "Error: TcpServer::CmdADDataFrame 可能是返回的AD个数有误 count:" << ParamList.count()-1
                     << " 起始杯号：" << ParamList.at(0)
                     << ", function:" << __func__  << ", Line:" << __LINE__;
        }
        break;
    }
    }

    return 1;
}
//警告信息帧
int TcpServer::CmdWarningFrame(int Sid, int Cid, QVector<QString> ParamList)
{
    return 1;
}

//保存AD数据到数据库中
void TcpServer::SaveADData()
{
    qDebug() << "SaveADData...";
    int ADValue;
    int Count;
    int cup;
    QStringList strList;
    strList.clear();

    //qDebug() << "ADMap:" << ADMap;

    for(int i = 0; i < TASKCOUNT; i ++)
    {
        if( taskList[i].Status == 1 )
        {
            ADValue = 0;
            int lightPos2 = taskList[i].LightPos2;
            int id = taskList[i].History_id;

            //若是计算项目
            if(taskList[i].isCalculateProj ==1)
            {
                qDebug() << "CalculateProj AD Count:" << taskList[i].AD_Count << lightPos2;

                //是否可以计算出结果 若周期个数大于lightPos2，则会一直遍历该计算项目的所有子项目是否已经完成，若完成则计算出结果
                if(taskList[i].AD_Count >= lightPos2)
                {
                    //计算计算项目的结果
                    QString str = calculateCalculateProjectResult(taskList[i]);
                    if(str != "" )
                    {
                        strList.append( str );
                        gScheduler->FinishedTest(i);
                        qDebug() << i << "计算项目完成...................";
                    }
                }

                //计算结果失败（否则会一直计算下去的，故需要加限制）
                if( taskList[i].AD_Count == MAX_AD_COUNT)
                {
                    qDebug() << "计算 计算项目结果失败。。。。。。。。SaveADData（）。。。";
                    gScheduler->FinishedTest(i);

                    //未完成
                    QString str =
                            "update TempHistoryT set statusID =15 where id = " + QString::number(id) + ";";
                    strList.append(str);
                }

                gScheduler->TaskList[i].AD_Count ++; //加1
                qDebug() << "Pt:" << i << " h_id:" << id << " Count:" << gScheduler->TaskList[i].AD_Count
                         << "isCalculate:" << taskList[i].isCalculateProj;
                continue; //计算项目无需保存AD数据，所有直接进入下一循环
            }

            //常规项目要多采5个AD
            if( taskList[i].AD_Count < lightPos2 +5 && taskList[i].AD_Count < MAX_AD_COUNT)
            {
                //即时计算浓度
                if( taskList[i].AD_Count == lightPos2+1)
                {
                    strList.append( CalculateProjectResult(taskList[i]) );
                    //修改状态、保存历史结果ID-保存到数据库中
                    QString str =
                            "update TaskT "
                            " set stateID  = 11 "  //测试状态
                            " where id ="+ QString::number(taskList[i].id) +";";
                    strList.append(str);
                    //qDebug() << str;
                }

                Count = taskList[i].AD_Count;
                cup = taskList[i].Cup[1];
                ADValue = ADMap.value(cup);
                QString str =
                        "update TempHistoryT set AD_A"+QString::number(Count) //AD
                        +" = " + QString::number(ADValue)
                        +" ,T" + QString::number(Count) //T
                        +" = '" + " " + "'"
                        +" " + " where id = " + QString::number(id) + ";";
                strList.append(str);

                taskList[i].AD_Count ++ ;
                gScheduler->TaskList[i].AD_Count ++; //加1

                qDebug() << "Pt:" << i << " h_id:" << id << " Count:" << Count << " cup:" << cup
                         << " AD:" << ADValue << " isCalculateP:" << taskList[i].isCalculateProj;
                //qDebug() << "cup60: " << ADMap.value(60);

            }
            else //
            {
                //修改状态、保存历史结果ID-保存到数据库中
                QString str =
                        "update TaskT "
                        " set stateID  = 11 "  //测试状态
                        " where id ="+ QString::number(taskList[i].id) +";";
                strList.append(str);
                //qDebug() << str;

                gScheduler->FinishedTest(i);
                qDebug() << i << "测试采光完成了...................";
            }

        }
    }

    SQLite::getInstance()->Transaction(strList);
}

//（常规项目）计算常规项目的幅度、浓度结果
QString TcpServer::CalculateProjectResult(Task task)
{
    double result = 0;
    ProjectParamS pParam = gParameter->getProjectParam(task.ProjectId);

    //反应幅度
    double AD;
    QString str =
            "select AD_A" + QString::number(pParam.Param_lightPos1)+", "
            +" AD_A" + QString::number(pParam.Param_lightPos2)+" "
            +", sampleTypeID from TempHistoryT where id = " + QString::number(task.History_id) + ";";
    //qDebug()  << "计算（浓度）结果:"<< str;
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())
    {
        AD = query.value(1).toDouble() - query.value(0).toDouble(); //反应幅度
        qDebug() << query.value(1).toDouble() << query.value(0).toDouble() << AD;
    }

    if(AD < 0) AD = 0;

    double bloodFactor = pParam.Param_bloodFactor;  //全血因子。sampleType=2，时表示样本类型为全血，
    //logic5P参数
    long double R0, K, a, b, c;
    double C;

    //用第1段参数曲线计算(默认)
    R0 = pParam.Param_R0;
    K = pParam.Param_K;
    a = pParam.Param_a;
    b  = pParam.Param_b;
    c = pParam.Param_c;
    //R = pParam.Param_R;
    C = pParam.Param_C;

    //用第2段参数曲线计算
    if( AD > pParam.Param_R)
    {
        if(pParam.isLogic5P2)
        {
            R0 = pParam.Param2_R0;
            K = pParam.Param2_K;
            a = pParam.Param2_a;
            b = pParam.Param2_b;
            c = pParam.Param2_c;
            C = pParam.Param2_C;
        }
    }

    if( bloodFactor <= 0) bloodFactor = 1;
    if( task.SampleType != 1 ) bloodFactor = 1; //sampleType=1，时表示样本类型为全血

    //计算浓度
    //查看用哪种定标方式计算(目前都用LOGIC5P)
    MV_Logic5P logic5P;
    result = logic5P.Consistence_Logic5P(0, C, R0, K, a, b, c, AD) * pParam.Param_factorK / bloodFactor;

    //项目线性下限
    if( result < pParam.Param_lowLinear )
        result = pParam.Param_lowLinear;

    QString temp =
            "update TempHistoryT set result = " + QString::number(result, 'f', pParam.Param_accuracy)
            +" , amplitude = " + QString::number(AD)
            +" , statusID = 11 "   //一定要的哦（修改状态）
            +" " + " where ID = " + QString::number(task.History_id) + ";";
    return temp;
}
//（计算项目）计算计算项目的结果
QString TcpServer::calculateCalculateProjectResult(Task task)
{
    //非计算项目
    if(task.isCalculateProj != 1) return "";

    int Pid = task.ProjectId;
    QStringList ProjectList = gParameter->getCalculateProjectList(Pid); //计算项目子项目列表
    QString Expression = gParameter->getCalculateProjectExpression(Pid);//计算项目表达式
    int Accracy = gParameter->getCalculateProjectAccracy(Pid);          //计算项目精度
    QString SamplePos = task.SamplePos;

    //qDebug() << "计算项目列表：" << ProjectList << "表达式：" << Expression;

    QMap<QString, double> ResultMap; //结果映射<项目名称，结果>
    QString str;
    QSqlQuery  query;
    int id;  //子项目id
    for( int i = 0; i < ProjectList.count()-1; i ++)
    {
        id = gParameter->getProject_id(ProjectList.at(i));
        str = "select statusID, result, id from TempHistoryT "
              " where projectID=" + QString::number(id) + " and isCalculateProj=0 and SamplePos='" + SamplePos + "'"
                //排序查询：
                + "order by id desc "
                //每次取出的已经申请的测试个数
                " limit 0, 1;";
        //qDebug() << str;
        query = SQLite::getInstance()->Select(str);  //查询数据库
        while(query.next())
        {
            if(query.value(0).toInt() == 11) //完成
            {
                ResultMap.insert(ProjectList.at(i), query.value(1).toDouble());
                qDebug() << ResultMap;
            }
            else//未完成返回空
                return "";
        }
    }

    //计算项目结果
    double result = ExpressionCalculate(Expression, ResultMap, ProjectList);

    //QSL语句
    QString temp =
            "update TempHistoryT set result = " + QString::number(result, 'f', Accracy)
            +" , amplitude = 0"
            +" , statusID = 11 "   //一定要的哦（修改状态）
            +" " + " where ID = " + QString::number(task.History_id) + ";";
    return temp;
}
//（计算项目）根据表达式计算项目的结果
double TcpServer::ExpressionCalculate(QString expression, QMap<QString, double> map, QStringList pList)
{
    //qDebug() << "项目结果:" << map;
    double result;

    for(int i = 0; i < pList.count()-1; i ++)
        expression.replace(pList.at(i), QString::number( map.value(pList.at(i)) ));

    //去掉表达式中“{”、“}”符
    expression.replace("{", "");
    expression.replace("}", "");
    result = AppHelper::CalculateProjResult(expression);
    return result;
}



////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//是否接收到读卡数据
bool TcpServer::get_isDataReceived() {return isDataReceived;}

//清空读卡数据
void TcpServer::clearReadCardData() {m_ReadCardData = "";}
//读卡数据
QString TcpServer::getReadCardData() {return m_ReadCardData;}

//分类处理读卡返回的数据
void TcpServer::SortOutCardData(QString msg)
{
    qDebug() << msg;
    int Count = msg.count();
    if(Count < 8) return;

    isDataReceived = false;
    int head = msg.mid(0, 2).toInt(0, 16);   //包头
    if(head != STX)  return;//检验起始符
    int tail = msg.mid(Count-2, 2).toInt(0, 16);//包尾
    if(tail != ETX) return; //检验帧结束符

    int state = msg.mid(4, 2).toInt(0, 16);    //状态
    int Lenght = msg.mid(6, 2).toInt(0, 16)*2; //数据长度(字节*2)
    QString frameMsg = msg.mid(8, Lenght);     //完整数据帧

    if( state == COMM_OK || state == OK ) //函数调用成功
    {
        isDataReceived = true;
        m_ReadCardData += frameMsg;
    }
    else if( state == NO_TAG_ERR ) //在有效区域内没有卡
    {
        qDebug() << state << "在有效区域内没有卡..." << frameMsg;
        m_ReadCardData = "";
        //可能用户还没有把卡片放置到读卡区，故这里不发射信号出来
        //sig_ReadCard_State(NO_TAG_ERR);//信号：读卡信号（返回状态）
    }
    else if( state == AUTH_ERR )   //不能验证
    {
        qDebug() << state << "不能验证..." << frameMsg;
        m_ReadCardData = "";
        sig_ReadCard_State(AUTH_ERR);//信号：读卡信号（返回状态）
    }
    else if( state == NOT_AUTH_ERR ) //卡没有验证
    {
        qDebug() << state << "卡没有验证..." << frameMsg;
        m_ReadCardData = "";
        sig_ReadCard_State(NOT_AUTH_ERR);//信号：读卡信号（返回状态）
    }
    else //读写卡出错
    {
        qDebug() << state << "出错..." << frameMsg;
        m_ReadCardData = "";
        sig_ReadCard_State(OTHER_ERR);//信号：读卡信号（返回状态）
    }
    return;
}



