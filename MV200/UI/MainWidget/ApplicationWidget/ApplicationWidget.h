#ifndef APPLICATIONWIDGET_H
#define APPLICATIONWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>

#include "SampleApplication.h"
#include "QCApplication.h"

class ApplicationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationWidget(QWidget *parent = 0);
    static ApplicationWidget *getInstance();          //实例对象


private:
    static ApplicationWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据


signals:

public slots:

private:
    QTabWidget *tabApplication;
    QGridLayout *layout;

};

#endif // APPLICATIONWIDGET_H
