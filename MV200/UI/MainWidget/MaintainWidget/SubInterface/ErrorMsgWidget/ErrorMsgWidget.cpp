#include "ErrorMsgWidget.h"

ErrorMsgWidget *ErrorMsgWidget::_instance = 0;
ErrorMsgWidget * ErrorMsgWidget::getInstance()
{
    if (_instance == 0)
        _instance = new ErrorMsgWidget();
    return _instance;
}

ErrorMsgWidget::ErrorMsgWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();              //初始化
    initTableFormat();       //格式化表格
    initStyle();             //界面风格
    initData();              //初始化数据
    initConnect();           //连接信号与槽
}

//初始化
void ErrorMsgWidget::initForm()
{
    termsWidget = new TermsWidget;
    termsWidget->setModal(true);  //使用模态窗口

    tbvList = new QTableView;            //列表
    modelList = new QStandardItemModel;  //列表模型
    tbvList->setModel(modelList);

    btnQueryTerms = new QPushButton("查询条件");     //查询条件
    btnReQuery = new QPushButton("重新查询");        //重新查询
    btnFormer = new QPushButton("上一页");           //上一页
    btnNext = new QPushButton("下一页");             //下一页
    btnBack = new QPushButton("返 回");              //返回

    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(5);
    layout->addWidget(tbvList,        0,  0, 50, 80);
    //layout->addWidget(btnQueryTerms, 50,  0,  5,  10);   // 20170619 hituck
    layout->addWidget(btnReQuery,    50, 10,  5,  10);
    layout->addWidget(btnFormer,     50, 20,  5,  10);
    layout->addWidget(btnNext,       50, 30,  5,  10);
    //layout->addWidget(btnBack,       50, 40,  5, 10);
    this->setLayout(layout);
}
//格式化表格
void ErrorMsgWidget::initTableFormat()
{
    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "id" << "时间" << "错误id" << "错误号" << "描述" << "";
    modelList->setHorizontalHeaderLabels( headerList_h );
    tbvList->hideColumn(0);             //id-隐藏
    tbvList->setColumnWidth( 1, 180);   //时间
    tbvList->setColumnWidth( 2, 100);   //错误id
    tbvList->setColumnWidth( 3, 100);   //错误号
    tbvList->setColumnWidth( 4, 250);   //描述

    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        modelList->setItem( i, 0, new QStandardItem( "" ));  //id(隐藏)
        modelList->setItem( i, 1, new QStandardItem( "" ));  //时间
        modelList->setItem( i, 2, new QStandardItem( "" ));  //错误id
        modelList->setItem( i, 3, new QStandardItem( "" ));  //错误号
        modelList->setItem( i, 4, new QStandardItem( "" ));  //描述
        modelList->setItem( i, 5, new QStandardItem( "" ));

        //列表内容显示方式
        modelList->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
    }
}
//界面风格
void ErrorMsgWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);


    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll() );
    qss.close();

    btnQueryTerms->setObjectName("button6");     //查询条件
    btnReQuery->setObjectName("button6");        //重新查询
    btnFormer->setObjectName("button6");         //上一页
    btnNext->setObjectName("button6");           //下一页
    btnBack->setObjectName("button6");           //返回

    this->setMinimumSize(800, 700);
}
//连接信号与槽
void ErrorMsgWidget::initConnect()
{
    connect(btnFormer, SIGNAL(clicked()), this, SLOT(slt_btnFormer_Clicked()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slt_btnNext_Clicked()));
    connect(btnReQuery, SIGNAL(clicked()), this, SLOT(slt_btnReQuery_Clicked()));
    connect(btnQueryTerms, SIGNAL(clicked()), this, SLOT(slt_btnQueryTerms_Clicked()));
    connect(termsWidget, SIGNAL(sig_QueryTerms_Change(QString)), this, SLOT(slt_ChangeQueryTerms(QString)));
}

//初始化数据
void ErrorMsgWidget::initData()
{
    Count = 0;
    queryTerms = ""; //查询条件
    QueryErrorMsg(); //查询错误信息
}

//查询错误信息
int ErrorMsgWidget::QueryErrorMsg()
{
    int flag = 0;
    //从数据库中取出用户申请的测试信息，并存入循环队列，等待执行
    QString str  =
            //
            "select id, dateTime, errID, errNo, details from ErrorT "
            + queryTerms +
            " order by id desc "
            " limit " + QString::number(Count) +", " + QString::number(ROWCOUNT)
            +";";

    //qDebug() << str;
    //清空数据
    for( int i = 0; i < ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setText("");  //id(隐藏)
        modelList->item(i, 1)->setText("");  //时间
        modelList->item(i, 2)->setText("");  //错误id
        modelList->item(i, 3)->setText("");  //错误号
        modelList->item(i, 4)->setText("");  //描述
    }

    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        modelList->item(i, 0)->setText(query.value(0).toString());  //id(隐藏)
        modelList->item(i, 1)->setText(query.value(1).toString());  //时间
        modelList->item(i, 2)->setText(query.value(2).toString());  //错误id
        modelList->item(i, 3)->setText(query.value(3).toString());  //错误号
        modelList->item(i, 4)->setText(query.value(4).toString());  //描述

        i++;
        flag = 1;
    }
    return flag;
}

//槽：改变查询条件
void ErrorMsgWidget::slt_ChangeQueryTerms(QString term)
{
    //qDebug() << term;
    Count = 0;
    queryTerms = term;
}
//槽：查询条件
void ErrorMsgWidget::slt_btnQueryTerms_Clicked()
{
    termsWidget->show();
}
//槽：重新查询
void ErrorMsgWidget::slt_btnReQuery_Clicked()
{
    QueryErrorMsg();
}
//槽：上一页
void ErrorMsgWidget::slt_btnFormer_Clicked()
{
    if(Count > 0)
    {
        Count -= ROWCOUNT;
        if( ! QueryErrorMsg() )//查询不成功，还原‘查询行数标志’
        {
            Count += ROWCOUNT;
            QueryErrorMsg();
        }
    }
}
//槽：下一页
void ErrorMsgWidget::slt_btnNext_Clicked()
{
    Count += ROWCOUNT;
    if( ! QueryErrorMsg() )
    {
        Count -= ROWCOUNT;
        QueryErrorMsg();
    }
}

