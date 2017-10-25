#include "QCSettingWidget.h"
#include "Thread/Scheduler.h"
#include <QFile>
#include <qdebug.h>

//本类单例对象
QCSettingWidget *QCSettingWidget::_instance = 0;
QCSettingWidget * QCSettingWidget::getInstance()
{
    if (_instance == 0)
        _instance = new QCSettingWidget();
    _instance->bindingQCLiquid();             //绑定质控液
    return _instance;
}

QCSettingWidget::QCSettingWidget(QWidget *parent) : QWidget(parent)
{
    initData();              //初始化数据
    initForm();              //初始化
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
}

//新增或删除常规项目时，要更新下些界面的常规项目表
void QCSettingWidget::setProjectList(QStringList list)
{
    ProjectList.clear();
    ProjectList = list;
    ProjectList.insert(0, "");
}

//初始化
void QCSettingWidget::initForm()
{
    initTableViewFormat();   //列表格式化
    //button组
    btnAddNew = new QPushButton("新增");  //新增
    btnAlter = new QPushButton("修改");   //修改
    btnDelete = new QPushButton("删除");  //删除
    btnCancel = new QPushButton("取消");  //取消
    btnSave = new QPushButton("保存");    //保存
    btnClose = new QPushButton("关闭");   //关闭
    hlytButton = new QHBoxLayout;        //按钮水平布局
    hlytButton->setMargin(5);
    hlytButton->setSpacing(10);
    hlytButton->addWidget(btnAddNew);
    hlytButton->addWidget(btnAlter);
    hlytButton->addWidget(btnDelete);
    hlytButton->addWidget(btnCancel);
    hlytButton->addWidget(btnSave);
    hlytButton->addWidget(btnClose);

    //总布局
    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(tbwQCName,      0,  0, 90, 50);
    layout->addWidget(tbwProjectList, 0, 50, 90, 50);
    layout->addItem(hlytButton,  90, 0, 10, 100, Qt::AlignCenter);
    setLayout(layout);
}

//列表格式化
void QCSettingWidget::initTableViewFormat()
{
    //质控液列表
    tbwQCName = new QTableWidget(0, 8);
    QStringList headerList_h;
    headerList_h << "" << "id" << "质控液" << "类型" << "批号" << "有效期" << "天数" << "条码";
    tbwQCName->setHorizontalHeaderLabels(headerList_h);
    tbwQCName->verticalHeader()->setVisible(false);                        //隐藏左边列（显示编号的列）
    tbwQCName->horizontalHeader()->setStretchLastSection(true);            //设置充满表宽度
    tbwQCName->setEditTriggers(QAbstractItemView::NoEditTriggers);         //设置表格的单元为只读属性，即不能编辑
    tbwQCName->setSelectionBehavior(QAbstractItemView::SelectRows);        //整行选择
    tbwQCName->horizontalHeader()->setStyleSheet("font-size:18px;");
    int row = 0;
    tbwQCName->hideColumn(row);                //选择列-隐藏
    tbwQCName->setColumnWidth(row ++, 30);     //选择列
    tbwQCName->hideColumn(row ++);             //id-隐藏
    tbwQCName->setColumnWidth( row ++, 80);    //质控液
    tbwQCName->setColumnWidth( row ++, 100);   //类型
    tbwQCName->setColumnWidth( row ++, 130);    //批号
    tbwQCName->setColumnWidth( row ++, 150);   //有效期
    tbwQCName->setColumnWidth( row ++, 90);    //剩余天数
    tbwQCName->setColumnWidth( row ++, 100);    //条码
    tbwQCName->setMinimumWidth(650);

    //项目列表
    int rowCount = PROJECT_COUNT;
    tbwProjectList = new QTableWidget(rowCount, 6);
    QStringList header;
    header << "id" << "项目名称" << "靶值" << "标准差" << "靶值下限" << "靶值上限";
    tbwProjectList->setHorizontalHeaderLabels(header);
    tbwProjectList->verticalHeader()->setVisible(false);                        //隐藏左边列（显示编号的列）
    tbwProjectList->horizontalHeader()->setStretchLastSection(true);            //设置充满表宽度
    tbwProjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);         //设置表格的单元为只读属性，即不能编辑
    tbwProjectList->setSelectionBehavior(QAbstractItemView::SelectRows);        //整行选择
    tbwProjectList->horizontalHeader()->setStyleSheet("font-size:18px;");
    tbwProjectList->setColumnHidden(0, true);                                 //隐藏id列
    tbwProjectList->setColumnWidth(1, 90);
    tbwProjectList->setColumnWidth(2, 90);
    tbwProjectList->setColumnWidth(3, 80);
    tbwProjectList->setColumnWidth(4, 90);
    tbwProjectList->setColumnWidth(5, 90);
    tbwProjectList->setMinimumWidth(480);
    //初始化每一个单元格
    for( int i = 0; i < rowCount; i ++ )
    {
        tbwProjectList->setItem( i, 0, new QTableWidgetItem( "" ));  //
        tbwProjectList->setItem( i, 1, new QTableWidgetItem( "" ));  //
        tbwProjectList->setItem( i, 2, new QTableWidgetItem( "" ));  //
        tbwProjectList->setItem( i, 3, new QTableWidgetItem( "" ));  //
        tbwProjectList->setItem( i, 4, new QTableWidgetItem( "" ));  //
        tbwProjectList->setItem( i, 5, new QTableWidgetItem( "" ));  //

        QComboBox *comBox = new QComboBox();
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setReadOnly(true);
        lineEdit->setAlignment(Qt::AlignCenter);
        comBox->setLineEdit(lineEdit);
        comBox->addItems(ProjectList);
        tbwProjectList->setCellWidget( i, 1, comBox);

        //列表内容显示方式
        tbwProjectList->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        tbwProjectList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        tbwProjectList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        tbwProjectList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        tbwProjectList->item(i, 5)->setTextAlignment(Qt::AlignCenter);
    }
}

