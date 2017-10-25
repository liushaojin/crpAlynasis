#include "modifyResultWidget.h"
#include "QDebug"

ModifyResultWidget::ModifyResultWidget() :
    QDialog()
{
    initForm();                           //初始化界面控件
    initLanguage();                       //语言设置
    initStyle();                          //初始化样式
    initData();                           //初始化数据
    initConnect();                        //连接信号与槽
}

void ModifyResultWidget::setList(QList<QStringList> list)
{
    idList.clear();
    SampleTypeList.clear();
    //修改的信息(ID、编号、姓名、结果)
    int i = 0;
    for(i; i < ROWCOUNT; i ++)
    {
        if(i < list.count())
        {
            idList.append(list.at(i).at(0));
            labNo[i]->setText(list.at(i).at(1));
            labProject[i]->setText(list.at(i).at(2));
            labName[i]->setText(list.at(i).at(3));
            labResult[i]->setText(list.at(i).at(4));
            SampleTypeList.append(list.at(i).at(5));

            labNo[i]->setVisible(true);
            labProject[i]->setVisible(true);
            labName[i]->setVisible(true);
            labResult[i]->setVisible(true);
            edit[i]->setVisible(true);
        }
        else
        {
            labProject[i]->setVisible(false);
            labNo[i]->setVisible(false);
            labName[i]->setVisible(false);
            labResult[i]->setVisible(false);
            edit[i]->setVisible(false);
            edit[i]->clear();
        }
    }
}

//语言设置
void ModifyResultWidget::initLanguage()
{
    QString str = "样本编号+项目+结果+反应幅度+输入正确幅度";
    labTitle->setText("结果修改");
    labNo0->setText(str.split("+").at(0));
    labProject0->setText(str.split("+").at(1));
    labName0->setText(str.split("+").at(2));
    labResult0->setText(str.split("+").at(3));
    labModifyResult0->setText(str.split("+").at(4));

    btnCancel->setText("取 消");
    btnSure->setText("确 定");
}

//初始化界面控件
void ModifyResultWidget::initForm()
{
    //输入内容格式-正则表达式（只能输入数字和小数点,即double类形）
    QRegExp rx("-?\\d+(\\.\\d+)");
    QRegExpValidator *Reg = new QRegExpValidator(rx, this);

    labTitle = new QLabel;
    labNo0 = new QLabel;          //
    labProject0 = new QLabel;        //
    labName0 = new QLabel;        //
    labResult0 = new QLabel;      //
    labModifyResult0 = new QLabel;//
    for(int i = 0; i < ROWCOUNT; i ++)
    {
        labProject[i] = new QLabel;
        labNo[i] = new QLabel;            //
        labName[i] = new QLabel;        //
        labResult[i] = new QLabel;    //
        edit[i] = new QLineEdit;                        //
        edit[i]->setValidator(Reg);

        labProject[i]->setVisible(false);
        labNo[i]->setVisible(false);
        labName[i]->setVisible(false);
        labResult[i]->setVisible(false);
        edit[i]->setVisible(false);
    }
    btnSure = new QPushButton;
    btnCancel = new QPushButton;
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(labTitle,           0,  0,  3, 40, Qt::AlignCenter);
    layout->addWidget(labNo0,             5,  0,  1, 10, Qt::AlignCenter);
    layout->addWidget(labProject0,       5, 10,  1, 10, Qt::AlignCenter);
    layout->addWidget(labName0,           5, 20,  1, 10, Qt::AlignCenter);
    layout->addWidget(labResult0,         5, 30,  1, 10, Qt::AlignCenter);
    layout->addWidget(labModifyResult0,   5, 40,  1, 10, Qt::AlignCenter);
    for(int i = 0; i < ROWCOUNT; i++)
    {
        layout->addWidget(labNo[i],     i+6,  0,  1, 10, Qt::AlignCenter);
        layout->addWidget(labProject[i],i+6, 10,  1, 10, Qt::AlignCenter);
        layout->addWidget(labName[i],   i+6, 20,  1, 10, Qt::AlignCenter);
        layout->addWidget(labResult[i], i+6, 30,  1, 10, Qt::AlignCenter);
        layout->addWidget(edit[i],      i+6, 40,  1, 10, Qt::AlignCenter);
    }
    layout->addWidget(btnCancel,         30, 20,  3, 10);
    layout->addWidget(btnSure,           30, 30,  3, 10);
    this->setLayout(layout);
}

//初始化样式
void ModifyResultWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
}

//初始化数据
void ModifyResultWidget::initData()
{

}

//连接信号与槽
void ModifyResultWidget::initConnect()
{
    connect(btnSure, SIGNAL(clicked()), this, SLOT(slt_btnSure_clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_clicked()));
}

//
void ModifyResultWidget::slt_btnSure_clicked()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString Date = time.toString("yyyy-MM-dd hh-mm-ss");//设置显示格式
    QStringList list;
    list.clear();

    QString text;
    double RR;
    for(int i = 0; i < ROWCOUNT; i ++)
    {
        if (i == idList.count() ) break;
        text = edit[i]->text().trimmed();
        if (text != "")
        {
            int p_id = gParameter->getProject_id(labProject[i]->text());
            RR = ReCalculateResult(p_id, text.toInt(), SampleTypeList.at(i).toInt() );
            //保存数据到数据库
            QString str =
                    "update TempHistoryT set "
                    " amplitude= " + text + ", "
                    + " result = " + QString::number(RR) + ", "
                    +" modifierID= " + QString::number(gParameter->getUserID()) +", "
                    +" modifyDate= '" + Date + "' "
                    +" where id = " + idList.at(i) + ";" ;
            //qDebug() << str;
            //SQLite::getInstance()->Update(str);
            list.append(str);
            edit[i]->clear();
        }
    }
    SQLite::getInstance()->Transaction(list);

    emit sig_ModifyResult();        //完成修改
    this->close();
}

void ModifyResultWidget::slt_btnCancel_clicked()
{
    this->close();
}

//重新计算幅度和结果
double ModifyResultWidget::ReCalculateResult(int projectID, int AD, int sampleType)
{
    //logic5P参数
    long double R0, K, a, b, c;
    double C;

    ProjectParamS pParam = gParameter->getProjectParam(projectID);
    //用第1段参数曲线计算(默认)
    R0 = pParam.Param_R0;
    K = pParam.Param_K;
    a = pParam.Param_a;
    b  = pParam.Param_b;
    c = pParam.Param_c;
    C = pParam.Param_C;

    //用第2段参数曲线计算
    if( AD > pParam.Param_R)
    {
        if(pParam.isLogic5P2)
        {
            R0 = pParam.Param2_R0;
            K = pParam.Param2_K;
            a = pParam.Param2_a;
            b = pParam.Param2_b;
            c = pParam.Param2_c;
            C = pParam.Param2_C;
        }
    }

    //
    double bloodFactor = pParam.Param_bloodFactor; //全血因子
    if( bloodFactor <= 0 ) bloodFactor = 1;
    if( sampleType != 1 ) bloodFactor = 1; //sampleType=1，时表示样本类型为全血

    //计算浓度
    //查看用哪种定标方式计算(目前都用LOGIC5P)
    MV_Logic5P logic5P;
    double result = logic5P.Consistence_Logic5P(0, C, R0, K, a, b, c, AD) * pParam.Param_factorK / bloodFactor;

    //项目线性下限
    if( result < pParam.Param_lowLinear)
        result = pParam.Param_lowLinear;

    return QString::number(result, 'f', pParam.Param_accuracy).toDouble();
}











