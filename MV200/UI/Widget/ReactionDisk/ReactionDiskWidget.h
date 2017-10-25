#ifndef REACTIONDISKWIDGET_H
#define REACTIONDISKWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>

#include "ReactionGraphicsItem.h"
#include "ParamDefine.h"
#include <QMutex>

#define STATUSCOUNT 7

class ReactionDiskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReactionDiskWidget(QWidget *parent = 0);
    //void setCupStatus(int cup,int status); //设置反应杯的状态
    void setCupStatus(int cup, QString objName); //设置反应杯的状态


private:
    void initReactionDisk(int num);
    void initStatusInfoForm();
    void initStyle();
    void reFlash(int unFlashID);

    int isFirstTimeClean;  //是否开机快速清洗标志（即开启软件第一次清洗，58， 59， 60号反应杯的状态不变，还是为“待清洗”）
    QMutex mutex;

signals:
    void sig_CurrentText(QString text); //信号：当前选中item的文本

public slots:
    void slt_SelectCup(int id);

private:
    QGridLayout *layout;
    QGraphicsView *view;
    QGraphicsScene *m_scene;
    ReactionDiskGraphicsItem *item[REACTION_CUP_COUNT];

    //状态说明
    //待清洗、空闲、清洗中、脏杯、稀释、R1、Sample、R2、End1、End2
    QWidget *wdgStatusInfo;
    QGridLayout *lytStatusInfo;
    QLabel *labStatusName[STATUSCOUNT];      //状态名称
    QLabel *labStatusPic[STATUSCOUNT];       //状态图标

};

#endif // REACTIONDISKWIDGET_H
