#ifndef SAMPLEDISKWIDGET_H
#define SAMPLEDISKWIDGET_H

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

#include "ParamDefine.h"

class SampleDiskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SampleDiskWidget(QWidget *parent = 0);

private:
    void initForm();              //初始化
    void initSampleDiskForm();    //初始化样本盘
    void initData();              //初始化数据
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽

    void setSampleStatus(int flag); //设置样本状态

signals:
    void sig_SelectedSampleList(QString s); //信号：已经选择的样本位

public slots:
    void slt_btnSure_Clicked();       //确定
    void slt_btnCancel_Clicked();     //取消
    void slt_btnAppend_Clicked();     //追加
    void slt_btnReset_Clicked();      //重置
    void slt_editBatch1_Change(QString text);     //槽：批量申请
    void slt_editBatch2_Change(QString text);     //槽：批量申请
    void slt_editBatch3_Change(QString text);     //槽：批量申请

private:
    QWidget *wdgSample;
    QGridLayout *lytSample;

    QWidget *wdgSample0;
    QGridLayout *lytSample0;
    QLabel *labSample0;
    QRadioButton *btnSample0[2];      //急诊样本架

    QWidget *wdgSample1;
    QVBoxLayout *lytvSample1;
    QHBoxLayout *lythSample1;
    QLabel *labSample1;
    QRadioButton *btnSample1[SAMPLESLOT_COUNT];  //第一排样本架
    QLineEdit *editBatch1;    //批量

    QWidget *wdgSample2;
    QVBoxLayout *lytvSample2;
    QHBoxLayout *lythSample2;
    QLabel *labSample2;
    QRadioButton *btnSample2[SAMPLESLOT_COUNT];  //第二排样本架
    QLineEdit *editBatch2;    //批量

    QWidget *wdgSample3;
    QVBoxLayout *lytvSample3;
    QHBoxLayout *lythSample3;
    QLabel *labSample3;
    QRadioButton *btnSample3[SAMPLESLOT_COUNT];  //第三排样本架
    QLineEdit *editBatch3;    //批量

    QWidget *wdgButton;
    QVBoxLayout *lytvButton;
    QPushButton *btnAppend;     //追加
    QPushButton *btnSure;       //确定
    QPushButton *btnReset;      //重置
    QPushButton * btnCancel;    //取消

    //总布局
    QGridLayout *layout;
};

#endif // SAMPLEDISKWIDGET_H
