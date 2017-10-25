#include "AppHelper.h"

//设置为开机启动
void AppHelper::AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
{
    QSettings *reg = new QSettings
            ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
             QSettings::NativeFormat);

    if (IsAutoRun)
        reg->setValue(AppName, AppPath);
    else
        reg->setValue(AppName, "");
}

//设置编码
void AppHelper::SetCode(char *code)
{
    //QT5版本以上默认就是采用UTF-8编码
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec = QTextCodec::codecForName(code);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);    //支持中文文件名显示
    QTextCodec::setCodecForTr(codec);          //支持Tr中文
#endif
}

//设置中/英文字符
void AppHelper::SetLanguageQM(QString qm)
{
    QTranslator *translator = new QTranslator(qApp);
    translator->load(qm);
    qApp->installTranslator(translator);
}

//判断一个字符串是否全部都是数字
int AppHelper::isDigitStr(QString text)
{
    QByteArray byte = text.toLatin1();//QString 转换为 char
    const char *temp = byte.data();

    while( *temp >= '0' && *temp<= '9' && *temp )
        temp ++;

    if (*temp) return 0; //不是纯数字
    else return 1;         //纯数字
}

//文件夹是否存在
bool AppHelper::FolderIsExist(QString strFolder)
{
    QDir tempFolder(strFolder);
    //qDebug() << strFolder << ":" << tempFolder.exists();
    return tempFolder.exists(); //返回文件夹是否存在
}

//创建文件夹
void AppHelper::CreateFloder(QString strFolder)
{
    QDir dir(strFolder);
    if(! dir.exists() )  //若文件夹不存在，则创建新的文件夹
    {
        dir.mkdir(strFolder);
        //qDebug() << "CreateFloder" << strFolder;
    }
}

//文件是否存在
bool AppHelper::FileIsExist(QString strFile)
{
    QFile tempFile(strFile);
    //qDebug() << strFile << ":" << tempFile.exists();
    if (tempFile.size() == 0) return false; //若文件大小为0，当作该文件不存在
    return tempFile.exists(); //返回文件夹是否存在
}

//拷贝文件到指定目录下
bool AppHelper::CopyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    //toDir.replace("\\","/");      //多余语句（WIN下才有作用）
    if (sourceDir == toDir)       //源目录和目标目录一样
        return true;
    if (!QFile::exists(sourceDir))//源目录出错（路径或文件不存在）
        return false;

    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if(exist)                 //目标路径中存在该文件
        if(coverFileIfExist)   //覆盖存在的文件
            createfile->remove(toDir);

    if(!QFile::copy(sourceDir, toDir)) //拷贝文件失败
        return false;
    return true;                       //拷贝文件成功
}

//16进制字符串转字节数组
QByteArray AppHelper::HexStrToByteArray(QString str)
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

char AppHelper::ConvertHexChar(char ch)
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

//字节数组转16进制字符串
QString AppHelper::ByteArrayToHexStr2(QByteArray data)
{
    QString temp = "";
    QString hex = data.toHex();
    for (int i = 0; i < hex.length(); i = i + 2)
    {
        temp += hex.mid(i, 2) + " ";
    }
    return temp.trimmed().toUpper();
}
QString AppHelper::ByteArrayToHexStr(QByteArray data)
{
    QString temp = "";
    QString hex = data.toHex();
    for (int i = 0; i < hex.length(); i = i + 2)
    {
        temp += hex.mid(i, 2);
    }
    return temp.trimmed().toUpper();
}

//异或加密算法
QString AppHelper::getXorEncryptDecrypt(QString str, char key)
{
    QByteArray data = str.toLatin1();
    int size = data.size();
    for (int i = 0; i < size; i++) {
        data[i] = data[i] ^ key;
    }
    return QLatin1String(data);
}

//字符串转换为十六进制字符串（用于串口发送数据）
QString AppHelper::StringToHexStr(QString str, int returnBits)
{
    //将字符串转换为十六进制字符串,结果不够returnBits个数时，前边补“0”
    //若returnBits=2时;如 16的十六进制是"f"，不够2位则返回时补“0”，返回“0f”。
    //若returnBits=4时;如 16的十六进制是"f"，不够2位则返回时补“0”，返回“000f”

    QString temp;
    temp = QString::number( str.toInt(),16); //转化为16进制

    if(temp.count() == returnBits)
        return temp;
    else
    {
        int supple = returnBits - temp.count();
        for(int i = 0; i < supple; i ++)
            temp = "0" + temp;
        return temp;
    }
}

