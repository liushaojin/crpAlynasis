#ifndef WORKSTATUSWIDGET_H
#define WORKSTATUSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>

#include "sampleInfoWidget.h"
#include "reagentInfoWidget.h"
#include "testListInfoWidget.h"
#include "reactionDiskInfoWidget.h"

class WorkStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkStatusWidget(QWidget *parent = 0);
    static WorkStatusWidget *getInstance();          //实例对象


private:
    static WorkStatusWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据


signals:

public slots:

private:
    QTabWidget *tabWorkStatus;
    QGridLayout *layout;

};

#endif // WORKSTATUSWIDGET_H
