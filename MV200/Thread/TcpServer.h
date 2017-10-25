#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QThread>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMutex>
#include <QVector>
#include <QMap>
#include <QTimer>
#include <QDateTime>
#include <QApplication>

#include "Scheduler.h"
#include "Parameter.h"
#include "Object/Logic5P/MV_Logic5P.h"
#include "Object/ErrorTable/MV_ErrTable.h"

#define TIME_RESPONSE    2000	  //命令应答帧的等待时间

/**************TX522库函数的用到的状态值列表************************************/
//      名称               状态值（Status）       描述
#define OK                   0                 //函数调用成功
#define COMM_OK              0                 //函数调用成功
#define NO_TAG_ERR           1                 //在有效区域内没有卡
#define NOT_AUTH_ERR         0x0a              //卡没有验证
#define AUTH_ERR             4                 //不能验证
#define OTHER_ERR            100               //其他错误（hituck add ）
//TX522读写卡 定义通信帧常量
#define STX                     0x20       //开始符
#define ETX                     0x03       //终止符

//当前通讯指令执行状态（是否能够发送下一帧指令）
enum CmdRuningState
{
    CurrentCMD_WiatResult = 0, //等待当前指令返回结果
    CurrentCMD_Resulted = 1,   //当前指令已经返回结果
    CurrentCMD_Error = 2,      //当前指令执行有错误返回
    CurrentCMD_OverTime = 3    //当前指令超时
};

class TcpServer : public QThread
{
    Q_OBJECT
public:
    explicit TcpServer(QThread *parent = 0);
    int StartMonitor();                      //开始监控网口返回数据

    bool isTcpConnected();                    //TCP服务器连接状态
    bool get_isRespnd();                 //是否接收到应答帧
    void set_isRespnd(bool flag);
    bool get_isResult();                 //是否接收到结果帧
    void set_isResult(bool flag);
    int get_isCanSendNextFrame_Flag();   //是否能发送下一帧通讯指令
    void set_isCanSendNextFrame_Flag(int flag);

    bool get_isDataReceived();   //是否接收到读卡数据
    void clearReadCardData();    //清空读卡数据
    QString getReadCardData();   //读卡数据

private:
    int m_Type;
    QString TCPip;
    int TCPport;
    void initParam();     //初始化参数
    int Send(QString data);                   //发送数据
    int Send(char *data, int lenght);         //发送数据
    QHostAddress *ServerIP;         //TCP服务器IP
    QTcpSocket *TcpSocket;          //TCP服务器
    bool isTcpSocketConnected;      //TCP服务器是否连接成功
    QMutex mutex;         //临界区，一次只能有一个线程进入临界区
    int isCanSendNextFrame_Flag;    //是否能发送下一帧通讯指令
    bool isRespnded;      //是否接收到应答帧
    bool isResulted;      //是否接收到结果帧
    QTimer *timerRespond; //指令应答计时器
    QTimer *timerResult;  //指令结果计时器
    QQueue<QString> m_ReceiveData; //暂存网口返回数据的队列
    QMap<int, int> ADMap; //保存AD数据的映射表（杯号，AD值）
    struct Task *taskList;   //任务队列,保存测试信息

    char ConvertHexChar(char ch);
    QByteArray HexStrToByteArray(QString str);
    QString ByteArrayToHexStr(QByteArray data);    //字节数组转16进制字符串
    QString ByteArrayToHexStr2(QByteArray data);   //字节数组转16进制字符串(中间以空格间隔)
    int CheckSum(char *cmd, int lenght);     //计算校验和(校验和是从帧头到校验位前一字节的累加和的低7位，高位固定为0)
    void SortData(QString data);   //分类处理数据
    int CheckSum(QString data);    //计算校验和
    int CmdRespondFrame(int Sid, int Cid, QVector<QString> ParamList);   //命令应答帧（Sid:发送者ID，Cid：命令ID，ParamList：参数列表）
    int CmdResultFrame(int Sid, int Cid, QVector<QString> ParamList);    //命令结果帧
    int CmdADDataFrame(int Sid, int Cid, QVector<int> ParamList);        //数据帧
    int CmdWarningFrame(int Sid, int Cid, QVector<QString> ParamList);   //警告信息帧
    //清空AD数据
    void clearADMap();
    //保存AD数据到数据库中
    void SaveADData();
    //（常规项目）计算常规项目的幅度、浓度结果
    QString CalculateProjectResult(Task t);
    //（计算项目）计算计算项目的结果
    QString calculateCalculateProjectResult(Task task);
    //（计算项目）根据表达式计算项目的结果
    double ExpressionCalculate(QString expression, QMap<QString, double> map, QStringList pList);

    //读卡功能
    QString m_ReadCardData; //读卡返回数据
    bool isDataReceived;   //是否接收到读卡数据
    void SortOutCardData(QString data);  //分类处理读卡返回的数据

protected:
    void run();

signals:
    void sig_timerRespond_start(int);
    void sig_timerResult_start(int);
    void sig_timerRespond_stop();
    void sig_timerResult_stop();
    void sig_AlertMsg_show(int,QString msg);    //信号：显示错误信息
    void sig_ReadCard_State(int state);         //信号：读卡信号（返回状态）

public slots:
    void slt_TCP_Connect(QString, int);//槽：连接服务器
    void slt_TCP_disConnect();         //槽：断开服务器
    void slt_Connected();        //槽：连接TCP服务器成功
    void slt_Disconnected();     //槽：断开服务器的连接
    void slt_Send(char *data, int lenght);   //槽：发送数据
    void slt_Send(QString data); //槽：发送数据
    void slt_dataReceived();     //槽：接收服务器发送的数据
    void slt_timerRespond_timeout(); //槽：指令应答计时器
    void slt_timerResult_timeout();  //槽：指令结果计时器

};
extern TcpServer *gTcpServer;
#endif // TCPSERVER_H
