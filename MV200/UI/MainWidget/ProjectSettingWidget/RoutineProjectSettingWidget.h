#ifndef ROUTINEPROJECTSETTINGWIDGET_H
#define ROUTINEPROJECTSETTINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <qcombobox.h>
#include <QCheckBox>
#include <QTableWidget>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "ParamDefine.h"
#include "Parameter.h"
#include "Object/SQLite/sqlite.h"
#include "CardWidget/ReadCareWidget.h"

class RoutineProjectSettingWidget : public QWidget
{
    Q_OBJECT
public:
    static RoutineProjectSettingWidget *getInstance();
    explicit RoutineProjectSettingWidget(QWidget *parent = 0);

private:
    static RoutineProjectSettingWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initBasicParamForm();    //基本参数
    void initQCParamForm();       //质控参数
    void initButtonForm();        //功能按钮
    void initTbvRoutineProject(); //常规项目列表

    void initStyle();                //界面风格
    void initConnect();              //连接信号与槽
    void initData();                 //初始化数据
    void initDataTbvRoutineProject();//初始化数据-常规项目列表
    void initDataComboBox();         //初始化下拉列表
    void CleanAll();                     //清空所有输入
    void selectCurrentProject(int p_id); //查看当前选中的项目

    ProjectParamS InsertProjectParamS();       //新增项目时更新GParameter参数类中的映射表
    ProjectParamS UpdateProjectParamS(int id); //修改项目时更新GParameter参数类中的映射表

    bool isAdd_flag;              //是否增加标志
    bool isUpdate_flag;           //是否修改标志

signals:

public slots:
    void slt_chkCalibrateParam_2_Check(int s);  //槽:是否启用定标参数2
    void slt_btnAdd_Clicked();      //槽：增加
    void slt_btnUpdate_Clicked();   //槽：修改
    void slt_btnDelete_Clicked();   //槽：删除
    void slt_btnCancel_Clicked();   //槽：取消
    void slt_btnSave_Clicked();     //槽：保存
    void slt_btnReadCard_Clicked(); //槽：读卡
    void slt_btnSaveParam_K_Clicked();  //槽：保存参数（K因数值）
    void slt_tbvSelectRow(const QModelIndex &index);   //单击列表选择中某行
    void slt_updateProject(QString name);              //读写卡成功后更新数据
    void slt_cboIsDiluent_IndexChanged(int index); //槽:选择是否要稀释时，稀释液&转移量会有变化

private:
    //常规项目
    QTableView *tbvRoutineProject;
    QStandardItemModel *modelRoutineProject;
    ReadCareWidget *readCareWidget;

    //参数设置
    QTabWidget *tabParam;

    //基本参数
    QWidget *wdgBasicParam;
    QGridLayout *lytBasicParam;
    QLabel *labProjectName0;     //项目简称
    QLineEdit *editProjectName0;
    QLabel *labProjectName;      //项目名称
    QLineEdit *editProjectName;
    QLabel *labBatchNum;         //批号
    QLineEdit *editBatchNum;
    QLabel *labAccuracy;         //结果精度
    QComboBox *cboAccuracy;
    QLabel *labAccuracy2;        //结果精度小数几位
    QLabel *labUnit;             //结果单位
    QComboBox *cboUnit;
    QLineEdit *editUnit;
    QLabel *labIsDiluent;        //是否要稀释
    QComboBox *cboIsDiluent;

    //加样量设置(ul)
    QGroupBox *grbAddingAmount;
    QGridLayout *lytAddingAmount;
    QLabel *labDiluent;          //稀释液
    QLineEdit *editDiluent;
    QLabel *labSample;           //样本
    QLineEdit *editSample;
    QLabel *labS_Transfer;       //样本转移量
    QLineEdit *editS_Transfer;
    QLabel *labR1;               //R1
    QLineEdit *editR1;
    QLabel *labR2;               //R2
    QLineEdit *editR2;

    QLabel *labReactionCycle;    //反应周期/时间(s)
    QLineEdit *editCycle1;
    QLineEdit *editTime1;
    QLineEdit *editCycle2;
    QLineEdit *editTime2;
    QLabel *labParam_K;          //K因数值
    QLineEdit *editParam_K;
    QLabel *labAllowanceWarm;    //余量报警(次)
    QLineEdit *editAllowanceWarm;
    QLabel *labBloodFactor;      //全血因子
    QLineEdit *editBloodFactor;
    QLabel *labLowLinear;        //线性下限
    QLineEdit *editLowLinear;

    //项目定标参数1
    QCheckBox *chkCalibrateParam_1;  //参数是否启用
    QLabel *lab_R0_1;
    QLineEdit *edit_R0_1;
    QLabel *lab_K_1;
    QLineEdit *edit_K_1;
    QLabel *lab_a_1;
    QLineEdit *edit_a_1;
    QLabel *lab_b_1;
    QLineEdit *edit_b_1;
    QLabel *lab_c_1;
    QLineEdit *edit_c_1;
    QLabel *labCalibrateParam_R_1;     //第一段曲线反应幅度分段点
    QLineEdit *editCalibrateParam_R_1;
    QLabel *labCalibrateParam_C_1;     //第一段曲线浓度分段点
    QLineEdit *editCalibrateParam_C_1;
    QGroupBox *grbCalibrateParam_1;
    QGridLayout *lytCalibrateParam_1;

    //项目定标参数2
    QCheckBox *chkCalibrateParam_2;  //参数是否启用
    QLabel *lab_R0_2;
    QLineEdit *edit_R0_2;
    QLabel *lab_K_2;
    QLineEdit *edit_K_2;
    QLabel *lab_a_2;
    QLineEdit *edit_a_2;
    QLabel *lab_b_2;
    QLineEdit *edit_b_2;
    QLabel *lab_c_2;
    QLineEdit *edit_c_2;
    QLabel *labCalibrateParam_R_2;     //第二段曲线反应幅度分段点
    QLineEdit *editCalibrateParam_R_2;
    QLabel *labCalibrateParam_C_2;     //第二段曲线浓度分段点
    QLineEdit *editCalibrateParam_C_2;
    QGroupBox *grbCalibrateParam_2;
    QGridLayout *lytCalibrateParam_2;

    //质控参数

    //功能按钮
    QWidget *wdgButton;
    QGridLayout *lytButton;
    QPushButton *btnReadCard;
    QPushButton *btnDelete;
    QPushButton *btnSaveParam_K;  //保存参数（K因数值）
    QPushButton *btnAdd;
    QPushButton *btnUpdate;
    QPushButton *btnCancel;
    QPushButton *btnSave;


    //总布局
    QGridLayout *layout;

};

#endif // ROUTINEPROJECTSETTINGWIDGET_H











