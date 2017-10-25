#include "resultCurveWidget.h"

//实例对象
ResultCurveWidget *ResultCurveWidget::_instance = 0;
ResultCurveWidget * ResultCurveWidget::getInstance()
{
    if (_instance == 0)
        _instance = new ResultCurveWidget(0);
    return _instance;
}

ResultCurveWidget::ResultCurveWidget(int id) :
    QDialog()
{
    start = 100;
    end = 10000;

    History_id = id;
    initForm();                           //初始化界面控件
    initStyle();                          //初始化样式
    initConnect();                        //连接信号与槽

    timeUpdateTestingData = new QTimer;   //要实例化
    connect(timeUpdateTestingData, SIGNAL(timeout()), this, SLOT(slt_timeUpdateTestingData_timeout()));
    initData();
}

ResultCurveWidget::~ResultCurveWidget()
{
    //    qDebug() << "~ResultCurveWidget...";
    //    delete timeUpdateTestingData;
    //    delete frameLogo;
    //    delete labTitle0;
    //    delete layout0;
    //    delete frameTitle0;
    //    delete frame;
    //    delete customPlot;
    //    delete btnPrevious;
    //    delete btnNext;
    //    delete layout;
}

//设置查询id
void ResultCurveWidget::setSelect_id(int id)
{
    idType = 0; //查询类型（0：临时结果表， 1：历史结果数据表）
    History_id = id;
    slt_timeUpdateTestingData_timeout();
    timeUpdateTestingData->start( gParameter->getCycleTime() );
}

//设置查询id（历史结果数据表）
void ResultCurveWidget::setSelect_History_id(int id)
{
    idType = 1; //查询类型（0：临时结果表， 1：历史结果数据表）
    History_id = id;
    slt_timeUpdateTestingData_timeout();
}

//语言设置
void ResultCurveWidget::initLanguage()
{
    btnNext->setText("下一个");
    btnPrevious->setText("上一个");
}

//初始化界面控件
void ResultCurveWidget::initForm()
{
    //测试信息
    labTestNo = new QLabel("测试编号:");         //测试编号
    editTestNo = new QLineEdit;
    labApplicateTime = new QLabel("申请时间:");  //申请时间
    editApplicateTime = new QLineEdit;
    labTestTime = new QLabel("测试时间:");  //测试时间
    editTestTime = new QLineEdit;

    labProjectName = new QLabel("项目名称:");    //项目名称
    editProjectName = new QLineEdit;
    labSamplePos = new QLabel("样本位置:");      //样本位置
    editSamplePos = new QLineEdit;
    labCup1 = new QLabel("反应杯1:");           //反应杯1
    editCup1 = new QLineEdit;
    labCup2 = new QLabel("反应杯2:");           //反应杯2
    editCup2 = new QLineEdit;
    labResult = new QLabel("结果浓度:");         //结果浓度
    editResult = new QLineEdit;
    labAD_Range = new QLabel("幅度:");          //幅度
    editAD_Range = new QLineEdit;
    grbTestInfo = new QGroupBox("测试信息");
    lytTestInfo = new QGridLayout;
    lytTestInfo->setMargin(10);
    lytTestInfo->setSpacing(5);
    lytTestInfo->addWidget(labApplicateTime,     0,  0, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editApplicateTime,    0,  5, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labTestTime,          2,  0, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editTestTime,         2,  5, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labProjectName,       0, 10, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editProjectName,      0, 15, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labSamplePos,         2, 10, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editSamplePos,        2, 15, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labCup1,              0, 20, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editCup1,             0, 25, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labCup2,              2, 20, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editCup2,             2, 25, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labResult,            0, 30, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editResult,           0, 35, 2, 5, Qt::AlignLeft);
    lytTestInfo->addWidget(labAD_Range,          2, 30, 2, 5, Qt::AlignRight);
    lytTestInfo->addWidget(editAD_Range,         2, 35, 2, 5, Qt::AlignLeft);
    grbTestInfo->setLayout(lytTestInfo);


    wdt = new QWidget;
    labUp = new QLabel("上限：");
    editUp = new QLineEdit;
    labDown = new QLabel("下限：");
    editDown = new QLineEdit;
    btnSure = new QPushButton("设 置");
    btnExprot = new QPushButton("导出数据");
    lyt = new QGridLayout;
    lyt->setSpacing(1);
    lyt->setMargin(5);
    lyt->addWidget(labUp,     0,  0, 1, 5, Qt::AlignRight);
    lyt->addWidget(editDown,  0,  5, 1, 5, Qt::AlignLeft);
    lyt->addWidget(labDown,   1,  0, 1, 5, Qt::AlignRight);
    lyt->addWidget(editUp,    1,  5, 1, 5, Qt::AlignLeft);
    lyt->addWidget(btnSure,   0, 10, 1, 5, Qt::AlignCenter);
    lyt->addWidget(btnExprot, 1, 10, 1, 5, Qt::AlignCenter);
    wdt->setLayout(lyt);

    wdgBtn = new QWidget;
    btnPrevious = new QPushButton("上一个");
    btnNext = new QPushButton("下一个");
    QVBoxLayout *hlytBtn = new QVBoxLayout;
    hlytBtn->setMargin(5);
    hlytBtn->setSpacing(1);
    hlytBtn->addWidget(btnPrevious);
    hlytBtn->addWidget(btnNext);
    wdgBtn->setLayout(hlytBtn);

    //临时历史结果表中的id（测试用）
    lab_Hid = new QLabel;

    customPlot = new QCustomPlot;
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(3);

    layout->addWidget(grbTestInfo,       0,  0,  5, 50, Qt::AlignLeft);
    layout->addWidget(lab_Hid,           0,  0,  5, 50, Qt::AlignRight | Qt::AlignTop);  //测试用
    layout->addWidget(wdgBtn,            0, 50,  5, 30, Qt::AlignCenter);
    layout->addWidget(wdt,               0, 80,  5, 20, Qt::AlignRight);
    layout->addWidget(customPlot,        5,  0, 35, 100);
    setLayout(layout);
}