//整形转换为十六进制字符串（用于串口发送数据）
#include <QTime>
#include <QTimer>
QString AppHelper::IntToHexStr(int num, int returnBits)
{
    //将整形转换为十六进制字符串,结果不够returnBits个数时，前边补“0”
    //若returnBits=2时;如 16的十六进制是"f"，不够2位则返回时补“0”，返回“0f”。
    //若returnBits=4时;如 16的十六进制是"f"，不够2位则返回时补“0”，返回“000f”

    QTime time;
    time.start();

    QString temp;
    temp = QString::number( num,16);

    if(temp.count() == returnBits)
        return temp;
    else
    {
        int supple = returnBits - temp.count();
        for(int i = 0; i < supple; i ++)
            temp = "0" + temp;
        return temp;
    }

    qDebug() << "num:" << num << time.elapsed();
}

//保存日志文件（Qt version<5）
void AppHelper::CustomMessageHandler(QtMsgType type, const char * msg)
{
    static QMutex mutex;
       mutex.lock();
       QString txt;
       switch (type)
       {
       case QtDebugMsg:    //调试信息提示
           txt = QString("%1").arg(msg);
           break;
       case QtWarningMsg:  //一般的warning提示
           txt = QString("Warning: %1").arg(msg);
           break;
       case QtCriticalMsg: //严重错误提示
           txt = QString("Critical: %1").arg(msg);
           break;
       case QtFatalMsg:    //致命错误提示
           txt = QString("Fatal: %1").arg(msg);
           abort();
       }

       ////v1.0 before:20170601
       //    QFile outFile("DebugLog.txt");
       //    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
       //    QTextStream ts(&outFile);
       //    //ts << txt << endl;  //endl不起换行作用（why？？？）
       //    ts << txt << "\r\n"; //换行
       //    outFile.close();

       ////v2.0 date:20170601
       QFile outFile1("DebugLog.txt");
       QFile outFile2("DebugLog_old.txt");
       outFile1.open(QIODevice::WriteOnly | QIODevice::Append);
       if(outFile1.size() >= 1024*1024*8 ) //日志文件如果大于5MB，则将日志拷贝到“DebugLog_old”中，再新建“DebugLog”。
       {
           outFile1.close();
           outFile2.remove();
           QFile::copy("DebugLog.txt","DebugLog_old.txt");
           outFile1.close();
           outFile1.remove();

           QFile outFile3("DebugLog.txt");
           outFile3.open(QIODevice::WriteOnly | QIODevice::Append);
           QTextStream ts(&outFile3);
           ts << txt << "\r\n";  //换行
           //ts << txt << endl;  //endl不起换行作用（why？？？）
           outFile3.close();
       }
       else
       {
           QTextStream ts(&outFile1);
           //ts << txt << endl;
           ts << txt << "\r\n"; //换行
           outFile1.close();
       }
       mutex.unlock();
}
//保存日志文件（Qt version>5）
void AppHelper::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    //QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    //QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString current_date = QString("(%1)").arg(current_date_time);
    //QString message = QString("%1 %2").arg(msg).arg(current_date);
    QString message = QString("%1").arg(msg);

    QFile outFile1("DebugLog.txt");
    QFile outFile2("DebugLog_old.txt");
    outFile1.open(QIODevice::WriteOnly | QIODevice::Append);
    if(outFile1.size() >= 1024*1024*10 ) //日志文件如果大于10MB，则将日志拷贝到“DebugLog_old”中，再新建“DebugLog”。
    {
        outFile1.close();
        outFile2.remove();
        QFile::copy("DebugLog.txt","DebugLog_old.txt");
        outFile1.close();
        outFile1.remove();

        QFile outFile3("DebugLog.txt");
        outFile3.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile3);
        ts << message << "\r\n";  //换行
        outFile3.close();
    }
    else
    {
        QTextStream ts(&outFile1);
        ts << message << "\r\n"; //换行
        outFile1.close();
    }
    mutex.unlock();
}





