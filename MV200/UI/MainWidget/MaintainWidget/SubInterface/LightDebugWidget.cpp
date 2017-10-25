#include "LightDebugWidget.h"


LightDebugWidget *LightDebugWidget::_instance = 0;
LightDebugWidget::LightDebugWidget(QWidget *parent) :
    QWidget(parent)
{
    initForm();                           //初始化界面控件
    initStyle();                          //初始化样式
    initConnect();                        //连接信号与槽
    initLanguage();                       //初始化语言
    initData();                           //初始化数据
}


//实例对象
LightDebugWidget * LightDebugWidget::getInstance()
{
    if(_instance == 0)
        _instance = new LightDebugWidget;
    return _instance;
}

//语言设置
void LightDebugWidget::initLanguage()
{
    grbSettingInfo->setTitle("反应盘调试设置");         //
    btnReset->setText("复位");            //反应盘复位
    labStartCup->setText("开始采光杯位:"); //开始采光杯位
    btnStart->setText("开 始");           //开始采光
    labDelay->setText("延迟时间:");
}

//初始化界面控件
void LightDebugWidget::initForm()
{
    grbSettingInfo = new QGroupBox;  //
    btnReset = new QPushButton;      //反应盘复位

    labDelay = new QLabel;        //采集延迟时间
    editDelay = new QLineEdit;

    labStartCup = new QLabel;        //开始采光杯位
    editStartCup = new QLineEdit;
    btnStart = new QPushButton;      //开始采光
    lytSettingInfo = new QGridLayout;
    lytSettingInfo->setMargin(10);
    lytSettingInfo->setSpacing(10);
    //lytSettingInfo->addWidget(btnReset,        0,  0, 2, 5, Qt::AlignCenter);
    lytSettingInfo->addWidget(labStartCup,     0,  8, 2, 5, Qt::AlignCenter);
    lytSettingInfo->addWidget(editStartCup,    0, 13, 2, 5, Qt::AlignCenter);
    lytSettingInfo->addWidget(labDelay,        0, 18, 2, 5, Qt::AlignCenter);
    lytSettingInfo->addWidget(editDelay,       0, 23, 2, 5, Qt::AlignCenter);
    lytSettingInfo->addWidget(btnStart,        0, 28, 2, 5, Qt::AlignCenter);
    grbSettingInfo->setLayout(lytSettingInfo);

    customPlot = new QCustomPlot;
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(grbSettingInfo,    5,  0, 10, 100, Qt::AlignCenter);
    layout->addWidget(customPlot,          15, 0, 60, 100);
    this->setLayout(layout);
}

//初始化样式
void LightDebugWidget::initStyle()
{
//    //设置背景
    this->setAutoFillBackground( true );
    QString bg_str = ":/resource/resource/bg/mainBg.png";
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap( bg_str )));
    this->setPalette(palette);

    QString styleGrb = "QGroupBox{font-size:18px; border: 1px solid #45C4E7; background-color:rgb(79, 208, 237); background: transparent; border-radius:8px; margin-top: 20;}"
                       "QGroupBox::title{subcontrol-origin: margin; subcontrol-position: top left; background-color:rgb(79, 208, 237); border-radius:3px; }";

    QString styleBTN =
            "        QPushButton{width:90;height:50; border-radius:8px; font-size:18px; background-image:url(:/resource/resource/button/button6.png);background-repeat:no-repeat; background-position:center;}"
            "QPushButton:pressed{width:90;height:50; border-radius:8px; font-size:18px; background-image:url(:/resource/resource/button/button6_pressed.png);background-repeat:no-repeat; background-position:center; color:white;}";

    QString styleLab =  //标签模式
            "QLabel{font-size:18px;}";
    QString styleEDIT = //编辑框模式
            "QLineEdit{ width:150; height:35; font-size:22px; border: 1px solid rgb(41, 57, 85); border-radius:3px; background:white; selection-background-color:green;}"
            "QLineEdit:hover{border: 1px solid blue;}"
            "QLineEdit:!enabled{border: 1px solid rgba(10,10,10,10); background-color:rgba(10,10,10,10);}";

    this->setStyleSheet( styleGrb +  " " + styleBTN + " " + styleEDIT + " " + styleLab );

    editStartCup->setAlignment(Qt::AlignCenter);
    editDelay->setAlignment(Qt::AlignCenter);

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
void LightDebugWidget::initData()
{
    editStartCup->setText("10");
    editDelay->setText("1");

    //add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

    //create and configure plottables:
    graphResult = customPlot->addGraph();
    graphResult->clearData();
    graphResult->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::green, 4), QBrush(Qt::red), 2));
    graphResult->setPen(QPen(QColor(120, 120, 120), 0.1));

    graphResultJump = customPlot->addGraph();
    graphResultJump->clearData();
    graphResultJump->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::red, 5), QBrush(Qt::red), 2));
    graphResultJump->setPen(QPen(QColor(120, 120, 120), 0.1));

    graphResultSensor = customPlot->addGraph();
    graphResultSensor->clearData();
    graphResultSensor->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::green, 1), QBrush(Qt::yellow), 2));
    graphResultSensor->setPen(QPen(QColor(120, 120, 120), 0.1));

    graphOriginData = customPlot->addGraph();
    graphOriginData->clearData();
    graphOriginData->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::blue, 1), QBrush(Qt::yellow), 2));
    graphOriginData->setPen(QPen(QColor(120, 120, 120), 0.5));

    customPlot->yAxis->setRange(0, 100000);
    customPlot->xAxis->setRange(1, 600);
}