//初始化样式
void ResultCurveWidget::initStyle()
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint);       //去掉标题栏
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);       //去掉标题栏

    int width, height;
    width = 1360;
    height = 768;
    this->setMinimumSize(width, height);

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

    QString styleBTN1 =
            "QPushButton{width:90; height:50;  border-radius:5px; font-size:20px; background-image:url(:/resource/resource/button/button6.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton:hover{width:90; height:50; border-radius:8px; font-size:23px; color:darkCyan; background-image:url(:/resource/resource/button/button6.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton:pressed{width:90; height:50; border-radius:8px; font-size:25px; color:darkMagenta; background-image:url(:/resource/resource/button/button6_pressed.png); background-repeat:no-repeat; background-position:center; }"
            ;
    btnNext->setStyleSheet(styleBTN1);
    btnPrevious->setStyleSheet(styleBTN1);
    btnExprot->setStyleSheet(styleBTN1);
    btnSure->setStyleSheet(styleBTN1);

    QString styleEDIT =
            "QLineEdit{ width:90; height:30; font-size:22px; border: 1px solid rgb(41, 57, 85); border-radius:3px; background:white; selection-background-color:green;}"
            "QLineEdit:hover{border: 1px solid blue;}";
    editUp->setStyleSheet(styleEDIT);
    editDown->setStyleSheet(styleEDIT);

    editTestNo->setReadOnly(true);
    editApplicateTime->setReadOnly(true);
    editProjectName->setReadOnly(true);
    editSamplePos->setReadOnly(true);
    editCup1->setReadOnly(true);
    editCup2->setReadOnly(true);
    editResult->setReadOnly(true);
    editAD_Range->setReadOnly(true);

    //设置背景
    this->setAutoFillBackground( true );
    QString bg_str = ":/resource/resource/bg/mainBg.png";
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap( bg_str )));
    this->setPalette(palette);

    //图标设置
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setStyleSheet("*{font-size:18px;}");


    editApplicateTime->setFixedWidth(180);
    editTestTime->setFixedWidth(180);
    editProjectName->setFixedWidth(80);
    editSamplePos->setFixedWidth(80);
    editCup1->setFixedWidth(60);
    editCup2->setFixedWidth(60);
    editResult->setFixedWidth(100);
    editAD_Range->setFixedWidth(100);
}

