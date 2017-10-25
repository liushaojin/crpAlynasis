#include "TestOrder.h"
#include <QVBoxLayout>
#include <QFile>
#include "Object/SQLite/sqlite.h"
TestOrder::TestOrder(QWidget *parent) : QWidget(parent)
{
    initForm();              //初始化
    initProjectList();       //常规项目列表
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//查询项目数据
void TestOrder::QueryProjectInfo()
{
    for(int i = 0; i < modelPorject->rowCount(); i ++)
        modelPorject->removeRow(i);

    QString str  = " select id, name, orderN from ProjectT order by orderN;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    int id, row = 0;
    while (query.next())
    {
        id = query.value(0).toInt();
        if(id > 0)
        {
            modelPorject->setItem( row, 0, new QStandardItem( QString::number(id) ));  //id(隐藏)
            modelPorject->setItem( row, 1, new QStandardItem( query.value(1).toString() ));  //
            modelPorject->setItem( row, 2, new QStandardItem( QString::number(row+1) ));  //
            modelPorject->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            modelPorject->item(row, 2)->setTextAlignment(Qt::AlignCenter);

            row ++;
        }
    }
}

//初始化
void TestOrder::initForm()
{
    tbvProject = new QTableView;
    modelPorject = new QStandardItemModel;
    tbvProject->setModel(modelPorject);

    btnTop = new QPushButton("置顶");       //置顶
    btnUp = new QPushButton("上移");        //上移
    btnDown = new QPushButton("下移");      //下移
    btnBottom = new QPushButton("底部");    //底部
    btnCancel = new QPushButton("取消");    //取消
    btnSave = new QPushButton("保存");      //保存
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setMargin(5);
    vlayout->setSpacing(10);
    vlayout->addWidget(btnTop);
    vlayout->addWidget(btnUp);
    vlayout->addWidget(btnDown);
    vlayout->addWidget(btnBottom);
    vlayout->addWidget(btnCancel);
    vlayout->addWidget(btnSave);

    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(tbvProject, 0,  0, 30, 90);
    layout->addLayout(vlayout,    0, 90, 30, 10, Qt::AlignVCenter);
    this->setLayout(layout);

}

//常规项目列表
void TestOrder::initProjectList()
{
    //列表
    QStringList headerList_h;
    headerList_h << "id" << "项目" << "顺序" << "";
    modelPorject->clear();
    modelPorject->setHorizontalHeaderLabels( headerList_h );
    int row = 0;
    tbvProject->hideColumn(row ++);             //id-隐藏
    tbvProject->setColumnWidth( row ++, 200);   //项目名称
    tbvProject->setColumnWidth( row ++, 100);   //项目成员
    tbvProject->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvProject->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvProject->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvProject->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvProject->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvProject->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvProject->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式
}

//界面风格
void TestOrder::initStyle()
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setMinimumSize(640, 600);
    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    QString ss = qss.readAll();
    setStyleSheet( ss );
    qss.close();

    btnTop->setObjectName("button6");
    btnUp->setObjectName("button6");
    btnDown->setObjectName("button6");
    btnBottom->setObjectName("button6");
    btnCancel->setObjectName("button6");
    btnSave->setObjectName("button6");

}

//连接信号与槽
void TestOrder::initConnect()
{
    connect(btnTop, SIGNAL(clicked(bool)), this, SLOT(slt_btnTop_Clicked()));
    connect(btnUp, SIGNAL(clicked(bool)), this, SLOT(slt_btnUp_Clicked()));
    connect(btnDown, SIGNAL(clicked(bool)), this, SLOT(slt_btnDown_Clicked()));
    connect(btnBottom, SIGNAL(clicked(bool)), this, SLOT(slt_btnBottom_Clicked()));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(slt_btnSave_Clicked()));
}

//初始化数据
void TestOrder::initData()
{

}