//界面风格
void QCSettingWidget::initStyle()
{
    this->setMinimumHeight(680);
    //置顶&隐藏最大/最小化
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);

    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //样式
    QFile qss(":/QSS/QSS/style3.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(qss.readAll());
    qss.close();

    btnAddNew->setEnabled(true);
    btnAlter->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnClose->setEnabled(true);
    this->setObjectName("customWidget");

    int width = 100;
    int height = 40;
    btnAddNew->setMinimumSize(width, height);
    btnAlter->setMinimumSize(width, height);
    btnDelete->setMinimumSize(width, height);
    btnCancel->setMinimumSize(width, height);
    btnSave->setMinimumSize(width, height);
    btnClose->setMinimumSize(width, height);
}

//连接信号与槽
void QCSettingWidget::initConnect()
{
    connect(btnAddNew, SIGNAL(clicked(bool)), this, SLOT(slt_btnAddNew_Clicked()));
    connect(btnAlter, SIGNAL(clicked(bool)), this, SLOT(slt_btnAlter_Clicked()));
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(slt_btnDelete_Clicked()));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(slt_btnSave_Clicked()));
    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(slt_btnClose_Clicked()));

    connect(tbwQCName, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbwQCName_itemClicked(QModelIndex)));
}

//初始化数据
void QCSettingWidget::initData()
{
    //标志，新增/修改标志，（-1：其他，0：修改，1：新增）
    is_AddNew_flag = -1;

    //当前选择的质控液的在表格中的位置
    CurrentQcLiquid_Row = -1;

    //
    ProjectList.clear();
    ProjectList.append("");
    QVector<QString> vecP = gParameter->get_ProjectName();
    for(int i = 0; i < vecP.count(); i ++)
        ProjectList.append(vecP.at(i));

    //样本类型列表 //1:全血，2：血清，3：血浆，4：尿液，5：其他
    SampleTypeList << "" << "全血" << "血清" << "血浆" << "尿液";
}