//根据表达式来计算计算项目结果（何处用：计算项目的计算）
double AppHelper::CalculateProjResult(QString expression)
{
    char mid[50];
    //获取字符串QString 转换成 char*
    //qDebug() << endl << "Expression:" << expression;
    QByteArray ba = expression.toLatin1();
    strcpy(mid,ba.data()); //C语言标准库函数strcpy，把从src地址开始且含有'\0'结束符的字符串复制到以mid开始的地址空间。

    //调用中缀表达式转为后缀表达式的计算
    QQueue<QString> list = mid2end(mid);

    //调用计算函数
    double result;
    result = calculate(list);
    //qDebug()<<"result:"<<result;
    delete mid;
    return result;
}
//中缀表达式转为后缀表达式的实现（何处用：计算项目的计算）
QQueue<QString> AppHelper::mid2end(const char* mid)
{
    int i=0;
    int j=0;
    QString number;
    QStack <char> mystack;    //栈
    QQueue<QString> QueueList;//队列，保存后辍表达式
    QueueList.clear();

    while(mid[i])
    {
        //遇到数字直接输出
        if( (mid[i]>='0' && mid[i]<='9') || mid[i] == '.')
            number.append(mid[i]);
        else
        {
            if( !number.isEmpty() )
            {
                QueueList.push_back(number);
                number.clear();
            }
        }


        if(mid[i]=='-'||mid[i]=='+')
        {
            //加号和减号的操作
            if(!mystack.isEmpty())
            {
                while((!mystack.isEmpty()) && (mystack.top()=='+'||mystack.top()=='-'||mystack.top()=='*'||mystack.top()=='/'))
                    QueueList.append(QString(mystack.pop()));          //只要是加号或者减号 栈中的无论是什么，出干净
            }
            mystack.push(mid[i]);
        }
        else if(mid[i]=='*'||mid[i]=='/')
        {
            if(!mystack.isEmpty())
            {
                while((!mystack.isEmpty()) && (mystack.top()=='*'||mystack.top()=='/'))
                    QueueList.append(QString(mystack.pop()));
            }
            mystack.push(mid[i]);
        }
        else if('('==mid[i])
        {
            mystack.push(mid[i]);
        }
        else if(')'==mid[i])
        {
            if(!mystack.isEmpty())
            {
                while((!mystack.isEmpty()) && mystack.top()!='(')
                    QueueList.append(QString(mystack.pop()));
            }
            mystack.pop(); //将之前进栈的“(”出栈
        }
        //qDebug() << "i:" << i << QueueList << mystack;
        i++;
    }

    if( !number.isEmpty() )
        QueueList.push_back(number);

    //扫描结束将所有的操作符出栈
    while(!mystack.isEmpty())
        QueueList.append(QString(mystack.pop()));

    //qDebug() << "queue" << QueueList;
    return QueueList;
}
//计算表达式的和（何处用：计算项目的计算）
double AppHelper::calculate(QQueue<QString> queue)
{
    //qDebug() << endl << "calculate()" << end;
    int i=0;
    double a;
    double b;
    QStack <QString> stack;
    QString temp;
    bool isOpreator = false;//是否为运算符
    while(!queue.isEmpty())
    {
        temp = queue.at(0);
        queue.pop_front();
        //qDebug() << "i:" << i << "temp:" << temp << stack;

        //判断当前出队的是否为运算符
        if(temp == "+" || temp == "-" || temp == "*" || temp == "/")
            isOpreator = true;

        if( isOpreator )
        {
            char type = temp.data()->toLatin1();
            switch(type)
            {
            case '+':
                b = stack.pop().toDouble();
                a = stack.pop().toDouble();
                stack.push( QString::number(a+b) );
                //qDebug()<<a<<"+"<<b<<"="<<stack.top().toDouble();
                break;
            case '-':
                b = stack.pop().toDouble();
                a = stack.pop().toDouble();
                stack.push( QString::number(a-b) );
                //qDebug()<<a<<"-"<<b<<"="<<stack.top();
                break;
            case '*':
                b = stack.pop().toDouble();
                a = stack.pop().toDouble();
                stack.push( QString::number(a*b) );
                //qDebug()<<a<<"*"<<b<<"="<<stack.top();
                break;
            case '/':
                b = stack.pop().toDouble();
                a = stack.pop().toDouble();
                stack.push( QString::number(a/b) );
                //qDebug()<<a<<"/"<<b<<"="<<stack.top();
                break;
            }
        }
        else
            stack.push( temp );
        i++;
        isOpreator = false;
    }
    return stack.top().toDouble();
}







