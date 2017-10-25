#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "SubInterface/HistoryResultWidget.h"
//#include "SubInterface/WorkloadAccountWidget.h"

class ReportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReportWidget(QWidget *parent = 0);
    static ReportWidget *getInstance();          //实例对象


private:
    static ReportWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据


signals:

public slots:

private:
    QGridLayout *layout;

};

#endif // REPORTWIDGET_H
