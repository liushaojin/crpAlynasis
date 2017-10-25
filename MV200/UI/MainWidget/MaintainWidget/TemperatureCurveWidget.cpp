#include "TemperatureCurveWidget.h"

TemperatureCurveWidget::TemperatureCurveWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();        //初始化界面控件
    initStyle();       //初始化样式
    initConnect();     //连接信号与槽
    initData();        //初始化数据
}

//初始化界面控件
void TemperatureCurveWidget::initForm()
{
    //查询设置
    wdtSetting = new QWidget;
    chkReactionDisk = new QCheckBox("反应盘温度");     //反应盘
    labReactionDisk = new QLabel("间隔时间(s):");
    editReactionDisk = new QLineEdit;
    btnReactionDisk = new QPushButton("确 定");
    labReactionDiskAlarm = new QLabel;               //温度报警

    chkReagentSlot = new QCheckBox("试剂槽温度");      //试剂槽
    labReagentSlot = new QLabel("间隔时间(s):");
    editReagentSlot = new QLineEdit;
    btnReagentSlot = new QPushButton("确 定");
    labReagentSlotAlarm = new QLabel;                //温度报警
    lytSetting = new QGridLayout;
    lytSetting->setMargin(5);
    lytSetting->setSpacing(5);
    lytSetting->addWidget(chkReactionDisk,  0,  0, 2, 5, Qt::AlignCenter);
    lytSetting->addWidget(labReactionDisk,  0,  5, 2, 5, Qt::AlignRight);
    lytSetting->addWidget(editReactionDisk, 0, 10, 2, 5, Qt::AlignLeft);
    lytSetting->addWidget(btnReactionDisk,  0, 15, 2, 5, Qt::AlignCenter);
    lytSetting->addWidget(labReactionDiskAlarm,  0, 20, 2, 15, Qt::AlignLeft);

    lytSetting->addWidget(chkReagentSlot,  2,  0, 2, 5, Qt::AlignCenter);
    lytSetting->addWidget(labReagentSlot,  2,  5, 2, 5, Qt::AlignRight);
    lytSetting->addWidget(editReagentSlot, 2, 10, 2, 5, Qt::AlignLeft);
    lytSetting->addWidget(btnReagentSlot,  2, 15, 2, 5, Qt::AlignCenter);
    lytSetting->addWidget(labReagentSlotAlarm,  2, 20, 2, 15, Qt::AlignLeft);
    wdtSetting->setLayout(lytSetting);

    //温度曲线
    customPlot = new QCustomPlot;

    //总布局
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(3);
    layout->addWidget(wdtSetting,  0, 0,  5, 100, Qt::AlignLeft);
    layout->addWidget(customPlot,  5, 0, 35, 100);
    setLayout(layout);

    //定时器
    timerReactionDisk = new QTimer;
    timerReagentSlot = new QTimer;
}

//初始化样式
void TemperatureCurveWidget::initStyle()
{
    btnReactionDisk->setObjectName("button6");
    btnReagentSlot->setObjectName("button6");
    editReactionDisk->setAlignment(Qt::AlignCenter);
    editReagentSlot->setAlignment(Qt::AlignCenter);

    labReactionDisk->setStyleSheet("QLabel{font-size:18px; color:green;}");
    labReagentSlot->setStyleSheet("QLabel{font-size:18px; color:darkRed;}");
    chkReactionDisk->setStyleSheet("color:green;");
    chkReagentSlot->setStyleSheet("color:darkRed;");

    //设置背景
    this->setAutoFillBackground( true );
    QString bg_str = ":/resource/resource/bg/mainBg.jpg";
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap( bg_str )));
    this->setPalette(palette);

    //set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::black, 1));    //x轴的颜色及大小
    customPlot->yAxis->setBasePen(QPen(Qt::black, 1));    //Y轴的颜色及大小
    customPlot->xAxis->setTickPen(QPen(Qt::black, 1));    //x轴勾笔
    customPlot->yAxis->setTickPen(QPen(Qt::black, 1));    //y轴勾笔
    customPlot->xAxis->setSubTickPen(QPen(Qt::black, 1)); //x轴勾笔
    customPlot->yAxis->setSubTickPen(QPen(Qt::black, 1)); //y轴勾笔
    customPlot->xAxis->setTickLabelColor(Qt::black);      //x轴坐标
    customPlot->yAxis->setTickLabelColor(Qt::black);      //y轴坐标
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    //背景着色1
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(135, 237, 251));
    plotGradient.setColorAt(1, QColor( 73, 195, 232));
    customPlot->setBackground(plotGradient);
    //背景着色2
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(135, 237, 251));
    axisRectGradient.setColorAt(1, QColor(98, 175, 222));
    customPlot->axisRect()->setBackground(axisRectGradient);
}

