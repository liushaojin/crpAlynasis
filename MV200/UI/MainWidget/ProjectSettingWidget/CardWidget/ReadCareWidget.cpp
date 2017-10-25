#include "ReadCareWidget.h"
#include "Thread/Scheduler.h"

ReadCareWidget::ReadCareWidget(QDialog *parent) : QDialog(parent)
{
    initForm();              //初始化
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//开始读卡
void ReadCareWidget::StartReadCard()
{
    wdgReading->setVisible(true);
    wdgCardData->setVisible(false);
    slt_ProgressBar_ValueChanged(0);
    gScheduler->StartReadCard();
}

//初始化
void ReadCareWidget::initForm()
{
    initReadingForm();     //初始化-准备读卡
    initCardDataForm();    //初始化-读卡数据
}

//初始化-准备读卡
void ReadCareWidget::initReadingForm()
{
    //准备读卡
    wdgReading = new QWidget;
    labReadingStatus = new QLabel;        //读卡状态
    barReadingStatus = new QProgressBar;  //读卡进度条
    lytReading = new QGridLayout;
    lytReading->setMargin(10);
    lytReading->setSpacing(10);
    lytReading->addWidget(labReadingStatus,  5, 0, 5, 100, Qt::AlignCenter | Qt::AlignBottom);
    lytReading->addWidget(barReadingStatus, 10, 0, 5, 100);
    wdgReading->setLayout(lytReading);
    barReadingStatus->setRange(0, 10);
}

//初始化-读卡数据
void ReadCareWidget::initCardDataForm()
{
    //基本参数
    labProjectName = new QLabel("项目名称:");      //项目名称
    editProjectName = new QLineEdit;
    labBatchNum = new QLabel("批号:");             //批号
    editBatchNum = new QLineEdit;
    labAccuracy = new QLabel("结果精度:");         //结果精度
    cboAccuracy = new QComboBox;
    labAccuracy2 = new QLabel("位");            //结果单位小数几位
    labIsDiluent = new QLabel("是否要稀释:");      //是否要稀释
    cboIsDiluent = new QComboBox;
    labUnit = new QLabel("结果单位:");             //结果单位
    editUnit = new QLineEdit;
    labParam_K = new QLabel("K因数值:");          //K因数值
    editParam_K = new QLineEdit;
    labBloodFactor = new QLabel("全血因子:");      //全血因子
    editBloodFactor = new QLineEdit;
    labLowLinear = new QLabel("线性下限:");    //线性下限
    editLowLinear = new QLineEdit;

    //加样量设置
    grbAddingAmount = new QGroupBox("加样量设置(ul)");
    lytAddingAmount = new QGridLayout;
    labDiluent = new QLabel("稀释液:");          //稀释液
    editDiluent = new QLineEdit;
    labSample = new QLabel("样本:");             //样本
    labS_Transfer = new QLabel("样本转移量:");    //样本转移量
    editS_Transfer = new QLineEdit;
    editSample = new QLineEdit;
    labR1 = new QLabel("R1:");               //R1
    editR1 = new QLineEdit;
    labR2 = new QLabel("R2:");               //R2
    editR2 = new QLineEdit;
    lytAddingAmount->setMargin(5);
    lytAddingAmount->setSpacing(5);
    lytAddingAmount->addWidget(labR1,              2,  0, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editR1,             2,  5, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labSample,          2, 10, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editSample,         2, 15, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labR2,              2, 20, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editR2,             2, 25, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labDiluent,         4,  0, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editDiluent,        4,  5, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labS_Transfer,      4, 10, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editS_Transfer,     4, 15, 2, 5, Qt::AlignLeft);
    grbAddingAmount->setLayout(lytAddingAmount);

    labReactionCycle = new QLabel("反应周期(个数):");    //反应周期
    editCycle1 = new QLineEdit;
    editCycle2 = new QLineEdit;

    //项目定标参数
    grbCalibrateParam_1 = new QGroupBox("定标参数1");
    lytCalibrateParam_1 = new QGridLayout;
    chkCalibrateParam_1 = new QCheckBox("启用参数");  //参数是否启用
    lab_R0_1 = new QLabel("R0:");
    edit_R0_1 = new QLineEdit;
    lab_K_1 = new QLabel("K:");
    edit_K_1 = new QLineEdit;
    lab_a_1 = new QLabel("a:");
    edit_a_1 = new QLineEdit;
    lab_b_1 = new QLabel("b:");
    edit_b_1 = new QLineEdit;
    lab_c_1 = new QLabel("c:");
    edit_c_1 = new QLineEdit;
    labCalibrateParam_R_1 = new QLabel("最大幅度:");     //第一段曲线反应幅度分段点
    editCalibrateParam_R_1 = new QLineEdit;
    labCalibrateParam_C_1 = new QLabel("最高浓度:");     //第一段曲线浓度分段点
    editCalibrateParam_C_1 = new QLineEdit;
    lytCalibrateParam_1->setMargin(5);
    lytCalibrateParam_1->setSpacing(5);
    lytCalibrateParam_1->addWidget(chkCalibrateParam_1,   16, 0, 2, 5, Qt::AlignCenter);
    lytCalibrateParam_1->addWidget(lab_R0_1,   14,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_R0_1,  14, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_K_1,    14, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_K_1,   14, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_a_1,    16,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_a_1,   16, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_b_1,    16, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_b_1,   16, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_c_1,    16, 25, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_c_1,   16, 30, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(labCalibrateParam_R_1,    18,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(editCalibrateParam_R_1,   18, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(labCalibrateParam_C_1,    18, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(editCalibrateParam_C_1,   18, 20, 2, 5, Qt::AlignLeft);
    grbCalibrateParam_1->setLayout(lytCalibrateParam_1);

    grbCalibrateParam_2 = new QGroupBox("定标参数2");
    lytCalibrateParam_2 = new QGridLayout;
    chkCalibrateParam_2 = new QCheckBox("启用参数");  //参数是否启用
    lab_R0_2 = new QLabel("R0:");
    edit_R0_2 = new QLineEdit;
    lab_K_2 = new QLabel("K:");
    edit_K_2 = new QLineEdit;
    lab_a_2 = new QLabel("a:");
    edit_a_2 = new QLineEdit;
    lab_b_2 = new QLabel("b:");
    edit_b_2 = new QLineEdit;
    lab_c_2 = new QLabel("c:");
    edit_c_2 = new QLineEdit;
    labCalibrateParam_R_2 = new QLabel("最大幅度:");     //第二段曲线反应幅度分段点
    editCalibrateParam_R_2 = new QLineEdit;
    labCalibrateParam_C_2 = new QLabel("最高浓度:");     //第二段曲线浓度分段点
    editCalibrateParam_C_2 = new QLineEdit;
    lytCalibrateParam_2->setMargin(5);
    lytCalibrateParam_2->setSpacing(5);
    lytCalibrateParam_2->addWidget(chkCalibrateParam_2,   16, 0, 2, 5, Qt::AlignCenter);
    lytCalibrateParam_2->addWidget(lab_R0_2,   14,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_R0_2,  14, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_K_2,    14, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_K_2,   14, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_a_2,    16,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_a_2,   16, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_b_2,    16, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_b_2,   16, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_c_2,    16, 25, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_c_2,   16, 30, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(labCalibrateParam_R_2,    18,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(editCalibrateParam_R_2,   18, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(labCalibrateParam_C_2,    18, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(editCalibrateParam_C_2,   18, 20, 2, 5, Qt::AlignLeft);
    grbCalibrateParam_2->setLayout(lytCalibrateParam_2);

    btnCancel = new QPushButton("取消");
    btnSave = new QPushButton("保存");

    lytBasicParam = new QGridLayout;
    lytBasicParam->setMargin(10);
    lytBasicParam->setSpacing(10);
    lytBasicParam->addWidget(labProjectName,    0,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editProjectName,   0,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labBatchNum,        0, 10, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editBatchNum,       0, 15, 2, 5, Qt::AlignLeft);

    lytBasicParam->addWidget(labIsDiluent,       2,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(cboIsDiluent,       2,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labAccuracy,        2, 10, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(cboAccuracy,        2, 15, 2, 4, Qt::AlignLeft);
    lytBasicParam->addWidget(labAccuracy2,       2, 15, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(labUnit,            2, 20, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editUnit,           2, 25, 2, 5, Qt::AlignLeft);

    lytBasicParam->addWidget(grbAddingAmount,    4,  0, 6, 30);

    lytBasicParam->addWidget(labReactionCycle,  10,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editCycle1,        10,  5, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(new QLabel("~"),   10, 10, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(editCycle2,        10, 15, 2, 5, Qt::AlignCenter);

    lytBasicParam->addWidget(labParam_K,        12,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editParam_K,       12,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labBloodFactor,    12, 10, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editBloodFactor,   12, 15, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labLowLinear,      12, 20, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editLowLinear,     12, 25, 2, 5, Qt::AlignLeft);

    lytBasicParam->addWidget(grbCalibrateParam_1,     16, 0, 6, 30);
    lytBasicParam->addWidget(grbCalibrateParam_2,     22, 0, 6, 30);
    lytBasicParam->addWidget(btnCancel,    30,  0, 2, 15, Qt::AlignCenter);
    lytBasicParam->addWidget(btnSave,    30, 15, 2, 15, Qt::AlignCenter);

    //读卡数据
    wdgCardData = new QWidget;
    wdgCardData->setLayout(lytBasicParam);
}

//界面风格
void ReadCareWidget::initStyle()
{
    wdgReading->setWindowFlags(Qt::WindowStaysOnTopHint);
    wdgCardData->setMinimumHeight(830);

    //QSS
    QFile qss( ":/QSS/QSS/style1.qss" );
    qss.open(QFile::ReadOnly);
    QString style = qss.readAll();
    wdgCardData->setStyleSheet( style );
    wdgReading->setStyleSheet(style);
    qss.close();

    btnCancel->setFixedSize(120, 40);
    btnSave->setFixedSize(120, 40);

    //默认定标参数1不可少的（所有项目至少有一段定标曲线）
    chkCalibrateParam_1->setChecked(true);
    chkCalibrateParam_1->setEnabled(false);

    //项目定标参数2
    chkCalibrateParam_2->setChecked(true);
}

//连接信号与槽
void ReadCareWidget::initConnect()
{
    connect(chkCalibrateParam_2, SIGNAL(stateChanged(int)), this, SLOT(slt_chkCalibrateParam_2_Check(int)));
    connect(gScheduler, SIGNAL(sig_ReadCard_State(int)), this, SLOT(slt_ReadCard_Error(int)));
    connect(gTcpServer, SIGNAL(sig_ReadCard_State(int)), this, SLOT(slt_ReadCard_Error(int)));
    connect(gScheduler, SIGNAL(sig_ReadCard_Data(QString)), this, SLOT(slt_ReadCard_Data(QString)));
    connect(gScheduler, SIGNAL(sig_ReadCardProgressBar_ValueChanged(int)), this, SLOT(slt_ProgressBar_ValueChanged(int)));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(slt_btnSave_Clicked()));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
    connect(cboIsDiluent, SIGNAL(currentIndexChanged(int)), this, SLOT(slt_cboIsDiluent_IndexChanged(int)));
}

//初始化下拉列表
void ReadCareWidget::initDataComboBox()
{
    QStringList list;
    //是否要稀释
    list.clear();
    list << "否(N)" << "是(Y)";
    cboIsDiluent->addItems(list);

    list.clear();
    list<< "0" << "1" << "2" << "3" << "4" << "5";
    cboAccuracy->addItems(list);
}

//初始化数据
void ReadCareWidget::initData()
{
    initDataComboBox();
}


//数据格式化
QString ReadCareWidget::FormateData(QString data)
{
    if(data.count() == 0) return "";

    QString tempText, str;
    for( int i = 0; i < data.count(); i ++ )
    {
        tempText += (char) data.mid(i, 2).toInt(0, 16);
        i ++;
    }

    for( int i = 0; i < tempText.count(); i ++ )
    {
        str += (char) tempText.mid(i, 2).toInt(0, 16);
        i ++;
    }
    return str;
}

//绑定数据
void ReadCareWidget::setCardData(QString data)
{
    QStringList list = data.split(CONNECTOR);
    int count = list.count();
    qDebug() << "数据个数：" << count;
    qDebug() << list;

    //防止下标越界
    if(count < 31) //数据丢失
    {
        slt_ReadCard_Error(101);
        return;
    }
    //检验卡片类型与机器类型是否对应
    if(list.at(0) != CARD_TYPE) //MV200
    {
       slt_ReadCard_Error(102); //检验卡片类型与机器类型是否对应
        return;
    }

    editProjectName->setText(list.at(1));    //项目名称
    editBatchNum->setText(list.at(2));       //批号

    cboIsDiluent->setCurrentIndex(list.at(3).toInt());//是否要稀释
    cboAccuracy->setCurrentIndex(list.at(4).toInt()); //结果精度
    editUnit->setText(list.at(5));                    //项目单位

    editR1->setText(list.at(6));             //R1
    editSample->setText(list.at(7));         //样本
    editR2->setText(list.at(8));             //R2
    editDiluent->setText(list.at(9));        //稀释液
    editS_Transfer->setText(list.at(10));    //样本转移量

    editCycle1->setText(list.at(11));          //反应周期（采光点1）
    editCycle2->setText(list.at(12));          //反应周期（采光点2）
    editParam_K->setText(list.at(13));         //K因数值
    editBloodFactor->setText(list.at(14));     //全血因子
    editLowLinear->setText(list.at(15));       //线性下限

    chkCalibrateParam_1->setChecked(list.at(16).toInt());
    edit_R0_1->setText(list.at(17));       //项目定标参数-R0
    edit_K_1->setText(list.at(18));        //项目定标参数-K
    edit_a_1->setText(list.at(19));        //项目定标参数-a
    edit_b_1->setText(list.at(20));        //项目定标参数-b
    edit_c_1->setText(list.at(21));        //项目定标参数-c
    editCalibrateParam_R_1->setText(list.at(22));
    editCalibrateParam_C_1->setText(list.at(23));

    chkCalibrateParam_2->setChecked(list.at(24).toInt());
    edit_R0_2->setText(list.at(25));       //项目定标参数-R0
    edit_K_2->setText(list.at(26));        //项目定标参数-K
    edit_a_2->setText(list.at(27));        //项目定标参数-a
    edit_b_2->setText(list.at(28));        //项目定标参数-b
    edit_c_2->setText(list.at(29));        //项目定标参数-c
    editCalibrateParam_R_2->setText(list.at(30));
    editCalibrateParam_C_2->setText(list.at(31));

    wdgReading->setVisible(false);
    wdgCardData->setVisible(true);
}

//槽:读卡出现错误
void ReadCareWidget::slt_ReadCard_Error(int state)
{
    QString msg;
    switch (state)
    {
    case OK:   //读卡成功(20170609读卡成功省略提示)
        msg = "读卡成功!";
        return;
        break;
    case NO_TAG_ERR:   //在有效区域内没有卡
        msg = "在有效区域内没有卡...";
        break;
    case AUTH_ERR:     //不能验证
        msg = "不能验证...";
        break;
    case NOT_AUTH_ERR: //卡没有验证
        msg = "卡没有验证...";
        break;
    case 101: //读卡成功，但数据丢失 20170610
        msg = "卡内数据不足或丢失\n请重试或更换卡片...";
        break;
    case 102: //检验卡片类型与机器类型是否对应 20170610
        msg = "卡片类型与机器类型不对应\n请更换卡片后重试...";
        break;
    default:           //读卡出现错误
        msg = "读卡出现错误...";
        break;
    }

    wdgReading->setVisible(false);
    gMessageBox->setInfo(QString("提示"), msg,
                         QPixmap("://resource/MessageBox/information.png"), true, true);
    gMessageBox->setButtonText(Cancel, "返回");
    gMessageBox->show();
}

//槽:读卡数据
void ReadCareWidget::slt_ReadCard_Data(QString data)
{
    setCardData( FormateData(data) );
}

//槽:保存数据
void ReadCareWidget::slt_btnSave_Clicked()
{
    //先检查是否已经有该项目存在
    int id = 0;
    QString name = editProjectName->text().trimmed(); //项目名称
    QString batchNum = editBatchNum->text().trimmed();//批号

    QString str  = "SELECT id FROM ProjectT where name ='" + name +"';";  //表的总行数
    QSqlQuery  query = SQLite::getInstance()->Select(str);
    while(query.next())
        id = query.value(0).toInt();

    //是否要覆盖
    if( id )
    {
        gMessageBox->setInfo(QString("提示"), QString("该项目已经存在，是否要覆盖！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Sure, "是(Y)");
        gMessageBox->setButtonText(Cancel, "否(N)");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            return;
        }
        else
        {
            CoverOldProject(id);
        }
    }
    else
    {
        AddNewProject();        
        gMessageBox->setInfo(QString("提示"), QString("保存成功！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "确定");
        gMessageBox->show();
    }
    wdgReading->setVisible(false);
    wdgCardData->setVisible(false);
}

//槽:取消
void ReadCareWidget::slt_btnCancel_Clicked()
{
    wdgReading->setVisible(false);
    wdgCardData->setVisible(false);
}

//新增项目
void ReadCareWidget::AddNewProject()
{
    QString name = editProjectName->text().trimmed();          //项目名称
    QString batchNum = editBatchNum->text().trimmed();         //批号
    int isDiluent = cboIsDiluent->currentIndex();              //是否要稀释
    int accuracy = cboAccuracy->currentText().toInt();         //结果精度
    QString unit = editUnit->text().trimmed();                 //结果单位
    double param_K = editParam_K->text().toDouble();           //K因数值
    double lowLinear = editLowLinear->text().toDouble();             //线性下限
    double bloodFactor = editBloodFactor->text().toDouble();   //全血因子
    int diluent = editDiluent->text().toInt();                 //稀释液
    int sample = editSample->text().toInt();                   //样本
    int transfer = editS_Transfer->text().toInt();             //样本转移量
    int r1 = editR1->text().toInt();                           //R1
    int r2 = editR2->text().toInt();                           //R2
    int reactionCycle1 = editCycle1->text().toInt();           //反应周期/时间(s)
    int reactionCycle2 = editCycle2->text().toInt();           //反应周期/时间(s)

    //定标分段曲线1
    long double calibrate1_R0 = edit_R0_1->text().toDouble();
    long double calibrate1_K = edit_K_1->text().toDouble();
    long double calibrate1_a = edit_a_1->text().toDouble();
    long double calibrate1_b = edit_b_1->text().toDouble();
    long double calibrate1_c = edit_c_1->text().toDouble();
    long double calibrate1_RR = editCalibrateParam_R_1->text().toDouble();
    long double calibrate1_CC = editCalibrateParam_C_1->text().toDouble();

    //定标分段曲线2
    int isCalibrate2 = chkCalibrateParam_2->isChecked();
    long double calibrate2_R0 = edit_R0_2->text().toDouble();
    long double calibrate2_K = edit_K_2->text().toDouble();
    long double calibrate2_a = edit_a_2->text().toDouble();
    long double calibrate2_b = edit_b_2->text().toDouble();
    long double calibrate2_c = edit_c_2->text().toDouble();
    long double calibrate2_RR = editCalibrateParam_R_2->text().toDouble();
    long double calibrate2_CC = editCalibrateParam_C_2->text().toDouble();

    QString str =
            "insert into ProjectT(name, batchNum,"
            "isDiluent, accuracy, unit,"
            "r0ID, r1ID, r2ID, sample_V, r0_V, r1_V, rs_V, r2_V,"
            "lightPos1, lightPos2, K, lowLinear, bloodFactor,"
            //定标参数
            "calibrateID, "
            "calibrate1_R0, calibrate1_K, calibrate1_a, calibrate1_b, calibrate1_c, calibrate1_RR, calibrate1_CC, "
            "isCalibrate2, calibrate2_R0, calibrate2_K, calibrate2_a, calibrate2_b, calibrate2_c, calibrate2_RR, calibrate2_CC) "
            " values( "
            "'" + name + "', "             //项目名称
            + "'" + batchNum +"', "        //批号

            + QString::number( isDiluent ) +", "   //稀释否（是否需要稀释样本）
            + QString::number( accuracy   ) +", "  //结果精度
            + "'" + unit+"', "                     //项目单位

            + " 0, "             //R0试剂ID（试剂表）(试剂位置)
            + " 0, "             //R1试剂ID（试剂表）(试剂位置)
            + " 0, "             //R2试剂ID（试剂表）(试剂位置)

            + QString::number( sample ) +", "       //样本量（ul）
            + QString::number( diluent ) +", "      //探针吸R0量（ul）
            + QString::number( r1 ) +", "           //探针吸R1量（ul）
            + QString::number( transfer ) +", "     //探针吸稀释后的样本量（ul）样本转移量
            + QString::number( r2 ) +", "           //探针吸R2量（ul）

            + QString::number( reactionCycle1 ) +", "       //测光点1
            + QString::number( reactionCycle2 ) +", "       //测光点2
            + QString::number( param_K ) +", "      //K因数值
            + QString::number( lowLinear ) +", "    //线性下限
            + QString::number( bloodFactor ) +", "  //全血因子

            + QString::number( 0 ) +", "                 //定标方式ID（定标方式表）
            + QString::number( calibrate1_R0, 'f', 6) +", "     //定标分段曲线1-R0
            + QString::number( calibrate1_K, 'f', 6 ) +", "      //定标分段曲线1-K
            + QString::number( calibrate1_a, 'f', 6 ) +", "      //定标分段曲线1-a
            + QString::number( calibrate1_b, 'f', 6 ) +", "      //定标分段曲线1-b
            + QString::number( calibrate1_c, 'f', 6 ) +", "      //定标分段曲线1-c
            + QString::number( calibrate1_RR, 'f', 6 ) +", "     //定标分段曲线1-最大幅度
            + QString::number( calibrate1_CC, 'f', 6 ) +", "     //定标分段曲线1-最高浓度

            + QString::number( isCalibrate2 ) +", "          //定标分段曲线2-是否有定标参数二
            + QString::number( calibrate2_R0, 'f', 6 ) +", "         //定标分段曲线2-R0
            + QString::number( calibrate2_K, 'f', 6 ) +", "          //定标分段曲线2-K
            + QString::number( calibrate2_a, 'f', 6 ) +", "          //定标分段曲线2-a
            + QString::number( calibrate2_b, 'f', 6 ) +", "          //定标分段曲线2-b
            + QString::number( calibrate2_c, 'f', 6 ) +", "          //定标分段曲线2-c
            + QString::number( calibrate2_RR, 'f', 6 ) +", "         //定标分段曲线2-最大幅度
            + QString::number( calibrate2_CC, 'f', 6)               //定标分段曲线2-最高浓度
            + " );";
    SQLite::getInstance()->Insert(str);
    emit sig_updateProject(name);
}

//覆盖原来的项目(即更新项目参数)
void ReadCareWidget::CoverOldProject(int id)
{
    QString name = editProjectName->text().trimmed();          //项目名称
    QString batchNum = editBatchNum->text().trimmed();         //批号
    int isDiluent = cboIsDiluent->currentIndex();              //是否要稀释
    int accuracy = cboAccuracy->currentText().toInt();         //结果精度
    QString unit = editUnit->text().trimmed();                 //结果单位
    double  param_K = editParam_K->text().toDouble();          //K因数值
    double lowLinear = editLowLinear->text().toDouble();             //项目线性下限
    double bloodFactor = editBloodFactor->text().toDouble();   //全血因子
    int diluent = editDiluent->text().toInt();                 //稀释液
    int sample = editSample->text().toInt();                   //样本
    int transfer = editS_Transfer->text().toInt();             //样本转移量
    int r1 = editR1->text().toInt();                           //R1
    int r2 = editR2->text().toInt();                           //R2
    int reactionCycle1 = editCycle1->text().toInt();           //反应周期/时间(s)
    int reactionCycle2 = editCycle2->text().toInt();           //反应周期/时间(s)
    //定标分段曲线1
    long double  calibrate1_R0 = edit_R0_1->text().toDouble();
    long double  calibrate1_K = edit_K_1->text().toDouble();
    long double  calibrate1_a = edit_a_1->text().toDouble();
    long double  calibrate1_b = edit_b_1->text().toDouble();
    long double  calibrate1_c = edit_c_1->text().toDouble();
    double  calibrate1_RR = editCalibrateParam_R_1->text().toDouble();
    double  calibrate1_CC = editCalibrateParam_C_1->text().toDouble();

    //定标分段曲线2
    int isCalibrate2 = chkCalibrateParam_2->isChecked();
    long double  calibrate2_R0 = edit_R0_2->text().toDouble();
    long double  calibrate2_K = edit_K_2->text().toDouble();
    long double  calibrate2_a = edit_a_2->text().toDouble();
    long double  calibrate2_b = edit_b_2->text().toDouble();
    long double  calibrate2_c = edit_c_2->text().toDouble();
    double  calibrate2_RR = editCalibrateParam_R_2->text().toDouble();
    double  calibrate2_CC = editCalibrateParam_C_2->text().toDouble();

    QString str =
            "update ProjectT set "
            " name='" + name + "', "                   //项目名称
            " batchNum='" + batchNum +"', "    //项目简称

            " isDiluent=" + QString::number( isDiluent ) +", "      //稀释否（是否需要稀释样本）
            " unit='"+ unit  +"', "                                 //项目单位ID
            " accuracy="+ QString::number( accuracy ) +", "         //结果精度

            " sample_V="+ QString::number( sample ) +", "      //样本量（ul）
            " r0_V="+ QString::number( diluent ) +", "         //探针吸R0量（ul）
            " r1_V="+ QString::number( r1 ) +", "              //探针吸稀释后的样本量（ul）样本转移量
            " rs_V="+ QString::number( transfer ) +", "        //探针吸R1量（ul）
            " r2_V="+ QString::number( r2 ) +", "              //探针吸R2量（ul）

            " lightPos1="+ QString::number( reactionCycle1 ) +", "       //测光点1
            " lightPos2="+ QString::number( reactionCycle2 ) +", "       //测光点2
            " K="+ QString::number( param_K ) +", "      //K因数值
            " lowLinear="+ QString::number( lowLinear ) +", "//余量报警（次数）
            " bloodFactor="+ QString::number( bloodFactor ) +", " //全血因子

            " calibrateID="+ QString::number( 0 ) +", "      //定标方式ID（定标方式表）
            " calibrate1_R0="+ QString::number( calibrate1_R0, 'f', 6 ) +", "    //定标参数
            " calibrate1_K="+ QString::number( calibrate1_K, 'f', 6 ) +", "      //定标参数
            " calibrate1_a="+ QString::number( calibrate1_a, 'f', 6 ) +", "      //定标参数
            " calibrate1_b="+ QString::number( calibrate1_b, 'f', 6 ) +", "      //定标参数
            " calibrate1_c="+ QString::number( calibrate1_c , 'f', 6) +", "       //定标参数
            " calibrate1_RR="+ QString::number( calibrate1_RR ) +", "    //定标参数
            " calibrate1_CC="+ QString::number( calibrate1_CC ) +", "    //定标参数

            " isCalibrate2=" + QString::number( isCalibrate2 ) +", "     //定标参数
            " calibrate2_R0="+ QString::number( calibrate2_R0, 'f', 6 ) +", "    //定标参数
            " calibrate2_K="+ QString::number( calibrate2_K, 'f', 6 ) +", "      //定标参数
            " calibrate2_a="+ QString::number( calibrate2_a, 'f', 6 ) +", "      //定标参数
            " calibrate2_b="+ QString::number( calibrate2_b, 'f', 6 ) +", "      //定标参数
            " calibrate2_c="+ QString::number( calibrate2_c, 'f', 6 ) +", "       //定标参数
            " calibrate2_RR="+ QString::number( calibrate2_RR ) +", "    //定标参数
            " calibrate2_CC="+ QString::number( calibrate2_CC ) +" "     //定标参数
            " where name ='" + name + "';";
    SQLite::getInstance()->Insert(str);

    //修改项目时更新GParameter参数类中的映射表
    gParameter->setProjectParam(id, UpdateProjectParamS(id));
}

//槽:更新读卡进度条
void ReadCareWidget::slt_ProgressBar_ValueChanged(int i)
{
    barReadingStatus->setValue(i);
}

//槽:是否启用定标参数2
void ReadCareWidget::slt_chkCalibrateParam_2_Check(int status)
{
    //项目定标参数2
    edit_R0_2->setVisible(status);
    edit_K_2->setVisible(status);
    edit_a_2->setVisible(status);
    edit_b_2->setVisible(status);
    edit_c_2->setVisible(status);
    editCalibrateParam_C_2->setVisible(status);
    editCalibrateParam_R_2->setVisible(status);

    lab_R0_2->setVisible(status);
    lab_K_2->setVisible(status);
    lab_a_2->setVisible(status);
    lab_b_2->setVisible(status);
    lab_c_2->setVisible(status);
    labCalibrateParam_C_2->setVisible(status);
    labCalibrateParam_R_2->setVisible(status);

    edit_R0_2->clear();
    edit_K_2->clear();
    edit_a_2->clear();
    edit_b_2->clear();
    edit_c_2->clear();
    editCalibrateParam_C_2->clear();
    editCalibrateParam_R_2->clear();
}

//槽:选择是否要稀释时，稀释液&转移量会有变化
void ReadCareWidget::slt_cboIsDiluent_IndexChanged(int index)
{
    if(index == 0) //非稀释项目 （不可用&隐藏）
    {
        editDiluent->setVisible(false);
        editS_Transfer->setVisible(false);
        labDiluent->setVisible(false);
        labS_Transfer->setVisible(false);
        editDiluent->clear();
        editS_Transfer->clear();
    }
    else //稀释项目  （可用&显示）
    {
        editDiluent->setVisible(true);
        editS_Transfer->setVisible(true);
        labDiluent->setVisible(true);
        labS_Transfer->setVisible(true);
    }
}


//新增项目时更新GParameter参数类中的映射表
ProjectParamS ReadCareWidget::InsertProjectParamS()
{
    //项目参数
    ProjectParamS param;
    param.Param_unit = editUnit->text().trimmed();               //项目单位  （是否有用，待定）
    param.Param_accuracy = cboAccuracy->currentText().toInt(); //项目精度
    param.Param_lowLinear = editLowLinear->text().toDouble();     //线性下限
    param.Param_factorK = editParam_K->text().toDouble();         //K因数值
    param.Param_bloodFactor = editBloodFactor->text().toDouble(); //全血因子（全血模式有效，非全血则为1）

    param.Param_lightPos1 = editCycle1->text().toInt();      //测光点1
    param.Param_lightPos2 = editCycle2->text().toInt();      //测光点2

    param.Param_R0 = edit_R0_1->text().trimmed().toDouble();  //参数R0
    param.Param_K = edit_K_1->text().trimmed().toDouble();    //参数K
    param.Param_a = edit_a_1->text().trimmed().toDouble();    //参数a
    param.Param_b = edit_b_1->text().trimmed().toDouble();    //参数b
    param.Param_c = edit_c_1->text().trimmed().toDouble();    //参数c
    param.Param_R = editCalibrateParam_R_1->text().trimmed().toDouble();    //参数-最大幅度
    param.Param_C = editCalibrateParam_C_1->text().trimmed().toDouble();    //参数-最大浓度

    //是否有第二段曲线
    if(chkCalibrateParam_2->isChecked())
    {
        param.isLogic5P2 = true;
        param.Param2_R0 = edit_R0_2->text().trimmed().toDouble();  //参数R0
        param.Param2_K = edit_K_2->text().trimmed().toDouble();    //参数K
        param.Param2_a = edit_a_2->text().trimmed().toDouble();    //参数a
        param.Param2_b = edit_b_2->text().trimmed().toDouble();    //参数b
        param.Param2_c = edit_c_2->text().trimmed().toDouble();    //参数c
        param.Param2_R = editCalibrateParam_R_2->text().trimmed().toDouble();    //参数-最大幅度
        param.Param2_C = editCalibrateParam_C_2->text().trimmed().toDouble();    //参数-最大浓度
    }
    else
    {
        param.isLogic5P2 = false;
        //以下参数正常情况下无实际用处，以防出错，（若没有第二段曲线，则第二段曲线的参数与第一段的参数一致）
        param.Param2_R0 = param.Param_R0;   //参数R0
        param.Param2_K = param.Param_K;     //参数K
        param.Param2_a = param.Param_a;     //参数a
        param.Param2_b = param.Param_b;     //参数b
        param.Param_c = param.Param_c;      //参数c
        param.Param2_R = param.Param_R;     //参数-最大幅度
        param.Param2_C = param.Param_C;     //参数-最大浓度
    }
    return param;
}

//修改项目时更新GParameter参数类中的映射表
ProjectParamS ReadCareWidget::UpdateProjectParamS(int id)
{
    //项目参数
    ProjectParamS param = gParameter->getProjectParam(id);
    param.Param_unit = editUnit->text().trimmed();               //项目单位  （是否有用，待定）
    param.Param_accuracy = cboAccuracy->currentText().toInt(); //项目精度
    param.Param_lowLinear = editLowLinear->text().toDouble();     //线性下限
    param.Param_factorK = editParam_K->text().toDouble();         //K因数值
    param.Param_bloodFactor = editBloodFactor->text().toDouble(); //全血因子（全血模式有效，非全血则为1）

    param.Param_lightPos1 = editCycle1->text().toInt();      //测光点1
    param.Param_lightPos2 = editCycle2->text().toInt();      //测光点2

    param.Param_R0 = edit_R0_1->text().trimmed().toDouble();  //参数R0
    param.Param_K = edit_K_1->text().trimmed().toDouble();    //参数K
    param.Param_a = edit_a_1->text().trimmed().toDouble();    //参数a
    param.Param_b = edit_b_1->text().trimmed().toDouble();    //参数b
    param.Param_c = edit_c_1->text().trimmed().toDouble();    //参数c
    param.Param_R = editCalibrateParam_R_1->text().trimmed().toDouble();    //参数-最大幅度
    param.Param_C = editCalibrateParam_C_1->text().trimmed().toDouble();    //参数-最大浓度

    //是否有第二段曲线
    if(chkCalibrateParam_2->isChecked())
    {
        param.isLogic5P2 = true;
        param.Param2_R0 = edit_R0_2->text().trimmed().toDouble();   //参数R0
        param.Param2_K = edit_K_2->text().trimmed().toDouble();    //参数K
        param.Param2_a = edit_a_2->text().trimmed().toDouble();    //参数a
        param.Param2_b = edit_b_2->text().trimmed().toDouble();    //参数b
        param.Param2_c = edit_c_2->text().trimmed().toDouble();    //参数c
        param.Param2_R = editCalibrateParam_R_2->text().trimmed().toDouble();    //参数-最大幅度
        param.Param2_C = editCalibrateParam_C_2->text().trimmed().toDouble();    //参数-最大浓度
    }
    else
    {
        param.isLogic5P2 = false;
    }

    return param;
}







