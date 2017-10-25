#include "QcWestgardWidget.h"

QcWestgardWidget::QcWestgardWidget(QWidget *parent) : QWidget(parent)
{
    initForm();                           //初始化界面控件
    initStyle();                          //初始化样式
    initConnect();                        //连接信号与槽
    initData();                           //初始化数据
}


QcWestgardWidget::~QcWestgardWidget()
{

}

//初始化数据
void QcWestgardWidget::initData()
{

}

//初始化界面控件
void QcWestgardWidget::initForm()
{
    hlytSD = new QVBoxLayout;
    hlytSD->setMargin(0);
    hlytSD->setSpacing(5);
    lab4SD = new QLabel("4SD");
    lab3SD = new QLabel("3SD");
    lab2SD = new QLabel("2SD");
    lab1SD = new QLabel("1SD");
    labSD = new QLabel("X");
    lab1SD1 = new QLabel("-1SD");
    lab2SD1 = new QLabel("-2SD");
    lab3SD1 = new QLabel("-3SD");
    lab4SD1 = new QLabel("-4SD");
    hlytSD->addWidget(lab4SD);
    hlytSD->addWidget(lab3SD);
    hlytSD->addWidget(lab2SD);
    hlytSD->addWidget(lab1SD);
    hlytSD->addWidget(labSD);
    hlytSD->addWidget(lab1SD1);
    hlytSD->addWidget(lab2SD1);
    hlytSD->addWidget(lab3SD1);
    hlytSD->addWidget(lab4SD1);

    labTimes = new QLabel("Times");
    customPlot = new QCustomPlot;

    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    //layout->addWidget(labSD,       0,  0, 100,   3, Qt::AlignCenter);
    layout->addLayout(hlytSD,      0,  0, 103,   3);
    layout->addWidget(customPlot,  2,  3, 100, 100);
    layout->addWidget(labTimes,  102,  0,   2, 100, Qt::AlignHCenter);
    setLayout(layout);
}

//初始化样式
void QcWestgardWidget::initStyle()
{
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 0, Qt::NoPen));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 0, Qt::NoPen));
    customPlot->xAxis->grid()->setSubGridVisible(false);
    customPlot->yAxis->grid()->setSubGridVisible(false);
    customPlot->yAxis->setVisible(false);
    customPlot->yAxis->setRange(-4, 4);
    customPlot->xAxis->setRange(1, 10);

    graph_0SD = customPlot->addGraph();
    graph_1SD0= customPlot->addGraph();
    graph_1SD1= customPlot->addGraph();
    graph_2SD0= customPlot->addGraph();
    graph_2SD1= customPlot->addGraph();
    graph_3SD0= customPlot->addGraph();
    graph_3SD1= customPlot->addGraph();
    graphResult = customPlot->addGraph();
    paintSDLine(0, 1);
}

//连接信号与槽
void QcWestgardWidget::initConnect()
{

}

//绘画（7条SD直线）
void QcWestgardWidget::paintSDLine(double target, int sd)
{
    QVector<double> vecX, vecY;
    vecX.append(1); vecX.append(2000);
    double xSD; //SD

    //X
    xSD = target;
    graph_0SD->clearData();
    graph_0SD->setPen(QPen(Qt::black, 1));
    vecY.append(xSD);vecY.append(xSD);
    graph_0SD->setData(vecX, vecY);

    //1SD
    xSD = target + sd;
    graph_1SD0->clearData();
    graph_1SD0->setPen(QPen(Qt::blue, 1));
    vecY.clear(); vecY.append(xSD); vecY.append(xSD);
    graph_1SD0->setData(vecX, vecY);

    //-1SD
    xSD = target - sd;
    graph_1SD1->clearData();
    graph_1SD1->setPen(QPen(Qt::blue, 1));
    vecY.clear(); vecY.append(xSD); vecY.append(xSD);
    graph_1SD1->setData(vecX, vecY);

    //2SD
    xSD = target + sd * 2;
    graph_2SD0->clearData();
    graph_2SD0->setPen(QPen(Qt::green, 1));
    vecY.clear(); vecY.append(xSD); vecY.append(xSD);
    graph_2SD0->setData(vecX, vecY);

    //-2SD
    xSD = target - sd * 2;
    graph_2SD1->clearData();
    graph_2SD1->setPen(QPen(Qt::green, 1));
    vecY.clear(); vecY.append(xSD); vecY.append(xSD);
    graph_2SD1->setData(vecX, vecY);

    //3SD
    xSD = target + sd * 3;
    graph_3SD0->clearData();
    graph_3SD0->setPen(QPen(Qt::red, 1));
    vecY.clear(); vecY.append(xSD); vecY.append(xSD);
    graph_3SD0->setData(vecX, vecY);

    //-3SD
    xSD = target - sd * 3;
    graph_3SD1->clearData();
    graph_3SD1->setPen(QPen(Qt::red, 1));
    vecY.clear(); vecY.append(xSD); vecY.append(xSD);
    graph_3SD1->setData(vecX, vecY);

    graphResult->clearData();
    graphResult->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QPen(Qt::darkCyan, 10), QBrush(Qt::black), 8));
    graphResult->setPen(QPen(Qt::darkGray, 1));
}

//槽：更新Westgard多规则质控图
void QcWestgardWidget::slt_UpdateGraph_Data(QVector<double> data)
{
    int range = 10;
    QVector<double> vecX;
    graphResult->clearData();
    if(data.count() > 0)
    {
        for(int i = 1; i <= data.count(); i ++)
        {
            vecX.append(i);
            if(i >= 10)
                range = i +3;
        }
        graphResult->setData(vecX, data);
    }

    customPlot->xAxis->setRange(1, range);
    customPlot->replot();  //实时更新曲线
}

//槽：更新Westgard多规则质控图
void QcWestgardWidget::slt_UpdateGraph_Data(QVector<double> data, int lower, int upper, int SD)
{
    int range = 10;
    QVector<double> vecX;
    graphResult->clearData();
    if(data.count() > 0)
    {
        for(int i = 1; i <= data.count(); i ++)
        {
            vecX.append(i);
            if(i >= 10)
                range = i +3;
        }
        graphResult->setData(vecX, data);
    }

    customPlot->yAxis->setRange(lower, upper);
    customPlot->xAxis->setRange(1, range);
    customPlot->replot();  //实时更新曲线
}

//槽：更新Westgard多规则质控图
void QcWestgardWidget::slt_UpdateGraph_Data(QVector<double> data, double target, double SD)
{
    int range = 10;
    QVector<double> vecX;
    graphResult->clearData();
    if(data.count() > 0)
    {
        for(int i = 1; i <= data.count(); i ++)
        {
            vecX.append(i);
            if(i >= 10)
                range = i +3;
        }
        graphResult->setData(vecX, data);
    }

    //qDebug() << "lower:" << target-SD*3 << " upper:" << target+3*SD;
    customPlot->xAxis->setRange(1, range);
    customPlot->replot();  //实时更新曲线
}









