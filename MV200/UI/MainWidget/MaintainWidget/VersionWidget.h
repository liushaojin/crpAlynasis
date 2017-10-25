#ifndef VERSIONWIDGET_H
#define VERSIONWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QGridLayout>
#include <QIcon>

class VersionWidget : public QDialog
{
    Q_OBJECT
public:
    explicit VersionWidget(QDialog *parent = 0);

private:
    void initForm();          //初始化
    void initStyle();         //界面风格

    QLabel *labName0;            //软件名称
    QLabel *labName;
    QLabel *labSpecifications0;  //型号规格
    QLabel *labSpecifications;
    QLabel *labVersion0;         //版本
    QLabel *labVersion;
    QLabel *labCVersion0;         //完整版本
    QLabel *labCVersion;
    QGridLayout *layout;
};

#endif // VERSIONWIDGET_H
