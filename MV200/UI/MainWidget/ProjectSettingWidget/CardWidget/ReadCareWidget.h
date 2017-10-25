#ifndef READCAREWIDGET_H
#define READCAREWIDGET_H

#include <QWidget>
#include <QDialog>
#include <qlabel.h>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <qgridlayout.h>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QFile>
#include <QTimer>

#include "Thread/Scheduler.h"
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"

//////////////////////////////////////////////////////////////////////
#define CARD_TYPE "MV200"
#define CONNECTOR "*"     //连接符
#define CARD_END  "###"

class ReadCareWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ReadCareWidget(QDialog *parent = 0);
    void StartReadCard();         //开始读卡

private:
    void initForm();              //初始化
    void initReadingForm();       //初始化-准备读卡
    void initCardDataForm();      //初始化-读卡数据
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据
    void initDataComboBox();      //初始化下拉列表
    void CoverOldProject(int id); //覆盖原来的项目(即更新项目参数)
    void AddNewProject();         //新增项目
    QString FormateData(QString data);//数据格式化
    void setCardData(QString data);//绑定数据
    ProjectParamS InsertProjectParamS();       //新增项目时更新GParameter参数类中的映射表
    ProjectParamS UpdateProjectParamS(int id); //修改项目时更新GParameter参数类中的映射表
    QTimer *ReadCardDelayTimer;

signals:
    void sig_updateProject(QString name);

public slots:
    void slt_ReadCard_Error(int state);             //槽:读卡出现错误（返回状态）
    void slt_ReadCard_Data(QString data);           //槽:读卡数据
    void slt_ProgressBar_ValueChanged(int i);       //槽:更新读卡进度条
    void slt_btnSave_Clicked();                     //槽:保存数据
    void slt_btnCancel_Clicked();                   //槽:取消
    void slt_chkCalibrateParam_2_Check(int status); //槽:是否启用定标参数2
    void slt_cboIsDiluent_IndexChanged(int index);  //槽:选择是否要稀释时，稀释液&转移量会有变化


private:
    //准备读卡
    QWidget *wdgReading;
    QGridLayout *lytReading;
    QLabel *labReadingStatus;        //读卡状态
    QProgressBar * barReadingStatus; //读卡进度条

    //读卡数据
    QWidget *wdgCardData;
    QGridLayout *lytCardData;
    //基本参数
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

    QPushButton *btnSave;
    QPushButton *btnCancel;

    //总布局
    QGridLayout *layout;

};
#endif // READCAREWIDGET_H
