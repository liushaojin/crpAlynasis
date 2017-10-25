#ifndef QUERYTERMSWIDGET_H
#define QUERYTERMSWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QFile>

#include "Object/SQLite/sqlite.h"

class QueryTermsWidget : public QDialog
{
    Q_OBJECT
public:
    explicit QueryTermsWidget(QDialog *parent = 0);

private:
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据

signals:
    void sig_QueryTerms_Change(QString terms); //信号：改变查询条件

public slots:
    void slt_btnSure_Clicked();         //槽:确定
    void slt_btnCancel_Clicked();       //槽:取消

private:
    QLabel *labStatus;            //状态
    QComboBox *cboStauts;
    QCheckBox *chk_isDateTime;    //是否按申请日期查询
    QDateEdit *dateDateTime;
    QLabel *labProject;           //项目
    QComboBox *cboProject;
    QLabel *labEmergency;         //是否急诊
    QRadioButton *rdoEmergency_n; //
    QRadioButton *rdoEmergency_y; //
    QLabel *labSampleType;        //样本类型
    QComboBox *cboSampleType;

    QPushButton *btnSure;         //确定
    QPushButton *btnCancel;       //取消
    QGridLayout *layout;


};

#endif // QUERYTERMSWIDGET_H
