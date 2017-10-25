#ifndef REAGENTINFOWIDGET_H
#define REAGENTINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "ParamDefine.h"
#include "Parameter.h"
#include "UI/Widget/MessageBox/messageBox.h"
#include "UI/MainWidget/ApplicationWidget/SampleApplication.h"
#include "UI/MainWidget/ProjectSettingWidget/ProjectGroupWidget.h"
#include "UI/MainWidget/ProjectSettingWidget/CalculateProjectWidget.h"
#include "UI/Widget/QFlowLayout/QFlowLayout.h"

#define REAGETN_ROWCOUNT   3

class ReagentInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReagentInfoWidget(QWidget *parent = 0);

private:
    void initForm();              //初始化
    void initReagentForm();       //初始化
    void initReagentInfoForm();   //初始化
    void initTableFormat();       //
    void initStyle();             //界面风格
    void initStyle2();            //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据
    void ClearTable();            //清空试剂信息表

    //设置试剂的状态
    void setReagentStatus(int rNo, QString objName);
    void QueryReagentInfo(QString project, QString batchNo, int RNo, int vol);   //查询项目的各种试剂的信息

    int CurrentRow;       //当前试剂的行数
    QString SaveReagentType(QString type, int pos); //保存试剂盘上对应位置的试剂类型（R0、R1、R2）
    int R0_v, R1_v, R2_v; //项目试剂加样时吸取的量（ul）
    int vDiluent, vR1, vR2; //项目对应在试剂仓的试剂位置信息

    //保存前检查试剂位置对应的状态
    bool CheckReagentState(int Pid, int R0, int R1, int R2);
    void setUsedReagentPosState();          //设置试剂仓上已经被项目占用的试剂位置（设置成“已使用”状态）
    void setProjectReagentCount(int count); //设置项目的试剂位个数（即可以选择的试剂个数，目前只分双试剂和三试剂两种，即稀释和非稀释的区别）
    void SaveReagentPos();    //保存项目位置到数据库中

signals:
    void sig_updatePReagentPos();   //信号：更新项目的试剂位信息

public slots:
    void slt_btnReagent_Clicked();           //槽：查看项目
    void slt_btnSaveReagentPos_Clicked();    //槽：保存项目试剂位置信息
    void slt_UpdateRoutineItem();            //槽：更新常规项目
    void slt_btnRelease_Clicked();           //槽：释放当前选中的项目试剂位置
    void slt_btnReleaseAll_Clicked();        //槽：全盘释放

private:
    //试剂模拟图
    QWidget *wdgReagent;
    QPushButton *btnReagent[REAGENT_COUNT];
    QGridLayout *ltyReagent;

    //试剂模拟图状态说明
    //正常、不足、耗尽、已过期、稀释液、R1、R2
    QWidget *wdgStatusInfo;
    QGridLayout *lytStatusInfo;
    QLabel *labStatusName[10];      //状态名称
    QLabel *labStatusPic[10];       //状态图标

    //
    QWidget *wdg;
    QGridLayout *lyt;

    //项目列表
    QGroupBox *grbProject;
    QGridLayout *lytProject;
    QRadioButton *rdoProject[PROJECT_COUNT];

    //项目试剂位信息
    QWidget *wdgProjectReagentPos;
    QGridLayout *lytProjectReagentPos;
    QLabel *labProjectName;
    QLabel *labReagent[3];
    QComboBox *cboReagent0;         //试剂-稀释液
    QComboBox *cboReagent1;         //试剂-R1
    QComboBox *cboReagent2;         //试剂-R2
    QPushButton *btnSaveReagentPos; //保存
    QPushButton *btnRelease;        //释放当前选中的项目试剂位置
    QPushButton *btnReleaseAll;     //全盘释放

    //项目试剂信息列表
    QTableView *tbvReagentList;
    QStandardItemModel *modelList;
    QWidget *wdg2;
    QGridLayout *lyt2;

    //
    QGridLayout *layout;

};

#endif // REAGENTINFOWIDGET_H
