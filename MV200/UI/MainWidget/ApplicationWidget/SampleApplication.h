#ifndef SAMPLEAPPLICATION_H
#define SAMPLEAPPLICATION_H

#include <QWidget>
#include <QHBoxLayout>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qtreewidget.h>
#include <QDateTime>

#include "ParamDefine.h"
#include "Parameter.h"
#include "Thread/ApplicateThread.h"
#include "Thread/Scheduler.h"
#include "../../Widget/MessageBox/messageBox.h"
#include "../../Widget/SampleDisk/SampleDiskWidget.h"
#include "SonWidget/ApplicationTaskListWidget.h"
#include "SonWidget/SampleDiskInfoWidget.h"
#include "UI/Widget/QFlowLayout/QFlowLayout.h"


class SampleApplication : public QWidget
{
    Q_OBJECT
public:
    explicit SampleApplication(QWidget *parent = 0);

private:
    void initForm();               //初始化
    void initSampleInfoForm();     //申请测试的样本的信息
    void initProjectListForm();    //项目列表(常规项目&计算项目)
    void initCombinationItemForm();//组合项目
    void initStyle();              //界面风格
    void initConnect();            //连接信号与槽
    void initData();               //初始化数据

    int RequestedNO;  //已经申请的测试的个数，每次开机都清零
    QMap<int, QString> SampleNoMap; //样本编号映射表

    //更新测试编号
    QString UpdateRequestedNO();
    //清空已经选择的样本位
    void CleanSelectSamplePos();
    //清空已经选择的项目
    void CleanAllProject();
    //按组合项目id的成员，设置常规项目是否选中flag（返回不能申请的项目的名称，即未设置试剂位的项目）
    QStringList setRoutineCheckState(int id, bool flag);
    //根据项目名称长度来确定空格长度
    QString FillSpace(int count);



signals:
    void sig_UpdateApplicationTaskList(); //信号：更新申请列表

public slots:
    void slt_btnApplicate_Clicked();       //槽：申请
    void slt_btnSamplePos_Clicked();       //槽：选择样本位
    void slt_SelectedSampleList(QString s);//槽：已经选择的样本
    void slt_SelectedSampleList(QString s, QMap<int, QString> NoMap); //槽：已经选择的样本位
    void slt_btnApplicateList_Clicked();   //槽：申请列表
    void slt_UpdateRoutineItem();          //槽：更新常规项目
    void slt_UpdateComputingItem();        //槽：更新计算项目
    void slt_UpdateCombinationItem();      //槽：更新组合项目
    void slt_UpdateApplicatedCount();      //槽：实时更新数据库中状态为“等待”的测试的个数
    void slt_CombineProject_Clicked();     //槽：选择组合项目时在常规项目中选中相应的项目
    void slt_CalculateProject_Clicked();   //槽：选择计算项目时判断该计算项目的子项目是否全部可以申请测试

private:
    ApplicationTaskListWidget *applicationTaskListWidget; //申请列表

    //申请测试的样本的信息
    QWidget *wdgSampleInfo;
    QHBoxLayout *lythSampleInfo;
    QLabel *labSamplePos;        //样本位
    QLineEdit *editSamplePos;
    QPushButton *btnSamplePos;
    QLabel *labNo;               //测试编号
    QLineEdit *editNo;
    QPushButton *btnSampleCode;  //设置样本编号
    QLabel *labSampleNo;         //样本编号
    QLineEdit *editSampleNo;
    QLabel *labSampleType;       //类型
    QComboBox *cboSampleType;
    QLabel *labSampleRepeat;     //重复(样本重复测试的个数)
    QLineEdit *editSampleRepeat;
    QLabel *labBatchApplicate;   //批量
    QComboBox *cboBatchApplicate;
    QPushButton *btnDetailInfo;  //详细信息

    //项目列表
    QGroupBox *grbProjectList;
    QTreeWidget *treeWidget;
    QRadioButton *rdoRoutineItem[PROJECT_COUNT];     //常规项目
    QRadioButton *rdoComputingItem[PROJECT_COUNT];   //计算项目
    QVBoxLayout *lytvProjectList;

    //组合项目
    QGroupBox *grbCombinationItem;
    QRadioButton *rdoCombinationItem[COMBINATIONITEMCOUNT];   //组合项目
    QFlowLayout *lytvCombinationItem;

    //样本空白
    QWidget *wdgSampleBlank;
    QCheckBox *chkSampleBlank;   //样本空白
    QLabel *labRepeat;           //重复
    QComboBox *cboRepeat;
    QLabel *labDiluentTime;      //稀释倍数
    QComboBox *cboDiluentTime;
    QPushButton *btnApplicate;     //申请
    QPushButton *btnApplicateList; //申请列表
    QGridLayout *ltySampleBlank;

    //总布局
    QGridLayout *layout;

};
extern SampleApplication *gSampleApplication;
#endif // SAMPLEAPPLICATION_H