//槽：置顶
void TestOrder::slt_btnTop_Clicked()
{
    QModelIndex index = tbvProject->currentIndex();
    if( !index.isValid() ) return;
    if( index.row() == 0 ) return;

    //要置顶的项目id、名称
    QString id0 = modelPorject->item( index.row(), 0)->text().trimmed();
    QString name0 = modelPorject->item( index.row(), 1)->text().trimmed();

    QString id, name;
    for(int i = index.row(); i > 0; i --)
    {
        id = modelPorject->item( i-1, 0)->text().trimmed();
        name = modelPorject->item( i-1, 1)->text().trimmed();

        modelPorject->item(i, 0)->setText(id);
        modelPorject->item(i, 1)->setText(name);
    }

    modelPorject->item(0, 0)->setText(id0);
    modelPorject->item(0, 1)->setText(name0);

    QModelIndex cIndex = modelPorject->index(0, 1);
    tbvProject->setCurrentIndex(cIndex);
}
//槽：上移
void TestOrder::slt_btnUp_Clicked()
{
    QModelIndex index = tbvProject->currentIndex();
    if( !index.isValid() ) return;
    if( index.row() == 0 ) return;

    //项目id、名称
    QString id0 = modelPorject->item( index.row()-1, 0)->text().trimmed();
    QString name0 = modelPorject->item( index.row()-1, 1)->text().trimmed();
    QString id = modelPorject->item( index.row(), 0)->text().trimmed();
    QString name = modelPorject->item(index.row(), 1)->text().trimmed();

    modelPorject->item(index.row()-1, 0)->setText(id);
    modelPorject->item(index.row()-1, 1)->setText(name);
    modelPorject->item(index.row(), 0)->setText(id0);
    modelPorject->item(index.row(), 1)->setText(name0);

    QModelIndex cIndex = modelPorject->index(index.row()-1, 1);
    tbvProject->setCurrentIndex(cIndex);
}
//槽：下移
void TestOrder::slt_btnDown_Clicked()
{
    QModelIndex index = tbvProject->currentIndex();
    if( !index.isValid() ) return;
    if( index.row() == modelPorject->rowCount()-1 ) return;

    //项目id、名称
    QString id0 = modelPorject->item( index.row()+1, 0)->text().trimmed();
    QString name0 = modelPorject->item( index.row()+1, 1)->text().trimmed();
    QString id = modelPorject->item( index.row(), 0)->text().trimmed();
    QString name = modelPorject->item(index.row(), 1)->text().trimmed();

    modelPorject->item(index.row()+1, 0)->setText(id);
    modelPorject->item(index.row()+1, 1)->setText(name);
    modelPorject->item(index.row(), 0)->setText(id0);
    modelPorject->item(index.row(), 1)->setText(name0);

    QModelIndex cIndex = modelPorject->index(index.row()+1, 1);
    tbvProject->setCurrentIndex(cIndex);
}
//槽：底部
void TestOrder::slt_btnBottom_Clicked()
{
    QModelIndex index = tbvProject->currentIndex();
    if( !index.isValid() ) return;
    if( index.row() == modelPorject->rowCount()-1 ) return;

    //要置底的项目id、名称
    QString id0 = modelPorject->item( index.row(), 0)->text().trimmed();
    QString name0 = modelPorject->item( index.row(), 1)->text().trimmed();

    QString id, name;
    for(int i = index.row()+1; i < modelPorject->rowCount(); i ++)
    {
        id = modelPorject->item( i, 0)->text().trimmed();
        name = modelPorject->item( i, 1)->text().trimmed();

        modelPorject->item(i-1, 0)->setText(id);
        modelPorject->item(i-1, 1)->setText(name);
    }

    modelPorject->item(modelPorject->rowCount()-1, 0)->setText(id0);
    modelPorject->item(modelPorject->rowCount()-1, 1)->setText(name0);

    QModelIndex cIndex = modelPorject->index(modelPorject->rowCount()-1, 1);
    tbvProject->setCurrentIndex(cIndex);
}
//槽：取消
void TestOrder::slt_btnCancel_Clicked()
{
    QueryProjectInfo();      //查询项目数据
}
//槽：保存
void TestOrder::slt_btnSave_Clicked()
{
    QStringList list;
    QString str;
    QString id, orderN;
    for(int i = 0; i < modelPorject->rowCount(); i ++)
    {
        id = modelPorject->item(i, 0)->text().trimmed();
        orderN = modelPorject->item(i, 2)->text().trimmed();
        str = "update ProjectT set orderN=" + orderN + " where id =" + id + ";";
        list.append(str);
    }
    if(list.count() > 0)
    {
        SQLite::getInstance()->Transaction(list);

        gMessageBox->setInfo(QString("保存提示"), QString("保存成功"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "确定");
        gMessageBox->show();
    }
}















