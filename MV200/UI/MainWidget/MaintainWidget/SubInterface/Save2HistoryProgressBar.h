#ifndef SAVE2HISTORYPROGRESSBAR_H
#define SAVE2HISTORYPROGRESSBAR_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QProgressBar>
#include "Parameter.h"
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"
#include "UI/MainWidget/ApplicationWidget/SonWidget/SampleDiskInfoWidget.h"

class Save2HistoryProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit Save2HistoryProgressBar(QWidget *parent = 0);
    void StartSave2HistoryT(); //开始转移到历史结果表中

private:
    void initForm();           //初始化
    void initStyle();          //界面风格

signals:

public slots:

private:
    QWidget *wdgSaving;             //
    QGridLayout *lytSaving;         //
    QLabel *labSavingStatus;        //
    QProgressBar *barSavingStatus; //
};

#endif // SAVE2HISTORYPROGRESSBAR_H
