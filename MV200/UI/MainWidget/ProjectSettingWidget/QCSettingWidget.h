#ifndef QCSETTINGWIDGET_H
#define QCSETTINGWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "ParamDefine.h"
#include "Parameter.h"
#include "UI/Widget/MessageBox/messageBox.h"

class QCSettingWidget : public QWidget
{
    Q_OBJECT
public:
    static QCSettingWidget *getInstance();
    void setProjectList(QStringList list); //新增或删除常规项目时，要更新下些界面的常规项目表

private:
    static QCSettingWidget *_instance;  //本类单例对象
    explicit QCSettingWidget(QWidget *parent = 0);
    void initForm();              //初始化
    void initTableViewFormat();   //列表格式化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据
    void bindingQCLiquid();             //绑定质控液
    void bindingQCProjectParam(int id); //绑定质控项目参数
    int ProjectListIndex(QString name); //查找name在ProjectList中的索引
    void clearProjectTable();           //清空质控项目参数表
    void AddNewQCLiquid();              //新增质控液
    void UpdateQCLiquid();              //修改质控液
    void SaveQcProjectParam(int row);    //保存质控项目参数

    //标志，新增/修改标志，（-1：其他，0：修改，1：新增）
    int is_AddNew_flag;
    QStringList ProjectList;    //常规项目列表
    QStringList SampleTypeList; //样本类型列表
    int CurrentQcLiquid_Row;     //当前选择的质控液的在表格中的位置


signals:

public slots:
    void slt_btnAddNew_Clicked();   //槽：新增
    void slt_btnAlter_Clicked();    //槽：修改
    void slt_btnDelete_Clicked();   //槽：删除
    void slt_btnCancel_Clicked();   //槽：取消
    void slt_btnSave_Clicked();     //槽：保存
    void slt_btnClose_Clicked();    //槽：关闭

    void slt_tbwQCName_itemClicked(QModelIndex); //槽：选择不同的质控液

private:
    //质控液列表
    QTableWidget *tbwQCName;

    //项目列表
    QTableWidget *tbwProjectList;

    //button组
    QPushButton *btnAddNew;  //新增
    QPushButton *btnAlter;   //修改
    QPushButton *btnDelete;  //删除
    QPushButton *btnCancel;  //取消
    QPushButton *btnSave;    //保存
    QPushButton *btnClose;   //关闭
    QHBoxLayout *hlytButton; //按钮水平布局

    //总布局
    QGridLayout *layout;
};

#endif // QCSETTINGWIDGET_H
