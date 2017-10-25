#ifndef LISCOMMUNICATETHREAD_H
#define LISCOMMUNICATETHREAD_H

#include <QThread>
#include <QAbstractSocket>
#include <QtNetwork>
#include <QTcpSocket>
#include <QMutex>
#include "UI/Widget/MessageBox/messageBox.h"
#include "Object/HL7/changetohl7.h"
#include "Object/SQLite/sqlite.h"

class LISCommunicateThread : public QThread
{
    Q_OBJECT
public:
    explicit LISCommunicateThread();
    ~LISCommunicateThread();

    //打开&关闭
    bool ConnectoHost( QString ip, int port);
    bool DisConnectoHost();
    //连接状态
    bool ConnectState();
    //发送数据
    int Write(QStringList idList);
    //
    void setACK(int ack, quint64 ackTime);

protected:
    void run();

signals:
    void CommunicationThreaderror();

protected slots:
    void readMessage();
    void writeMessage();

private:
    QMutex mutex;
    QTcpSocket *tcpSocket;
    QStringList SendtoLIS;       //要发送到LIS的信息
    QString reservemessage;      //LIS返回内容
    QStringList AllSelectID;     //要发送的信息id
    int isack;
    quint64 ackOverTime;

    //初始化消息
    void initSendtoLISDate( int id,QStringList &send);
    //查询数据库
    void getDataForm_db(int id, QStringList &data);
};
extern LISCommunicateThread *gLISCommunicate;
#endif // LISCOMMUNICATETHREAD_H
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