//连接信号与槽
void LightDebugWidget::initConnect()
{
    //connect(timer, SIGNAL(timeout()), this, SLOT(slt_Timer_timeout()));

    connect(btnReset, SIGNAL(clicked()), this, SLOT(slt_btnReset_Clicked()));
    connect(btnStart, SIGNAL(clicked()), this, SLOT(slt_btnStart_Clicked()));
    //connect(gSerialThread, SIGNAL(sig_ReceiveADData()), this, SLOT(slt_Timer_timeout()));
     connect(gSerialThread2, SIGNAL(sig_ReceiveADData()), this, SLOT(slt_Timer_timeout()));
}

//更新数据
void LightDebugWidget::updateData()
{
    QVector<int> DataAD, DataJump, DataSensor, OriginData;
    OriginData =  gSerialThread2->getVecLightingDebugADData();

    int temp, data;
    int JumpCount;    //跳变计数
    int zore = -1000;
    for(int i = 0; i < OriginData.count(); i ++)
    {
        temp = OriginData.at(i);
        //传感器
        if(temp > 32768)
            DataSensor.append(1);
        else
            DataSensor.append(0);

        //AD数据
        if( temp > 32768)
            data = temp-32768;
        else
            data = temp;
        DataAD.append(data);

        //编码信号数据跳变
        if(temp > 32768)
        {
            if( JumpCount < 100 )
            {
                JumpCount ++;
                DataJump.append(data);
            }
            else
            {
                DataJump.append(zore);
            }
        }
        else
        {
            JumpCount = 0;
            DataJump.append(zore);
        }

    }

    //        qDebug() << "Jump:" << DataJump.count();
    //        qDebug() << "DataAD:" << DataAD.count();
    //        qDebug() << "Sensor:" << DataSensor.count();
    //        qDebug() << "";

    int xAxis;
    if(DataAD.count() < 100)
        xAxis = 100;
    else
        xAxis = DataAD.count();

    QVector<double> x, y1, y2, y3, y4;
    x.clear();
    y1.clear();
    for(int i = 0; i < DataAD.count(); i ++)
    {
        //
        QCPData newData;
        newData.value = DataAD.at(i);
        x.append(i+1);
        y1.append(DataAD.at(i));
        customPlot->graph(2)->data()->insert(i, newData);

        //
        QCPData newData2;
        newData2.value = DataJump.at(i);
        y2.append(DataJump.at(i));
        customPlot->graph(3)->data()->insert(i, newData2);

        //
        QCPData newData3;
        double sensor;
        if( DataSensor.at(i) == 1 )
            sensor = 5000;
        else
            sensor = 2000;
        newData3.value = sensor;
        y3.append(sensor);
        customPlot->graph(4)->data()->insert(i, newData3);

        //
        //        QCPData newData4;
        //        newData4.value = OriginData.at(i);
        //        y4.append(OriginData.at(i));
        //        customPlot->graph(5)->data()->insert(i, newData4);

    }

    graphResult->clearData();
    graphResult->setData(x, y1);
    graphResult->setPen(QPen(QColor(180, 180, 180), 0.01));
    graphResult->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::green, 4), QBrush(Qt::red), 3));

    graphResultJump->clearData();
    graphResultJump->setData(x, y2);
    graphResultJump->setPen(QPen(QColor(180, 180, 180), 0.01));
    graphResultJump->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(Qt::red, 6), QBrush(Qt::red), 3));

    graphResultSensor->clearData();
    graphResultSensor->setData(x, y3);
    graphResultSensor->setPen(QPen(QColor(180, 180, 180), 6));

    //    graphOriginData->clearData();
    //    graphOriginData->setData(x, y4);
    //    graphOriginData->setPen(QPen(QColor(180, 180, 180), 0.5));
    //    graphOriginData->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(Qt::blue, 3), QBrush(Qt::red), 3));

    customPlot->yAxis->setRange(0, 7500);
    customPlot->xAxis->setRange(1, xAxis);
    customPlot->replot();  //实时更新曲线
}

//槽：复位
void LightDebugWidget::slt_btnReset_Clicked()
{
}

//槽：开始采光
void LightDebugWidget::slt_btnStart_Clicked()
{
    QString cup = AppHelper::IntToHexStr(editStartCup->text().toInt(), 2);
    QString delay1 = AppHelper::IntToHexStr(editDelay->text().toInt()/128, 2);
    QString delay2 = AppHelper::IntToHexStr(editDelay->text().toInt()%128, 2);

    QString str = "FA 0f 01 3F 41 08 " + cup + " " + delay1 + " " + delay2 +" 00 FB";
    gSerialThread->Write(str);

    customPlot->graph(2)->clearData();
    customPlot->graph(3)->clearData();
    customPlot->graph(4)->clearData();
    customPlot->replot();  //实时更新曲线
}

//槽：
void LightDebugWidget::slt_Timer_timeout()
{
    updateData();
}


