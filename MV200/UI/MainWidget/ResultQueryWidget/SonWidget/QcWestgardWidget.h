#ifndef QCWESTGARDWIDGET_H
#define QCWESTGARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>

#include "UI/Widget/QCustomPlot/qcustomplot.h"
//#include "Garameter.h"

class QcWestgardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QcWestgardWidget(QWidget *parent = 0);
    ~QcWestgardWidget();

private:
    void initForm();                           //初始化界面控件
    void initStyle();                          //初始化样式
    void initConnect();                        //连接信号与槽
    void initData();                           //初始化数据

    void paintSDLine(double target, int sd); //绘画（7条SD直线）


signals:

public slots:
    void slt_UpdateGraph_Data(QVector<double> data);  //槽：更新Westgard多规则质控图
    void slt_UpdateGraph_Data(QVector<double> data, double target, double SD);  //槽：更新Westgard多规则质控图
    void slt_UpdateGraph_Data(QVector<double> data, int lower, int upper, int SD);  //槽：更新Westgard多规则质控图

private:
    QVBoxLayout *hlytSD;
    QLabel *lab4SD,*lab3SD,*lab2SD,*lab1SD,*labSD,*lab1SD1,*lab2SD1,*lab3SD1,*lab4SD1;
    QLabel *labTimes;

    QCustomPlot *customPlot;
    QCPGraph *graphResult;
    QCPGraph *graph_0SD;
    QCPGraph *graph_1SD0, *graph_1SD1;
    QCPGraph *graph_2SD0, *graph_2SD1;
    QCPGraph *graph_3SD0, *graph_3SD1;

    QGridLayout *layout;
};

#endif // QCWESTGARDWIDGET_H