//绑定质控液
void QCSettingWidget::bindingQCLiquid()
{
    while(tbwQCName->rowCount()>0)
        tbwQCName->removeRow(0);

    int days;
    QMap<int, QCLiquid> map = gParameter->getQCLiquidMap();
    QMap<int, QCLiquid>::iterator it = map.begin();//迭代器
    int i = 0;
    while(it != map.end())
    {
        tbwQCName->setRowCount(i+1);

        QComboBox *comBox = new QComboBox();
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setReadOnly(true);
        lineEdit->setAlignment(Qt::AlignCenter);
        comBox->setLineEdit(lineEdit);
        comBox->addItems(SampleTypeList);
        comBox->setEnabled(false);

        QFont font("", 13);
        QDateEdit *dateEdit = new QDateEdit;
        dateEdit->setDate(QDate::currentDate());
        dateEdit->setFont(font);
        dateEdit->setDisplayFormat("yyyy-MM-dd");

        //<< "" << "id" << "质控液" << "类型" << "批号" << "有效期" << "剩余天数" << "条码";
        tbwQCName->setItem( i, 0, new QTableWidgetItem(""));  //选择
        tbwQCName->setItem( i, 1, new QTableWidgetItem( QString::number(it.value().id) ));  //id
        tbwQCName->setItem( i, 2, new QTableWidgetItem( it.value().Name ));  //质控液
        tbwQCName->setItem( i, 3, new QTableWidgetItem( ""));                          //类型
        tbwQCName->setCellWidget( i, 3, comBox);
        comBox->setCurrentIndex(                        it.value().SampleType);
        tbwQCName->setItem( i, 4, new QTableWidgetItem( it.value().BatchNo ));  //批号
        tbwQCName->setItem( i, 5, new QTableWidgetItem( "" ));                         //有效期
        tbwQCName->setCellWidget( i, 5, dateEdit);
        dateEdit->setDate( QDate::fromString(it.value().ValidDate, "yyyy-MM-dd") );
        tbwQCName->setItem( i, 6, new QTableWidgetItem( "" ));                         //剩余天数
        tbwQCName->setItem( i, 7, new QTableWidgetItem( it.value().BarCode ));  //条码

        days = QDate::currentDate().daysTo( QDate::fromString(it.value().ValidDate, "yyyy-MM-dd") );
        if(days < 0) days = 0;
        tbwQCName->item(i, 6)->setText(QString::number(days));

        it ++;
        i ++;
    }
}

//绑定质控项目参数
void QCSettingWidget::bindingQCProjectParam(int id)
{
    //清空质控项目参数表
    clearProjectTable();

    QComboBox *comBox;
    QCParam *param = gParameter->getQcProjParam(id);
    for(int i = 0; i < PROJECT_COUNT; i ++ )
    {
        if(param[i].Proj_id > 0)
        {
            comBox = (QComboBox*) tbwProjectList->cellWidget(i, 1);

            // << "id" << "项目名称" << "靶值" << "标准差" << "靶值下限" << "靶值上限";
            tbwProjectList->item(i, 0)->setText( QString::number( param[i].Proj_id ));  //id
            comBox->setCurrentIndex( ProjectListIndex( param[i].ProjName ));            //项目
            tbwProjectList->item(i, 2)->setText( QString::number( param[i].TargetValue ) );  //靶值
            tbwProjectList->item(i, 3)->setText( QString::number( param[i].SD ) );           //标准差
            tbwProjectList->item(i, 4)->setText( QString::number( param[i].TargetLowerL ) ); //靶值下限
            tbwProjectList->item(i, 5)->setText( QString::number( param[i].TargetUpperL ) ); //靶值上限
        }
    }
}

//查找name在ProjectList中的索引
int QCSettingWidget::ProjectListIndex(QString name)
{
    int i = 1;
    for(i; i < ProjectList.count(); i ++)
    {
        if( name == ProjectList.at(i))
            return i;
    }
    return 0;
}

//清空质控项目参数表
void QCSettingWidget::clearProjectTable()
{
    //清空每一个单元格
    QComboBox *combox;
    int rowCount = tbwProjectList->rowCount();
    for( int i = 0; i < rowCount; i ++ )
    {
        tbwProjectList->item(i, 0)->setText("");
        //tbwProjectList->item(i, 1)->setText("--");
        combox=(QComboBox*)tbwProjectList->cellWidget(i,1); //强制转化为QComboBox
        combox->setCurrentIndex(0);
        tbwProjectList->item(i, 2)->setText("");
        tbwProjectList->item(i, 3)->setText("");
        tbwProjectList->item(i, 4)->setText("");
        tbwProjectList->item(i, 5)->setText("");
    }
}

