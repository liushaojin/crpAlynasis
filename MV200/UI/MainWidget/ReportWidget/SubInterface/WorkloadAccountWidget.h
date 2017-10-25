#ifndef WORKLOADACCOUNTWIDGET_H
#define WORKLOADACCOUNTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>

#include "Parameter.h"
#include "Object/SQLite/sqlite.h"

class WorkloadAccountWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkloadAccountWidget(QWidget *parent = 0);

private:
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据

signals:

public slots:
    void slt_btnQuery_Clicked();       //槽：查询

private:
    //查询条件
    QGroupBox *grbQueryTerms;
    QRadioButton *rdoProject;    //按项目查询
    QComboBox *cboProject;
    QRadioButton *rdoSampleType; //按样本查询
    QComboBox *cboSampleType;
    QRadioButton *rdoDate;       //按日期查询
    QDateEdit *dateQuery;
    QPushButton *btnQuery;       //查询
    QGridLayout *lytQueryTerms;

    //查询结果
    QLabel *labResult0;   //结果
    QLabel *labResult;

    //总布局
    QGridLayout *layout;

};

#endif // WORKLOADACCOUNTWIDGET_H
