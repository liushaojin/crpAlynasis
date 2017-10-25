/*******************************************************************
Copyright (C),    2016~ ,    中山迈胜有限公司，Co
    File Name:    appHelper.h
       Author:    郑文洋
      version:    1
         Date:    2016-01-21
  Description:    全局辅助操作类实现文件
       Others:
Function list:    1.AutoRunWithSystem();
                  2.SetCode();
                  3.SetLanguageQM();
                  4.FolderIsExist();
                  5.CreateFloder();
                  6.CopyFileToPath();
                  7.HexStrToByteArray();
                  8.ConvertHexChar();
                  9.ByteArrayToHexStr();
                  10.getXorEncryptDecrypt();
                  11.StringToHexStr();
                  12.IntToHexStr();
                  13.getModel8_Data();
History:
*********************************************************************/
#ifndef APPHELPER_H
#define APPHELPER_H

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <math.h>
#include <fcntl.h>
#include <QTextCodec>
#include <QTranslator>
#include <QApplication>
#include <QQueue>
#include <QDebug>
#include <QMutex>
#include <QStack>


#define MODEL8                 8  //模八校验
#define RESULT_DATA_COUNT      6  //每次读数据的个数（从中位机中返回的每个反应杯每次读的数据个数）

class AppHelper : public QObject
{
    Q_OBJECT
public:
    //设置为开机启动
    static void AutoRunWithSystem(bool isAutoRun, QString AppName, QString AppPath);
    //设置编码
    static void SetCode(char *code);
    //设置中/英文字符
    static void SetLanguageQM(QString qm);

    //判断一个字符串是否全部都是数字
    static int isDigitStr(QString text);

    //文件夹是否存在
    static bool FolderIsExist(QString strFolder);
    //创建文件夹
    static void CreateFloder(QString strFolder);
    //文件是否存在
    static bool FileIsExist(QString strFile);
    //拷贝文件到指定目录
    static bool CopyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    //16进制字符串转字节数组
    static QByteArray HexStrToByteArray(QString str);
    static char ConvertHexChar(char ch);
    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data);
    static QString ByteArrayToHexStr2(QByteArray data);
    //异或加密算法
    static QString getXorEncryptDecrypt(QString str, char key);

    //字符串转换为十六进制字符串（用于串口发送数据）
    static QString StringToHexStr(QString str, int bits);
    static QString IntToHexStr(int num, int bits);

    //保存日志文件
    static void CustomMessageHandler(QtMsgType type, const char * msg);
    static void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    //根据表达式来计算计算项目结果（何处用：计算项目计算）
    static double CalculateProjResult(QString expression);
    //中缀表达式转为后缀表达式的实现（何处用：计算项目计算）
    static QQueue<QString> mid2end(const char* mid);
    //计算表达式的和（何处用：计算项目计算）
    static double calculate(QQueue<QString> list);
};

#endif // APPHELPER_H
