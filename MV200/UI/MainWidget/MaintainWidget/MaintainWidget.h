#ifndef MAINTAINWIDGET_H
#define MAINTAINWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QDialog>
#include <QProcess>
#include <QProgressBar>

//#include "CmdTypeDefine.h"
//#include "Object/CmdSetting.h"
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"
#include "Thread/Scheduler.h"
//#include "SubInterface/ErrorMsgWidget/ErrorMsgWidget.h"

class MaintainWidget : public QWidget
{
    Q_OBJECT
public:
    static MaintainWidget *getInstance();          //实例对象

private:
    explicit MaintainWidget(QWidget *parent = 0);
    static MaintainWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据


signals:

public slots:
    void slt_btnDeleteTable_Clicked();     //槽：删除数据表
    void slt_btnRebuildSQL_Clicked();      //槽：重检数据库

    void slt_btnStartMaintain_Clicked();   //槽：开始维护
    void slt_UpdateProgressBar(int s);     //槽：更新进度条的状态
    void slt_finishedMaintain(int type);   //槽：完成维护或者维护过程中出错(type=0：完成维护，type=1：维护出错)

private:
    //数据库维护
    QGroupBox *grbSQLMatain;
    QLabel *labSQLTableName;     //表名
    QComboBox *cboSQLMatain;     //数据表下拉列表
    QPushButton *btnDeleteTable; //删除数据表
    QPushButton *btnRebuildSQL;  //重检数据库
    QGridLayout *lytSQLMatain;

    //针、盘维护
    QGroupBox *grbMaintain;
    QCheckBox *chkMaintainProbe;        //加样针维护
    QLabel *labMaintainLiquid;          //针维护清洗液位置
    QComboBox *cboMatainLiquidPos;
    QLabel *labProbeMaintainVol;        //注射器吸清洗液的量
    QLineEdit *editProbeMaintainVol;
    QLabel *labProbeMaintainTimes;      //维护次数
    QLineEdit *editProbeMaintainTimes;
    QCheckBox *chkMaintainReactionDisk; //反应盘维护
    QLabel *labRDiskMaintainTime;       //加完清洗液后浸泡的时间
    QLineEdit *editRDiskMaintainTime;
    QLabel *labRDiskMaintainVol;        //反应盘吸清洗液的量
    QLineEdit *editRDiskMaintainVol;
    QPushButton *btnStartMaintain;      //开始维护
    QGridLayout *lytMaintain;

    QDialog *digMaintainBar;         //针、盘维护进度条提示窗
    QProgressBar * barReadingStatus; //进度条
    QLabel *labTime;                 //剩余时间
    QLabel *labTime1;
    QGridLayout *lytMaintainBar;

    //查看日志
    QGroupBox *grbDebugFile;
    QPushButton *btnCheckDebugFile;  //查看日志文件
    QGridLayout *lytDebugFile;

    //主布局
    QGridLayout *layout;
};

#endif // MAINTAINWIDGET_H
