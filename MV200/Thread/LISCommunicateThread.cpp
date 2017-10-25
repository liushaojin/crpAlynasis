#include "LISCommunicateThread.h"

LISCommunicateThread * gLISCommunicate;
LISCommunicateThread::LISCommunicateThread()
{
    tcpSocket = new QTcpSocket(this);
    //connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()), Qt::QueuedConnection);
    //connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

LISCommunicateThread::~LISCommunicateThread()
{
    tcpSocket->close();
    tcpSocket->deleteLater();
}

//打开&关闭
bool LISCommunicateThread::ConnectoHost( QString ip, int port)
{
    //取消已有连接
    tcpSocket->abort();
    tcpSocket->connectToHost(ip,port);
    if(!tcpSocket->waitForConnected(1000))//等待重新连接到Host,不加上这句会出错
    {
        //        qDebug()<<tcpSocket->state();
        //        gMessageBox->setInfo(QString("提示"), QString("请确保网络连通！"),
        //                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        //        gMessageBox->setButtonText(Cancel, "返回");
        //        gMessageBox->show();
        qDebug() << "请确保网络连通！ LIS" << ip << ":" << port;
        return false;
    }
    qDebug() << "LIS 网络连接成功!!！" << ip << ":" << port;
    return true;
}
bool LISCommunicateThread::DisConnectoHost()
{
    if( tcpSocket->isOpen() )
        tcpSocket->close();
}
//连接状态
bool LISCommunicateThread::ConnectState() {return isRunning();}

//发送数据
int LISCommunicateThread::Write(QStringList idList)
{
    if(this->isRunning())
    {
        qDebug() << "LISCommunicateThread running...";
        return 0; //线程工作中(未执行)
    }

    AllSelectID.clear();
    AllSelectID = idList;

    run();
    //this->start(); //不用线程
    return 1;
}

void LISCommunicateThread::setACK(int ack, quint64 ackTime)
{
    isack = ack;
    ackOverTime = ackTime * 1000;
}


void LISCommunicateThread::run()
{
    for(int i = 0; i < AllSelectID.count(); i ++)
    {
        //qDebug() << "AllSelectID.at(i) = " << AllSelectID.at(i);
        SendtoLIS.clear();
        initSendtoLISDate(AllSelectID.at(i).toInt(), SendtoLIS);

        if(isack == 1)
        {
            int sendtimes = 3;
            while(sendtimes--)
            {
                reservemessage.clear();
                writeMessage();
                const QString temp("MSA|AA|");

                if(tcpSocket->waitForReadyRead(ackOverTime))
                {
                    sleep(2);
                    if(reservemessage.indexOf(temp) >= 0)
                    {
                        qDebug()<< "应答数据接收成功...";
                        sendtimes = 0;
                    }
                    else
                    {
                        qDebug()<< "应答数据错误...";
                        sendtimes = 0;
                    }
                }
                else
                {
                    qDebug()<< "未接收到应答数据...";
                }
            }
        }
        else
        {
            //qDebug() << "isack != 1";
            reservemessage.clear();
            writeMessage();
            sleep(1);
        }

    }

    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}

void LISCommunicateThread::readMessage()
{
    QByteArray ba;
    ba = tcpSocket->readAll();
    reservemessage.clear();
    reservemessage.append(ba) ;

    //接收字符串数据。
    //qDebug() << reservemessage ;
}

void LISCommunicateThread::writeMessage()
{
    //qDebug() << "writeMessage...";
    //qDebug() << "LIS send = " << SendtoLIS;
    QString  tempsend;
    ChangetoHL7 HL7(SendtoLIS);
    QByteArray block;
//    QDataStream out(&block,QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_8);
//    out << (quint32)0;

    for(quint16 i = 0;i < HL7.sendmessage->count();i++)
    {
        QStringList list = HL7.sendmessage->at(i);
        for(int j = 0;j < list.count();j++)
            tempsend += list.at(j);
    }

    block.append(tempsend.toUtf8()) ;
    // block+=(HL7.tuduan);
//    out.device()->seek(0);
//    out << (quint32)(block.size() - sizeof(quint32));

    if(tcpSocket->isWritable()==false)
    {
        qDebug() << "LIS make sure the network is connected!!!";
        return;
    }

    int state = tcpSocket->write(block);
    if( state == -1)
    {
        qDebug() << "LIS make sure the network is connected!!!";
        //msgBox.setTipInfo(tr("请确保网络连通!"));
    }

    //qDebug() << "state:" << state;

    if( tcpSocket->waitForBytesWritten() )
    {
        //qDebug() << "LIS send SUCCESS";
        return;
    }
    else
        sleep(1);
}

/*
send.at()
0--id
1--样本类型(质控Q，样本P)
2--样本编号
3--姓名
4--生日
5--性别(男M 女F 其它O)
6--门诊
7--科室
8--费别（自费）
9--样本结果编号
10--采样时间
11--检验时间
12--送检者
13--病症信息（感冒）
14--送检时间
15--血样模式（静脉血，末梢血）
16--审核时间
while(20--)
17--数据类型（NM,IS,ED）
18--项目编号
19--项目值
20--单位
21--参考范围
*/

//初始化消息()
void LISCommunicateThread::initSendtoLISDate( int id, QStringList &send)
{
    QStringList valuelist;
    valuelist.clear();
    getDataForm_db(id, valuelist);

    send.clear();
    send.append(QString::number(id));       //0--id
    send.append("P");                       //1--样本类型(质控Q，样本P)
    send.append(valuelist.at(7));           //2--样本编号
    send.append("");                        //3--姓名
    send.append("");                        //4--生日
    send.append("");                        //5--性别(男M 女F 其它O)

    send.append("");                  //6--病人类型
    send.append("");                  //7--科室
    send.append("");                  //8--自费
    send.append("");                  //9--样本结果编号
    send.append("");                  //10--采样时间
    send.append("");                  //11--检验时间
    send.append("");                  //12--送检者
    send.append("");                  //13--病症信息
    send.append(valuelist.at(2));     //14--送检时间
    send.append(valuelist.at(9));     //15--血样模式（样本类型）
    send.append("");                  //16-审核时间

    send.append("NM");                         //17--数据类型（NM,IS,ED）
    send.append("^" + valuelist.at(5) + "^");  //18--项目编号
    send.append(valuelist.at(10));             //19--项目值
    send.append(valuelist.at(6));               //20--单位
    send.append("0-1");                        //21--参考范围

    for(int i=0;i<29;i++)
    {
        send.append("NM"); //17--数据类型（NM,IS,ED）
        send.append("");   //18--项目编号
        send.append("");   //19--项目值
        send.append("");   //20--单位
        send.append("");   //21--参考范围
    }

    //22~46
    for(int i = 22; i < 47; i ++)
        send.append("");
}

//查询数据库
void LISCommunicateThread::getDataForm_db(int id, QStringList &data)
{
    //0-测试编号，1-申请时间，2-送检时间，3-测试时间，4-报告时间
    //5-项目名称，6-项目单位，7-样本条码，8-样本类型
    //9-反应幅度，10-结果浓度，
    QString str =
            "select TempHistoryT.no, TempHistoryT.applyTime, TempHistoryT.inspectionDate, TempHistoryT.testDate, TempHistoryT.reprotDate, "
            " ProjectT.name, ProjectT.unit, TempHistoryT.sampleCode, SampleTypeT.name, "
            " TempHistoryT.amplitude, TempHistoryT.result "
            " from TempHistoryT "
            //连接方式
            " Inner join ProjectT ON TempHistoryT.projectID = ProjectT.id "
            " Inner join SampleTypeT ON TempHistoryT.sampleTypeID = SampleTypeT.id "
            " where TempHistoryT.id = " + QString::number( id ) + ";";

    QSqlQuery query = SQLite::getInstance()->Select(str);  //查询数据库
    while( query.next() )
    {
        for(int i = 0; i < 11; i ++ )
            data.append( query.value(i).toString() );
    }
}















