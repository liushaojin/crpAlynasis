#ifndef RESULTQUERYWIDGET_H
#define RESULTQUERYWIDGET_H


#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>

#include "QCWidget.h"
#include "SampleWidget.h"
//#include "ReagentBlankWidget.h"

class ResultQueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultQueryWidget(QWidget *parent = 0);
    static ResultQueryWidget *getInstance();          //实例对象


private:
    static ResultQueryWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据


signals:

public slots:
    void slt_tabCurrentChanged(int index); //槽：切换界面

private:
    QTabWidget *tabResultQuery;
    QGridLayout *layout;

    SampleWidget *gSampleWidget;
    QCWidget *qcWidget;

};

#endif // RESULTQUERYWIDGET_H
