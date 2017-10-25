#ifndef QCAPPLICATION_H
#define QCAPPLICATION_H

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
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "Thread/Scheduler.h"
#include "Thread/ApplicateThread.h"
#include "ParamDefine.h"
#include "SonWidget/SampleDiskInfoWidget.h"
#include "UI/Widget/QFlowLayout/QFlowLayout.h"
#include "UI/MainWidget/ProjectSettingWidget/QCSettingWidget.h"


class QCApplication : public QWidget
{
    Q_OBJECT
public:
    explicit QCApplication(QWidget *parent = 0);

private:
    void initForm();               //初始化
    void initQCInfoForm();         //申请测试的样本的信息
    void initProjectListForm();    //项目列表
    void initQCTableForm();        //初始化质控申请列表
    void initStyle();              //界面风格
    void initConnect();            //连接信号与槽
    void initData();               //初始化数据
    //根据项目名称长度来确定空格长度
    QString FillSpace(int count);

    QMap<int, QString> SampleNoMap; //样本编号映射表
    int Count;
    bool isAllNull_Flag; //全选/反选标志

signals:

public slots:
    //槽：更新质控液下拉列表
    void slt_cboQCLiquid_UpdateItems();
    //槽：更新质控液下拉列表
    void slt_cboQCLiquid_Update();

    void slt_btnSamplePos_Clicked();   //槽：样本位选择
    void slt_btnApplicate_Clicked();   //槽：申请
    void slt_btnQCSetting_Clicked();   //槽：质控设置
    //槽：下拉列表索引改变
    void slt_cboQCLiquid_IndexChanged(int);

    void slt_btnAllNone_Clicked();  //槽：全选/反选
    void slt_btnFormer_Clicked();   //槽：上一页
    void slt_btnNext_Clicked();     //槽：下一页
    void slt_btnDelete_Clicked();   //槽：删除
    //槽：已经选择的样本位
    void slt_SelectedSampleList(QString,QMap<int,QString>);

    //槽：更新已申请的质控列表
    int slt_tbvQCList_Update();

private:
    //申请测试的样本的信息
    QWidget *wdgSampleInfo;
    QGridLayout *lytSampleInfo;
    QLabel *labSamplePos;        //样本位
    QLineEdit *editSamplePos;
    QPushButton *btnSamplePos;
    QLabel *labNo;               //编号
    QLineEdit *editNo;
    QLabel *labQCLiquid;         //质控液
    QComboBox *cboQCLiquid;
    QPushButton *btnApplicate;   //申请
    QPushButton *btnQCSetting;   //质控设置

    //项目列表
    QWidget *wdgProjectList;
    QGroupBox *grbProjectList;
    QRadioButton *rdoRoutineItem[PROJECT_COUNT];     //常规项目
    QFlowLayout *lytProjectList;     //流布局

    //质控申请列表
    QWidget *wdgQCList;
    QTableView *tbvQCList;
    QStandardItemModel *modelQCList;
    QPushButton *btnAllNone;
    QPushButton *btnFormer;
    QPushButton *btnNext;
    QPushButton *btnDelete;
    QGridLayout *lytQCList;

    //总布局
    QGridLayout *layout;

};

#endif // QCAPPLICATION_H
