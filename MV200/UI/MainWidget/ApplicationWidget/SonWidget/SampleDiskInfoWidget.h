#ifndef SAMPLEDISKINFOWIDGET_H
#define SAMPLEDISKINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QIntValidator>
#include<QSignalMapper>

#include "ParamDefine.h"
#include "Parameter.h"

class SampleDiskInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SampleDiskInfoWidget(QWidget *parent = 0);
    void setSampleNoStart(int no);   //重置样本编号

private:
    void initForm();              //初始化
    void initSampleDiskForm();    //初始化样本盘
    void initData();              //初始化数据
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void ClearAll();                //清空所有

    //生成系统条码(样本条码)
    QString DefaultSampleCode(int no);
    int SampleNoStart;        //自动生成的样本条码起始编号
    int SampleNoStart2;       //自动生成的样本条码起始编号(重置用)

    QSignalMapper *sigMapper;       //信号管理
    QMap<int, QString> SampleNoMap; //样本编号映射表

signals:
    void sig_SelectedSampleList(QString s, QMap<int, QString> NoMap); //信号：已经选择的样本位

public slots:
    void slt_btnSure_Clicked();       //确定
    void slt_btnCancel_Clicked();     //取消
    void slt_btnAppend_Clicked();     //追加
    void slt_btnReset_Clicked();      //重置
    void slt_btnBack_Clicked();       //返回
    void slt_editBatch1_Change(QString text);     //槽：批量申请
    void slt_editBatch2_Change(QString text);     //槽：批量申请
    void slt_editBatch3_Change(QString text);     //槽：批量申请
    void slt_sigMapReceive(const int);  //槽：选中样本自动生成样本编号

private:
    QWidget *wdgSample;
    QGridLayout *lytSample;

    QWidget *wdgSample0;
    QGridLayout *lytSample0;
    QLabel *labSample0;
    QRadioButton *btnSample0[2];  //急诊样本架
    QLineEdit *editSample0[2];    //急诊样本架-条码

    QWidget *wdgSample1;
    QGridLayout *lytSample1;
    QLabel *labSample1;
    QRadioButton *btnSample1[SAMPLESLOT_COUNT];  //第一排样本架
    QLineEdit *editSample1[SAMPLESLOT_COUNT];    //第一排样本-条码
    QLineEdit *editBatch1;    //批量

    QWidget *wdgSample2;
    QGridLayout *lytSample2;
    QLabel *labSample2;
    QRadioButton *btnSample2[SAMPLESLOT_COUNT];  //第二排样本架
    QLineEdit *editSample2[SAMPLESLOT_COUNT];    //第二排样本-条码
    QLineEdit *editBatch2;    //批量

    QWidget *wdgSample3;
    QGridLayout *lytSample3;
    QLabel *labSample3;
    QRadioButton *btnSample3[SAMPLESLOT_COUNT];  //第三排样本架
    QLineEdit *editSample3[SAMPLESLOT_COUNT];    //第三排样本-条码
    QLineEdit *editBatch3;    //批量

    QWidget *wdgButton;
    QVBoxLayout *lytvButton;
    QPushButton *btnAppend;     //追加
    QPushButton *btnSure;       //确定
    QPushButton *btnReset;      //重置
    QPushButton *btnCancel;     //取消
    QPushButton *btnBack;       //返回

    //总布局
    QGridLayout *layout;
};
extern SampleDiskInfoWidget *gSampleApplicationSampleDiskInfo; //样本申请-样本盘信息
extern SampleDiskInfoWidget *gQCApplicationSampleDiskInfo;     //质控申请-样本盘信息
#endif // SAMPLEDISKINFOWIDGET_H
