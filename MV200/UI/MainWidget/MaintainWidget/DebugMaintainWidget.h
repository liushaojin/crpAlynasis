#ifndef DEBUGMAINTAINWIDGET_H
#define DEBUGMAINTAINWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include "SubInterface/LightDebugWidget.h"
#include "SubInterface/UnitMaintain.h"

class DebugMaintainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DebugMaintainWidget(QWidget *parent = 0);

private:
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据

signals:

public slots:

private:
    QTabWidget *tabMaintain;
    QGridLayout *layout;
};

#endif // DEBUGMAINTAINWIDGET_H