//初始化数据
void TemperatureCurveWidget::initData()
{
    vecReactionDisk.clear();
    vecReagentSlot.clear();

    chkReactionDisk->setChecked(true);
    chkReagentSlot->setChecked(true);
    editReactionDisk->setText("5");
    editReagentSlot->setText("5");

    vecReactionDisk.clear(); //反应盘温度动态数组
    vecReagentSlot.clear();  //试剂槽温度动态数组

    //add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

    //反应盘温度曲线 customPlot->graph(2)
    graphResult_Disk = customPlot->addGraph();
    graphResult_Disk->clearData();
    graphResult_Disk->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare, QPen(Qt::green, 8), QBrush(Qt::cyan), 4));
    graphResult_Disk->setPen(QPen(QColor(120, 120, 120), 1.2));

    //试剂盘温度曲线 customPlot->graph(3)
    graphResult_Reagent = customPlot->addGraph();
    graphResult_Reagent->clearData();
    graphResult_Reagent->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, QPen(Qt::darkRed, 6), QBrush(Qt::red), 4));
    graphResult_Reagent->setPen(QPen(QColor(120, 120, 120), 1));

    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(TEMPERATURE_L, TEMPERATURE_H); //温度范围
    customPlot->xAxis->setRange(1, TEMPERATURE_COUNT);         //x轴个数范围
}

//连接信号与槽
void TemperatureCurveWidget::initConnect()
{
    connect(timerReactionDisk, SIGNAL(timeout()), this, SLOT(slt_timerReactionDisk_timeout()));
    connect(timerReagentSlot, SIGNAL(timeout()), this, SLOT(slt_timerReagentSlot_timeout()));

    connect(btnReactionDisk, SIGNAL(clicked()), this, SLOT(slt_btnReactionDisk_Clicked()));
    connect(btnReagentSlot, SIGNAL(clicked()), this, SLOT(slt_btnReagentSlot_Clicked()));

    connect(gSerialThread, SIGNAL(sig_currentReactionDisk_T(double)), this, SLOT(slt_appendNewReactionDist_T(double)));
    connect(gSerialThread, SIGNAL(sig_currentReagentSlot_T(double)), this, SLOT(slt_appendNewReagentSlot_T(double)));
}

//槽：追加反应盘温度到动态数据中
void TemperatureCurveWidget::slt_appendNewReactionDist_T(double t)
{
    if(vecReactionDisk.count() == TEMPERATURE_COUNT)
        vecReactionDisk.remove(0); //移除第一个点
    vecReactionDisk.append(t);

    if(t < TEMPERATURE_L)
        labReactionDisk->setText("提示：反应盘温度过低");
    else if(t > TEMPERATURE_H)
        labReactionDisk->setText("提示：反应盘温度过高");
    else
        labReactionDisk->setText("");

    updateReactionDiskCurve(); //更新反应盘温度曲线
}

//槽：追加试剂槽温度到动态数据中
void TemperatureCurveWidget::slt_appendNewReagentSlot_T(double t)
{
    if(vecReagentSlot.count() == TEMPERATURE_COUNT)
        vecReagentSlot.remove(0); //移除第一个点
    vecReagentSlot.append(t);

    if(t < TEMPERATURE_L)
        labReagentSlotAlarm->setText("提示：试剂槽温度过低");
    else if(t > TEMPERATURE_H)
        labReagentSlotAlarm->setText("提示：试剂槽温度过高");
    else
        labReagentSlotAlarm->setText("");

    updateReagentSlotCurve();  //更新反应盘温度曲线
}

//更新反应盘温度曲线
void TemperatureCurveWidget::updateReactionDiskCurve()
{
    QVector<double> x, y1;
    x.clear();
    y1.clear();
    QCPData newData;
    for(int i = 0; i < vecReactionDisk.count(); i ++)
    {
        newData.value = vecReactionDisk.at(i);
        x.append(i+1);
        y1.append(vecReactionDisk.at(i));
        customPlot->graph(2)->data()->insert(i, newData);
    }
    graphResult_Disk->clearData();
    graphResult_Disk->setData(x, y1);
    customPlot->replot();  //实时更新曲线
}

//更新反应盘温度曲线
void TemperatureCurveWidget::updateReagentSlotCurve()
{
    QVector<double> x, y1;
    x.clear();
    y1.clear();
    QCPData newData;
    for(int i = 0; i < vecReagentSlot.count(); i ++)
    {
        newData.value = vecReagentSlot.at(i);
        x.append(i+1);
        y1.append(vecReagentSlot.at(i));
        customPlot->graph(3)->data()->insert(i, newData);
    }
    graphResult_Reagent->clearData();
    graphResult_Reagent->setData(x, y1);
    customPlot->replot();  //实时更新曲线
}

//槽：反应盘温度
void TemperatureCurveWidget::slt_btnReactionDisk_Clicked()
{
    if(timerReactionDisk->isActive())
        timerReactionDisk->stop();

    int time = editReactionDisk->text().trimmed().toInt();
    if(time > 0)
        timerReactionDisk->start(time*1000);

    slt_timerReactionDisk_timeout();
}

//槽：试剂槽温度
void TemperatureCurveWidget::slt_btnReagentSlot_Clicked()
{
    if(timerReagentSlot->isActive())
        timerReagentSlot->stop();

    int time = editReagentSlot->text().trimmed().toInt();
    if(time > 0)
        timerReagentSlot->start(time*1000);

    slt_timerReagentSlot_timeout();
}

//槽：反应盘温度定时器
void TemperatureCurveWidget::slt_timerReactionDisk_timeout()
{
    gSerialThread->WriteDebugFrame("FA 0f 01 3F 49 01 00 FB");
}

//槽：试剂槽温度定时器
void TemperatureCurveWidget::slt_timerReagentSlot_timeout()
{
    gSerialThread->WriteDebugFrame("FA 0f 01 3F 49 02 00 FB");
}



