#include "UserManagementWidget.h"

//实例对象
UserManagementWidget *UserManagementWidget::_instance = 0;
UserManagementWidget * UserManagementWidget::getInstance()
{
    if (_instance == 0)
        _instance = new UserManagementWidget();
    return _instance;
}

UserManagementWidget::UserManagementWidget(QWidget *parent) : QWidget(parent)
{
    initForm();              //初始化
    initTableFormat();       //初始化用户列表
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//初始化
void UserManagementWidget::initForm()
{
    tbvList = new QTableView;
    btnAllNone = new QPushButton(QObject::tr("全选/反选"));
    btnDelect = new QPushButton(QObject::tr("删除"));
    btnAddNew = new QPushButton(QObject::tr("新增"));
    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(btnAllNone,  0,  0,  5, 10, Qt::AlignCenter);
    layout->addWidget(btnDelect,   0, 10,  5, 10, Qt::AlignCenter);
    layout->addWidget(btnAddNew,   0, 20,  5, 10, Qt::AlignCenter);
    //layout->addWidget(btnDelect,   0, 30,  5, 10, Qt::AlignCenter);
    layout->addWidget(tbvList,     5,  0, 100, 60);
    setLayout(layout);
}

//初始化用户列表
void UserManagementWidget::initTableFormat()
{
    ModelList = new QStandardItemModel;
    //列表
    QStringList headerList_h;
    headerList_h << "" << "id" << QObject::tr("用户名") << QObject::tr("用户属性") << "";
    ModelList->setHorizontalHeaderLabels( headerList_h );
    tbvList->setModel(ModelList);
    int row = 0;
    tbvList->setColumnWidth( row ++, 30);    //选择列
    tbvList->hideColumn(row ++);             //id-隐藏
    tbvList->setColumnWidth( row ++, 180);   //用户名
    tbvList->setColumnWidth( row ++, 200);   //用户属性
    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    //tbvList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item = 0;
    //初始化每一个单元格
    for( int i = 0; i < MAX_USER_COUNT; i ++ )
    {
        item = new QStandardItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        ModelList->setItem( i, 0, item);
        ModelList->setItem( i, 1, new QStandardItem( "" ));  //id(隐藏)
        ModelList->setItem( i, 2, new QStandardItem( "" ));  //
        ModelList->setItem( i, 3, new QStandardItem( "" ));  //
        ModelList->setItem( i, 4, new QStandardItem( "" ));  //

        //列表内容显示方式
        ModelList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ModelList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
    }
}

//界面风格
void UserManagementWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    this->setWindowTitle(QObject::tr("用户管理"));

    QFile  qss(":/QSS/QSS/style3.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    btnAddNew->setFixedSize(120, 40);
    btnDelect->setFixedSize(120, 40);
    btnAllNone->setFixedSize(120, 40);
    this->setFixedHeight(600);
}

//连接信号与槽
void UserManagementWidget::initConnect()
{
    connect(btnAllNone, SIGNAL(clicked(bool)), this, SLOT(slt_btnAllNone_Clicked()));
    connect(btnDelect, SIGNAL(clicked(bool)), this, SLOT(slt_btnDelect_Clicked()));
    connect(btnAddNew, SIGNAL(clicked(bool)), this, SLOT(slt_btnAddNew_Clicked()));
    connect(AddNewUserWidget::getInstance(), SIGNAL(sig_AddNewUser()), this, SLOT(slt_UpdateUserData()));
}

//初始化数据
void UserManagementWidget::initData()
{
    isAllNull_Flag = false;

    SelectAllUser();
}

//查看所有用户
void UserManagementWidget::SelectAllUser()
{
    QString str = "select id, name, pwd from UserT where id != 1;";
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库

    int i = 0;
    while(query.next())
    {
        if(i >= MAX_USER_COUNT) break;

        ModelList->item(i, 1)->setText(query.value(0).toString());  //id(隐藏)
        ModelList->item(i, 2)->setText(query.value(1).toString());  //用户名
        ModelList->item(i, 3)->setText("一般用户");  //
        i ++;
    }

    for(i; i < ModelList->rowCount(); i ++)
    {
        ModelList->item(i, 1)->setText("");
        ModelList->item(i, 2)->setText("");
        ModelList->item(i, 3)->setText("");
    }
}

//槽：全选/反选
void UserManagementWidget::slt_btnAllNone_Clicked()
{
    for(int i = 0; i < 60; i ++ )
    {
        if(ModelList->item( i, 1)->text().toInt() > 0)
        {
            if (isAllNull_Flag)              //全选/反选标志
                ModelList->item( i, 0)->setCheckState(Qt::Unchecked);
            else
                ModelList->item( i, 0)->setCheckState(Qt::Checked);
        }
    }
    isAllNull_Flag = !isAllNull_Flag;
}

//槽：删除
void UserManagementWidget::slt_btnDelect_Clicked()
{
    QString str;
    QStringList list;
    list.clear();
    for(int i = 0; i < ModelList->rowCount(); i ++)
    {
        if (ModelList->item(i, 0)->checkState()) //选中状态
        {
            str = "delete from UserT where id=" + ModelList->item(i, 1)->text() + ";";
            list.append(str);
            ModelList->item(i, 0)->setCheckState(Qt::Unchecked);
            ModelList->item(i, 1)->setText("");
            ModelList->item(i, 2)->setText("");
            ModelList->item(i, 3)->setText("");
        }
    }
    SQLite::getInstance()->Transaction(list);
}

//槽：新增
void UserManagementWidget::slt_btnAddNew_Clicked() {AddNewUserWidget::getInstance()->show();}

//槽：更新用户信息
void UserManagementWidget::slt_UpdateUserData()
{
    SelectAllUser();
}