//连接信号与槽
void ResultCurveWidget::initConnect()
{
    connect(btnPrevious, SIGNAL(clicked()), this, SLOT(slt_btnPrevious_Clicked()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slt_btnNext_Clicked()));
    connect(btnExprot, SIGNAL(clicked()), this, SLOT(slt_btnExprot_Clicked()));
    connect(btnSure, SIGNAL(clicked()), this, SLOT(slt_btnSure_Clicked()));
}

//初始化数据
void ResultCurveWidget::initData()
{
    editUp->setText(QString::number(start));
    editDown->setText(QString::number(end));
    lightPos1 = lightPos2 = 0; //测光点1，2

//    int dataCount = 30;  //只显示50个数据
    //add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

    //create and configure plottables:
    graphResult = customPlot->addGraph();
    graphResult->clearData();
    graphResult->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black, 4), QBrush(Qt::red), 2));
    graphResult->setPen(QPen(QColor(120, 120, 120), 0.1));

    ////测光点1, 2
    graphLightPos1 = customPlot->addGraph();
    graphLightPos1->clearData();
    graphLightPos1->setPen(QPen(Qt::darkBlue, 1));
    graphLightPos2 = customPlot->addGraph();
    graphLightPos2->clearData();
    graphLightPos2->setPen(QPen(Qt::red, 1));

    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(start, end);
//    customPlot->xAxis->setRange(1, dataCount);
}

//查询AD数据
QVector<double> ResultCurveWidget::selectDB()
{
    QVector<double> vecData;
    vecData.clear();

    if(History_id == 0) return vecData;
    QString str, text;
    if(idType == 0) //历史结果数据临时表(TempHistoryT)
    {
        str =
                "select AD_A1, AD_A2, AD_A3, AD_A4, AD_A5, AD_A6, AD_A7, AD_A8, AD_A9, AD_A10, AD_A11, AD_A12, AD_A13, AD_A14, AD_A15, AD_A16, AD_A17, AD_A18, AD_A19, AD_A20, AD_A21, AD_A22, AD_A23, AD_A24, AD_A25, AD_A26,"
                " AD_A27, AD_A28, AD_A29, AD_A30, AD_A31, AD_A32, AD_A33, AD_A34, AD_A35, AD_A36, AD_A37, AD_A38, AD_A39, AD_A40, AD_A41, AD_A42, AD_A43, AD_A44, AD_A45, AD_A46, AD_A47, AD_A48, AD_A49, AD_A50, "
                " TempHistoryT.statusID, TempHistoryT.no, TempHistoryT.applyTime, ProjectT.name, SamplePos, Cup0, Cup1, "
                " result, amplitude, ProjectT.lightPos1, ProjectT.lightPos2, TempHistoryT.testDate "
                " from TempHistoryT "
                //连接方式(任务队列表、项目表)
                " Inner join ProjectT ON TempHistoryT.projectID = ProjectT.id "
                " where TempHistoryT.id = " + QString::number( History_id ) + ";";
    }
    else            //历史结果数据表(HistoryT)
    {
        str =
                "select AD_A1, AD_A2, AD_A3, AD_A4, AD_A5, AD_A6, AD_A7, AD_A8, AD_A9, AD_A10, AD_A11, AD_A12, AD_A13, AD_A14, AD_A15, AD_A16, AD_A17, AD_A18, AD_A19, AD_A20, AD_A21, AD_A22, AD_A23, AD_A24, AD_A25, AD_A26,"
                " AD_A27, AD_A28, AD_A29, AD_A30, AD_A31, AD_A32, AD_A33, AD_A34, AD_A35, AD_A36, AD_A37, AD_A38, AD_A39, AD_A40, AD_A41, AD_A42, AD_A43, AD_A44, AD_A45, AD_A46, AD_A47, AD_A48, AD_A49, AD_A50, "
                " HistoryT.statusID, HistoryT.no, HistoryT.applyTime, ProjectT.name, SamplePos, Cup0, Cup1, "
                " result, amplitude, ProjectT.lightPos1, ProjectT.lightPos2, HistoryT.testDate"
                " from HistoryT "
                //连接方式(任务队列表、项目表)
                " Inner join ProjectT ON HistoryT.projectID = ProjectT.id "
                " where HistoryT.id = " + QString::number( History_id ) + ";";
    }

    //qDebug() << str;
    QSqlQuery query = SQLite::getInstance()->Select(str);  //查询数据库
    while( query.next() )
    {
        for( int i = 0; i < 50; i ++ )
        {
            text = query.value(i).toString();
            if( text != "")
            {
                vecData.append(text.toDouble());
            }
        }

        //query.value(50).toString(); //状态？？
        editTestNo->setText(query.value(51).toString());
        editApplicateTime->setText(query.value(52).toString());
        editProjectName->setText(query.value(53).toString());
        editSamplePos->setText(query.value(54).toString());
        editCup1->setText(query.value(55).toString());
        editCup2->setText(query.value(56).toString());
        editResult->setText( QString::number(query.value(57).toDouble(), 'g', 6));
        editAD_Range->setText(query.value(58).toString());
        lightPos1 = query.value(59).toInt();
        lightPos2 = query.value(60).toInt();
        editTestTime->setText(query.value(61).toString());

        lab_Hid->setText(QString::number(History_id));
    }
    return vecData;
}

