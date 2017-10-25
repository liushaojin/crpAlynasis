#ifndef SAMPLEWIDGET_H
#define SAMPLEWIDGET_H

#include <QWidget>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"
#include "../CurveWidget/resultCurveWidget.h"
//#include "Thread/PrinterThread.h"
#include "UI/Widget/MessageBox/messageBox.h"
#include "Object/Logic5P/MV_Logic5P.h"

#include "SonWidget/modifyResultWidget.h"
#include "Thread/Scheduler.h"
#include "UI/MainWidget/ApplicationWidget/SampleApplication.h"
#include "Thread/LISCommunicateThread.h"

class SampleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SampleWidget(QWidget *parent = 0);

private:
    void initForm();                    //初始化
    void initQueryTermsForm();          //查询条件
    void initSampleCode();              //样本编号
    void initResultList();              //测试结果查询
    void initData();                    //初始化数据
    void initTableFormat();             //格式化表格（样本编号表、结果表）
    void initStyle();                   //界面风格
    void initConnect();                 //连接信号与槽
    int QuerySampleLisData();//查询样本编号数据
    int QueryData();                      //查询数据
    QString ReCalculate(int projectID, int h_id);//重新计算幅度和结果

    int CurrentResultT_Count;        //（结果列表）查询行数标志
    QString QueryTermsResultT;       //（结果列表）查询条件
    QString QueryTermsApplicateTime; //（结果列表）查询条件-申请时间
    QString QueryTermsTestTime;      //（结果列表）查询条件-测试时间
    int CurrentSampleCode_Count;     //（样本列表）结果列表）查询行数标志
    QString QueryTermsSampleCode;    //（样本列表）查询条件

    int isAllNull_Flag;              //全选/反选标志
    QString CurrentSelectSampleCode; //当前选中样本的条码

    int isAlter_flag;   //是否修改标志

signals:
    void sig_UpdateApplicatedCount(); //信号：实时更新数据库中状态为“等待”的测试的个数

public slots:
    void slt_btnQuery_Clicked();            //槽：查询
    void slt_btnSampleCodeUpdate_Clicked(); //槽：修改样本编号
    void slt_btnSampleCodeFormer_Clicked(); //槽：上一页
    void slt_btnSampleCodeNext_Clicked();   //槽：下一页
    void slt_btnAllNull_Clicked();    //槽：全选/反选
    void slt_btnCurve_Clicked();      //槽：反应曲线
    void slt_btnFormer_Clicked();     //槽：上一页
    void slt_btnNext_Clicked();       //槽：下一页
    void slt_btnReTest_Clicked();     //槽：重测
    void slt_btnPrint_Clicked();      //槽：打印
    void slt_btnReCalculate_Clicked();//槽：重算
    void slt_btnDelete_Clicked();     //槽：删除
    void slt_btnLIS_Clicked();        //槽：LIS
    void slt_btnAlter_Clicked();      //槽：修改反应幅度（隐藏）
    void slt_ModifyResult();          //槽：修改返回的结果
    void slt_tbvSampleListSelectRow(const QModelIndex &index); //槽：（样本列表）单击列表选择中某行，列表对应行左边复选框对应变化
    void slt_tbvListSelectRow(const QModelIndex &index);       //槽：（结果列表）击列表选择中某行，列表对应行左边复选框对应变化
    void slt_AutoUpdateSampleList();  //槽：自动更新样本列表

private:
    ModifyResultWidget * modifyWidget; //结果修改

    //样本列表
    QWidget *wdgSampleCode;
    QGridLayout *lytSampleCode;
    QTableView *tbvSampleList;            //样本列表
    QStandardItemModel *modelSampleList;  //样本列表模型
    QPushButton *btnSampleCodeUpdate;     //修改样本条码
    QPushButton *btnSampleCodeNext;       //上一页
    QPushButton *btnSampleCodeFormer;     //下一页

    //结果列表
    QWidget *wdgResultList;
    QGridLayout *lytResultList;
    QTableView *tbvList;            //结果列表
    QStandardItemModel *modelList;  //结果列表模型
    QWidget *wdgMenuButton;
    QPushButton *btnAllNull;        //全选/反选
    QPushButton *btnFormer;         //上一页
    QPushButton *btnNext;           //下一页
    QPushButton *btnReTest;         //重测
    QPushButton *btnReCalculate;    //重算
    QPushButton *btnPrint;          //打印
    QPushButton *btnCurve;          //反应曲线
    QPushButton *btnDelete;         //删除
    QPushButton *btnLIS;            //LIS
    QPushButton *btnAlter;          //修改反应幅度（隐藏）
    QGridLayout *lytMenuButton;

    //查询条件
    QLabel *labStatus;            //状态
    QComboBox *cboStauts;
    QCheckBox *chk_isDateTime;    //是否按申请日期查询
    QDateEdit *dateDateTime;
    QCheckBox *chk_isTestTime;    //是否按测试日期查询
    QDateEdit *dateTestTime;
    QLabel *labProject;           //项目
    QComboBox *cboProject;
    QLabel *labSampleType;        //样本类型
    QComboBox *cboSampleType;
    QPushButton *btnQuery;        //查询
    QCheckBox *chkSampleCode;     //样本编号（条码）
    QLineEdit *editSampleCode;
    QGroupBox *grbQueryTerms;
    QGridLayout *lytQueryTerms;

    //总布局
    QGridLayout *layout;
};

#endif // SAMPLEWIDGET_H