//新增质控液
void QCSettingWidget::AddNewQCLiquid()
{
    // "" << "id" << "质控液" << "类型" << "批号" << "有效期" << "天数" << "条码";
    QStringList list;
    QString str;
    int id = 1;
    QString cMaxId = "SELECT MAX(id) from QCLiquidT;";
    QSqlQuery query = SQLite::getInstance()->Select(cMaxId);
    while (query.next()) {
        id = query.value(0).toInt() + 1;
    }

    int newRow = tbwQCName->rowCount() - 1;
    tbwQCName->item(newRow, 1)->setText(QString::number(id));
    QComboBox *comboBox = (QComboBox*)tbwQCName->cellWidget(newRow, 3);
    QDateEdit *dateEdit = (QDateEdit*)tbwQCName->cellWidget(newRow, 5);

    QString nameQC = tbwQCName->item(newRow, 2)->text();
    int sampleType = comboBox->currentIndex();
    QString batchNo = tbwQCName->item(newRow, 4)->text();
    QString validDate = dateEdit->text();
    QString barCode = tbwQCName->item(newRow, 7)->text();

    str = "insert into QCLiquidT(id, name, sampleType, batchNo, validDate, barCode) values("
            + QString::number(id) + ", '"+ nameQC +"', "+ QString::number(sampleType) +","
            + "'" + batchNo + "', '"+ validDate +"', '" + barCode +"');";
    list.append(str);

    //质控项目参数
    QComboBox *comboBoxP;
    int Pid;
    QString name;
    QString targetValue, sd, target1, target2; //靶值，标准差，靶值下限，靶值上限
    for(int i = 0; i < tbwProjectList->rowCount(); i ++)
    {
        comboBoxP = (QComboBox*)tbwProjectList->cellWidget(i, 1);

        //未设置质控项目
        if(comboBoxP->currentIndex() == 0) continue;

        name = comboBoxP->currentText();
        Pid = gParameter->getProject_id(name);
        targetValue = tbwProjectList->item(i, 2)->text();
        sd = tbwProjectList->item(i, 3)->text();
        target1 = tbwProjectList->item(i, 4)->text();
        target2 = tbwProjectList->item(i, 5)->text();

        str = "insert into QCParamT(QCLiquidID, projectID, projectName, targetValue, SD, TargetValue0, TargetValue1) values("
                + QString::number(id) + ", "+ QString::number(Pid) +", '"+ name +"',"
                + targetValue + "," + sd + "," + target1 + "," + target2 + ");";
        list.append(str);
    }
    //qDebug() << list;

    SQLite::getInstance()->Transaction(list);
}

//修改质控液
void QCSettingWidget::UpdateQCLiquid()
{
    //保存最后修改的行
    SaveQcProjectParam(tbwQCName->currentRow());

    QStringList list;
    QString str;

    QMap<int, QCLiquid> map = gParameter->getQCLiquidMap();
    QMap<int, QCLiquid>::iterator it = map.begin();//迭代器

    while(it != map.end())
    {
        str =
                " update QCLiquidT set name='" + it.value().Name + "', "
                +" sampleType=" + QString::number( it.value().SampleType ) +", "
                + " batchNo='" + it.value().BatchNo +"', "
                + " validDate='" + it.value().ValidDate +"', "
                + " barCode='"+ it.value().BarCode  +"' "
                + " where id =" + QString::number( it.value().id )+ ";";
        list.append(str);

        str = "delete from QCParamT where QCLiquidID=" + QString::number( it.value().id ) + ";";
        list.append(str);
        for(int i = 0; i < PROJECT_COUNT; i ++)
        {
            if( it.value().Param[i].Proj_id > 0 )
            {
                str = "insert into QCParamT(QCLiquidID, projectID, projectName, targetValue, SD, TargetValue0, TargetValue1) values("
                        + QString::number( it.value().id ) + ", "+ QString::number( it.value().Param[i].Proj_id ) +", '"+ it.value().Param[i].ProjName +"',"
                        + QString::number(it.value().Param[i].TargetValue) + "," + QString::number(it.value().Param[i].SD)
                        + "," + QString::number(it.value().Param[i].TargetUpperL) + "," + QString::number(it.value().Param[i].TargetLowerL) + ");";
                list.append(str);
                qDebug() << str;
            }
        }

        it ++;
    }

    SQLite::getInstance()->Transaction(list);
}

