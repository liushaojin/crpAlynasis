#include "MaintainWidget.h"
#include "qdebug.h"

//实例对象
MaintainWidget *MaintainWidget::_instance = 0;
MaintainWidget * MaintainWidget::getInstance()
{
    if (_instance == 0)
        _instance = new MaintainWidget();
    return _instance;
}

MaintainWidget::MaintainWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void MaintainWidget::initForm()
{
    //数据库维护
    grbSQLMatain = new QGroupBox("数据库维护");
    labSQLTableName = new QLabel("表名:");     //表名
    cboSQLMatain = new QComboBox;     //数据表下拉列表
    btnDeleteTable = new QPushButton("删除"); //删除数据表
    btnRebuildSQL = new QPushButton("重检");  //重检数据库
    lytSQLMatain = new QGridLayout;
    lytSQLMatain->setMargin(12);
    lytSQLMatain->setSpacing(5);
    lytSQLMatain->addWidget(labSQLTableName,  0,  0, 2,  5, Qt::AlignRight);
    lytSQLMatain->addWidget(cboSQLMatain,     0,  5, 2, 10);
    lytSQLMatain->addWidget(btnDeleteTable,   0, 15, 2,  5, Qt::AlignCenter);
    lytSQLMatain->addWidget(btnRebuildSQL,    0, 20, 2,  5, Qt::AlignCenter);
    grbSQLMatain->setLayout(lytSQLMatain);

    //针盘维护
    grbMaintain = new QGroupBox("针、盘维护");
    chkMaintainProbe = new QCheckBox("针维护");        //加样针维护
    labMaintainLiquid = new QLabel("清洗液位置:");          //针维护清洗液位置
    cboMatainLiquidPos = new QComboBox;
    labProbeMaintainVol = new QLabel("清洗量(ml):");        //注射器吸清洗液的量
    editProbeMaintainVol = new QLineEdit;
    labProbeMaintainTimes = new QLabel("维护次数:");      //维护次数
    editProbeMaintainTimes = new QLineEdit;
    chkMaintainReactionDisk = new QCheckBox("反应盘维护"); //反应盘维护
    labRDiskMaintainTime = new QLabel("浸泡时间(min):");       //加完清洗液后浸泡的时间
    editRDiskMaintainTime = new QLineEdit;
    labRDiskMaintainVol = new QLabel("清洗液的量:");        //反应盘吸清洗液的量
    editRDiskMaintainVol = new QLineEdit;
    btnStartMaintain = new QPushButton("开始维护");      //开始维护
    lytMaintain = new QGridLayout;
    lytMaintain->setMargin(12);
    lytMaintain->setSpacing(10);
    lytMaintain->addWidget(chkMaintainProbe,         0,  0, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(labMaintainLiquid,        0,  5, 2, 5, Qt::AlignRight);
    lytMaintain->addWidget(cboMatainLiquidPos,       0, 10, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(labProbeMaintainVol,      0, 15, 2, 5, Qt::AlignRight);
    lytMaintain->addWidget(editProbeMaintainVol,     0, 20, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(labProbeMaintainTimes,    0, 25, 2, 5, Qt::AlignRight);
    lytMaintain->addWidget(editProbeMaintainTimes,   0, 30, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(chkMaintainReactionDisk,  2,  0, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(labRDiskMaintainTime,     2,  5, 2, 5, Qt::AlignRight);
    lytMaintain->addWidget(editRDiskMaintainTime,    2, 10, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(labRDiskMaintainVol,      2, 15, 2, 5, Qt::AlignRight);
    lytMaintain->addWidget(editRDiskMaintainVol,     2, 20, 2, 5, Qt::AlignLeft);
    lytMaintain->addWidget(btnStartMaintain,         0, 35, 4, 5, Qt::AlignCenter);
    grbMaintain->setLayout(lytMaintain);

    digMaintainBar = new QDialog;         //针、盘维护进度条提示窗
    barReadingStatus = new QProgressBar; //进度条
    labTime = new QLabel("维护剩余时间(min):");                 //剩余时间
    labTime1 = new QLabel;
    lytMaintainBar = new QGridLayout;
    lytMaintainBar->setMargin(15);
    lytMaintainBar->setSpacing(10);
    lytMaintainBar->addWidget(labTime,  0,  0, 1, 15, Qt::AlignRight);
    lytMaintainBar->addWidget(labTime1, 0, 15, 1, 15, Qt::AlignLeft);
    lytMaintainBar->addWidget(barReadingStatus, 2,  0, 2, 30);
    digMaintainBar->setLayout(lytMaintainBar);
    barReadingStatus->setRange(0, 0);
    digMaintainBar->setModal(true);

    //主布局
    layout = new QGridLayout;
    layout->setSpacing(20);
    layout->setMargin(5);
    layout->addWidget(grbMaintain,  10, 0, Qt::AlignVCenter | Qt::AlignLeft);

    if( !gParameter->get_isRelease() ) //0:研发版，即供公司内部使用。1：发布版，即供客户端使用。
        layout->addWidget(grbSQLMatain, 30, 0, Qt::AlignVCenter | Qt::AlignLeft);
    this->setLayout(layout);
}

//界面风格
void MaintainWidget::initStyle()
{
    //
    btnDeleteTable->setObjectName("button6");
    btnRebuildSQL->setObjectName("button6");
    btnStartMaintain->setObjectName("button5");

    //进度条窗体样式设置
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    QString style = qss.readAll();
    digMaintainBar->setStyleSheet(style);
    qss.close();
    labTime->setObjectName("lab25");
    labTime1->setObjectName("lab26");
    digMaintainBar->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    digMaintainBar->setFixedWidth(680);

    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

}

//连接信号与槽
void MaintainWidget::initConnect()
{
    connect(btnDeleteTable, SIGNAL(clicked()), this, SLOT(slt_btnDeleteTable_Clicked()));
    connect(btnRebuildSQL, SIGNAL(clicked()), this, SLOT(slt_btnRebuildSQL_Clicked()));
    connect(btnStartMaintain, SIGNAL(clicked()), this, SLOT(slt_btnStartMaintain_Clicked()));

    connect(gScheduler, SIGNAL(sig_FinishedMaintain(int)), this, SLOT(slt_finishedMaintain(int)));
}

//初始化数据
void MaintainWidget::initData()
{
    //数据库维护
    QStringList List;
    QString str  = "select name from sqlite_master where type='table' order by name;";     //数据库里所有的表名
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())
        List.append(query.value(0).toString());
    cboSQLMatain->addItems(List);
    cboSQLMatain->removeItem( List.count()-1 ); //移除最后一项目“sqlite_sequence”

    //针、盘维护
    QStringList posList;
    posList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    cboMatainLiquidPos->addItems(posList);
    editProbeMaintainTimes->setPlaceholderText("1~5");
    editProbeMaintainVol->setPlaceholderText("50~450");
    editRDiskMaintainTime->setPlaceholderText("2~10");
    editRDiskMaintainVol->setPlaceholderText("300~400");
}

//槽：删除数据表
void MaintainWidget::slt_btnDeleteTable_Clicked()
{
    QString table = cboSQLMatain->currentText().trimmed();
    SQLite::getInstance()->DropTable( table );
}
//槽：重检数据库
void MaintainWidget::slt_btnRebuildSQL_Clicked()
{
    SQLite::getInstance()->initCheckDB();
}

//槽：开始维护
void MaintainWidget::slt_btnStartMaintain_Clicked()
{
    gMessageBox->setInfo(QString("提示"), QString("'维护'功能尚未开放!!"),
                         QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
    gMessageBox->setButtonText(Cancel, "返回");
    gMessageBox->show();
    return;


    ////以下为MV100程序
    //    bool isProbeMaintain = chkMaintainProbe->isChecked();
    //    bool isReactionDiskMaintain = chkMaintainReactionDisk->isChecked();

    //    int liquidPos = cboMatainLiquidPos->currentText().toInt();
    //    int probeVol = editProbeMaintainVol->text().trimmed().toInt();
    //    int pMaintainT = editProbeMaintainTimes->text().trimmed().toInt();
    //    int rMaintainTime = editRDiskMaintainTime->text().trimmed().toInt();
    //    int rVol = editRDiskMaintainVol->text().trimmed().toInt();

    //    //默认参数
    //    if(probeVol == 0) probeVol = 200;
    //    if(pMaintainT == 0) pMaintainT = 1;
    //    if(rMaintainTime == 0) rMaintainTime =2;
    //    if(rVol == 0) rVol == 350;

    //    if(isProbeMaintain || isReactionDiskMaintain) //两者都进行维护 || 单独维护加样针 || 单独维护反应盘
    //    {
    //        //MV100工作状态（0：空闲，1：普通测试，2：质控测试，3：定标测试，4：开机复位, 5：开机未自检, 6：暂停， 7：停止, 8:无法继续工作[前条指令无应答/结果返回]）
    //        int Status = gScheduler->getMV100_WorkStatus();
    //        if(Status == 0)
    //        {
    //            labTime->setText("维护中，请等待......");
    //            digMaintainBar->show();
    //            gScheduler->SysMaintain(isProbeMaintain, liquidPos, probeVol, pMaintainT, isReactionDiskMaintain, rMaintainTime, rVol);
    //        }
    //        else
    //        {
    //            gMessageBox->setInfo(QString("提示"), QString("\n系统处于“开机未自检”“暂停”“停止”等状态下无法进行维护!"),
    //                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
    //            gMessageBox->setButtonText(Cancel, "返回");
    //            if(gMessageBox->exec() == MVMessageBox::Rejected)
    //                return;

    //        }
    //    }
    //    else //没有选中要维护的机构，提示
    //    {
    //        gMessageBox->setInfo(QString("提示"), QString("\n请选择将要维护的机构！"),
    //                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
    //        gMessageBox->setButtonText(Cancel, "确定");
    //        if(gMessageBox->exec() == MVMessageBox::Accepted)
    //            return;
    //    }
}

//槽：更新进度条的状态
void MaintainWidget::slt_UpdateProgressBar(int s)
{
    //    barReadingStatus->setValue(s);
}

//槽：完成维护或者维护过程中出错(type=0：完成维护，type=1：维护出错)
void MaintainWidget::slt_finishedMaintain(int type)
{
    qDebug() << "slt_finishedMaintain(int type)" << type << "完成维护或者维护过程中出错(type=0：完成维护，type=1：维护出错)......";
    digMaintainBar->close();

    if(type == 1)
    {
        gScheduler->setWorkModeFlag(MV_State_unChecked);  //未自检

        gMessageBox->setInfo(QString("提示"), QString("\n维护过程中出错！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
    }
}