void ResultCurveWidget::slt_timeUpdateTestingData_timeout()
{
    QVector<double> vecData;
    vecData.clear();
    vecData = selectDB();

    QVector<double> x, y1;
    x.clear();
    y1.clear();
    for(int i = 0; i < vecData.count(); i ++)
    {
        QCPData newData;
        newData.value = vecData.at(i);
        x.append(i+1);
        y1.append(vecData.at(i));
        customPlot->graph(2)->data()->insert(i, newData);
    }
    graphResult->clearData();
    graphResult->setData(x, y1);
    graphResult->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QPen(Qt::black, 6), QBrush(Qt::red), 3));
    graphResult->setPen(QPen(QColor(180, 180, 180), 0.8));

    ////测光点1, 2
    QVector<double> L1, L2, lPot;
    L1.append(lightPos1);
    L2.append(lightPos2);
    lPot.append(end);
    L1.append(lightPos1);
    L2.append(lightPos2);
    lPot.append(-100);
    graphLightPos1->clearData();
    graphLightPos1->setData(L1, lPot);
    graphLightPos2->clearData();
    graphLightPos2->setData(L2, lPot);

    //
    int upper = 26;
    if(vecData.count() > 20)
        upper = vecData.count() + 4;

    customPlot->xAxis->setRange(1, upper);
    customPlot->replot();  //实时更新曲线
}

//槽：上一个
void ResultCurveWidget::slt_btnPrevious_Clicked()
{
    History_id = History_id-1;
    ClearTestInfo();
    slt_timeUpdateTestingData_timeout();
}

//槽：下一个
void ResultCurveWidget::slt_btnNext_Clicked()
{
    History_id = History_id+1;
    ClearTestInfo();
    slt_timeUpdateTestingData_timeout();
}

//槽：导出Excel
void ResultCurveWidget::slt_btnExprot_Clicked()
{
    //用时间来作为excle的文件名
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyyMMddhhmmss"); //设置显示格式

    QString col1;
    //创建一个文本流
    QFile fileWrite("Data/"+ str +".txt");
    if (!fileWrite.open(QIODevice::Text | QFile::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out(&fileWrite);
    QVector<double> data1 = selectDB();

    for (int i = 0; i < data1.count(); i++)
    {
        col1 = "";
        if( i < data1.count() )
            col1 = QString::number( data1.at(i) );
        out << col1;
        out << "\n";//写完一行数据
    }
    fileWrite.close();
}

//槽：更改（曲线上下限设置）
void ResultCurveWidget::slt_btnSure_Clicked()
{
    start = editUp->text().toInt();
    end = editDown->text().toInt();

    customPlot->yAxis->setRange(start, end);
    customPlot->update();
    slt_timeUpdateTestingData_timeout();
}

//清空测试信息
void ResultCurveWidget::ClearTestInfo()
{
    editTestNo->setText("");
    editApplicateTime->setText("");
    editProjectName->setText("");
    editSamplePos->setText("");
    editCup1->setText("");
    editCup2->setText("");
    editResult->setText("");
    editAD_Range->setText("");
}

//
void ResultCurveWidget::moveEvent(QMoveEvent *event)
{
    //this->setGeometry(200, 100, 1700, 800);
}