//槽：新增
void QCSettingWidget::slt_btnAddNew_Clicked()
{
    QComboBox *comBox = new QComboBox();
    comBox->addItems(SampleTypeList);
    comBox->setCurrentIndex(SampleType_Serum);
    comBox->setEnabled(false);

    QFont font("", 13);
    QDateEdit *dateEdit = new QDateEdit;
    dateEdit->setDate(QDate::currentDate().addMonths(3));
    dateEdit->setFont(font);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    //<< "" << "id" << "质控液" << "类型" << "批号" << "有效期" << "剩余天数" << "条码";
    int i = tbwQCName->rowCount();
    tbwQCName->setRowCount(i+1);
    tbwQCName->setItem( i, 0, new QTableWidgetItem( "" ));  //
    tbwQCName->setItem( i, 1, new QTableWidgetItem( "" ));  //
    tbwQCName->setItem( i, 2, new QTableWidgetItem( "" ));  //
    tbwQCName->setItem( i, 3, new QTableWidgetItem( "" ));  //类型
    tbwQCName->setCellWidget( i, 3, comBox);
    tbwQCName->setItem( i, 4, new QTableWidgetItem( "" ));  //
    tbwQCName->setItem( i, 5, new QTableWidgetItem( "" ));  //有效期
    tbwQCName->setCellWidget( i, 5, dateEdit);
    tbwQCName->setItem( i, 6, new QTableWidgetItem( "" ));  //
    tbwQCName->setItem( i, 7, new QTableWidgetItem( "" ));  //

    tbwQCName->setEditTriggers(QAbstractItemView::DoubleClicked);           //
    tbwQCName->setSelectionBehavior(QAbstractItemView::SelectItems);        //
    tbwProjectList->setEditTriggers(QAbstractItemView::DoubleClicked);      //
    tbwProjectList->setSelectionBehavior(QAbstractItemView::SelectItems);   //

    btnAddNew->setEnabled(false);
    btnAlter->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancel->setEnabled(true);
    btnSave->setEnabled(true);
    btnClose->setEnabled(true);
    is_AddNew_flag = 1;

    clearProjectTable();
}

//槽：修改
void QCSettingWidget::slt_btnAlter_Clicked()
{
    btnAddNew->setEnabled(false);
    btnAlter->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancel->setEnabled(true);
    btnSave->setEnabled(true);
    btnClose->setEnabled(true);
    is_AddNew_flag = 0;

    tbwQCName->setEditTriggers(QAbstractItemView::DoubleClicked);           //
    tbwQCName->setSelectionBehavior(QAbstractItemView::SelectItems);        //
    tbwProjectList->setEditTriggers(QAbstractItemView::DoubleClicked);      //
    tbwProjectList->setSelectionBehavior(QAbstractItemView::SelectItems);   //
}

//槽：删除
void QCSettingWidget::slt_btnDelete_Clicked()
{
    int row = tbwQCName->currentRow();
    if( row == -1 ) return;
    QString name = tbwQCName->item(row, 2)->text();
    gMessageBox->setInfo(QString("删除提示"), QString("\n是否要删除选中的质控液“" + name +"”？"),
                         QPixmap("://resource/resource/MessageBox/information.png"), true, false);
    gMessageBox->setButtonText(Cancel, "否(N)");
    gMessageBox->setButtonText(Sure, "是(Y)");
    if(gMessageBox->exec() == MVMessageBox::Accepted)
    {
        QString QCLiquidID = tbwQCName->item(row, 1)->text();
        QString str = "delete from QCLiquidT where id=" + QCLiquidID + ";";
        SQLite::getInstance()->Delete(str);
        tbwQCName->removeRow(row);
        str = "delete from QCParamT where QCLiquidID=" + QCLiquidID + ";";
        SQLite::getInstance()->Delete(str);
        gParameter->ResetQcLiquidMap();
    }

    btnAddNew->setEnabled(true);
    btnAlter->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnClose->setEnabled(true);
}

//槽：取消
void QCSettingWidget::slt_btnCancel_Clicked()
{
    btnAddNew->setEnabled(true);
    btnAlter->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnClose->setEnabled(true);

    //整行选择&禁止编辑
    tbwQCName->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbwQCName->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbwProjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbwProjectList->setSelectionBehavior(QAbstractItemView::SelectRows);

    //删除质控液表中新添加的一行（-1：其他，0：修改，1：新增）
    if( is_AddNew_flag == 1)
    {
        int row = tbwQCName->rowCount();
        tbwQCName->removeRow(row-1);
        clearProjectTable();     //清空质控项目参数表
    }
    else if( is_AddNew_flag == 0)
    {
        gParameter->ResetQcLiquidMap();
        bindingQCLiquid();
    }
}

