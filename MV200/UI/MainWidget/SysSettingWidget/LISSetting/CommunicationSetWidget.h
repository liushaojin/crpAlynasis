#ifndef COMMUNICATIONSETWIDGET_H
#define COMMUNICATIONSETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include "Parameter.h"
#include "UI/Widget/IPAddressEdit/IPAddressEdit.h"

class CommunicationSetWidget : public QWidget
{
    Q_OBJECT
public:
    static CommunicationSetWidget *getInstance();

private:
    static CommunicationSetWidget *_instance;
    explicit CommunicationSetWidget(QWidget *parent = 0);

    void initForm();                           //初始化界面控件
    void initStyle();                          //初始化样式
    void initData();                           //初始化数据
    void initConnect();                        //连接信号与槽

    void QueryLISParam();         //查询LIS参数表

signals:

public slots:
    void slt_btnSaveIP_Clicked();         //槽：保存中位机IP
    void slt_btnResetIP_Clicked();        //槽：更新中位机IP
    void slt_ResetServerIP(int state);    //槽：修改中位机IP是否成功（0：失败， 1：成功）
    void slt_btnSave_Clicked();           //槽：保存
    void slt_btnCancel_Clicked();         //槽：取消

private:
    //MV200服务器地址
    QGroupBox *grbServer;
    QLabel *labServer_IP;              //服务器 ip地址
    IPAddressEdit *Server_IPAdress;
    QLabel *labServerPort;             //服务器 端口
    QLineEdit *editServerPort;
    QPushButton *btnSaveIP;            //保存中位机IP
    QPushButton *btnResetIP;           //更新中位机IP
    QGridLayout *lytServer;

    QGroupBox *grbLis;
    QLabel *labAgreement;           //通讯协议
    QComboBox *cboAgreement;
    QLabel *labLIS_IP;              //Lis ip地址
    IPAddressEdit *LIS_IPAdress;
    QLabel *labLISPort;             //Lis 端口
    QLineEdit *editLISPort;
    QGridLayout *lytLis;

    QPushButton *btnSave;           //保存
    QPushButton *btnCancel;         //取消

};

#endif // COMMUNICATIONSETWIDGET_H