//槽：保存
void QCSettingWidget::slt_btnSave_Clicked()
{
    btnAddNew->setEnabled(true);
    btnAlter->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnClose->setEnabled(true);

    //整行选择&禁止编辑
    tbwQCName->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbwQCName->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbwProjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbwProjectList->setSelectionBehavior(QAbstractItemView::SelectRows);

    if(is_AddNew_flag == 1) AddNewQCLiquid();          //新增质控液
    else if(is_AddNew_flag == 0) UpdateQCLiquid(); //修改质控液

     gParameter->ResetQcLiquidMap();
    //标志，新增/修改标志，（-1：其他，0：修改，1：新增）
    is_AddNew_flag = -1;
}

//槽：关闭
void QCSettingWidget::slt_btnClose_Clicked()
{
    btnAddNew->setEnabled(true);
    btnAlter->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnClose->setEnabled(true);

    //整行选择&禁止编辑
    tbwQCName->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbwQCName->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbwProjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbwProjectList->setSelectionBehavior(QAbstractItemView::SelectRows);

    qDebug() << is_AddNew_flag;
    //删除质控液表中新添加的一行
    if( is_AddNew_flag == 1)
    {
        int row = tbwQCName->rowCount();
        tbwQCName->removeRow(row-1);
    }

    this->close();
}

//槽：选择不同的质控液
void QCSettingWidget::slt_tbwQCName_itemClicked(QModelIndex index)
{
    if( !index.isValid() ) return;

    //先保存质控项目参数（-1：其他，0：修改，1：新增）
    if(is_AddNew_flag == 0)
    {
        if(CurrentQcLiquid_Row != index.row() )
            SaveQcProjectParam(CurrentQcLiquid_Row);    //保存质控项目参数
    }

    //质控液ID
    int liquidID = tbwQCName->item(index.row(), 1)->text().toInt();
    if(liquidID > 0)
    {
        if(CurrentQcLiquid_Row == index.row() ) return;

        //当前选择的质控液的在表格中的位置
        CurrentQcLiquid_Row = index.row();

        //绑定质控项目参数
        bindingQCProjectParam(liquidID);
    }
}

//保存质控项目参数
void QCSettingWidget::SaveQcProjectParam(int row)
{
    //qDebug() << "CurrentRow:" << row;
    if(row == -1) return;
    // << "" << "id" << "质控液" << "类型" << "批号" << "有效期" << "天数" << "条码";
    QCLiquid liquid;
    QComboBox *comboBox;
    if(row < tbwQCName->rowCount())
    {
        liquid.id = tbwQCName->item(row, 1)->text().toInt();
        if(liquid.id <= 0) return;

        liquid.Name = tbwQCName->item(row, 2)->text();
        comboBox = (QComboBox*)tbwQCName->cellWidget(row, 3);
        liquid.SampleType  = comboBox->currentIndex();
        liquid.BatchNo = tbwQCName->item(row, 4)->text();
        QDateEdit *dateEdit = (QDateEdit*)tbwQCName->cellWidget(row, 5);
        liquid.ValidDate = dateEdit->text();
        liquid.BarCode = tbwQCName->item(row, 7)->text();
    }
    else
        return;

    for(int i = 0; i < PROJECT_COUNT; i ++)
        liquid.Param[i].Proj_id = 0;

    //"id" << "项目名称" << "靶值" << "标准差" << "靶值下限" << "靶值上限";
    int j = 0;
    for( int i = 0; i < tbwProjectList->rowCount(); i ++ )
    {
        comboBox = (QComboBox*) tbwProjectList->cellWidget(i, 1);

        if(comboBox->currentIndex() > 0 )
        {
            liquid.Param[j].ProjName = comboBox->currentText();
            liquid.Param[j].Proj_id = gParameter->getProject_id(comboBox->currentText());
            liquid.Param[j].TargetValue = tbwProjectList->item(i, 2)->text().toDouble();
            liquid.Param[j].SD = tbwProjectList->item(i, 3)->text().toDouble();
            liquid.Param[j].TargetLowerL = tbwProjectList->item(i, 4)->text().toDouble();
            liquid.Param[j].TargetUpperL = tbwProjectList->item(i, 5)->text().toDouble();
            j ++;
        }
    }

    gParameter->insertQcLiquidMap(liquid.id, liquid);
}









