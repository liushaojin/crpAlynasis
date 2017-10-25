#ifndef CLACULATEPROJECTWIDGET_H
#define CLACULATEPROJECTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include <QSignalMapper>

#include "Parameter.h"
#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"

#define CALCULATE_BTN_COUNT 20  //计算功能键显示区按钮的个数

class CalculateProjectWidget : public QWidget
{
    Q_OBJECT
public:

    static CalculateProjectWidget *getInstance();

private:
    explicit CalculateProjectWidget(QWidget *parent = 0);
    static CalculateProjectWidget *_instance;  //本类单例对象
    void initForm();                 //初始化
    void initTableForm();            //格式化表格
    void initStyle();                //界面风格
    void initConnect();              //连接信号与槽
    void initData();                 //初始化数据
    //绑定组合项目到列表中
    void BindingCombProject();

    QStringList ExpressionList;      //表达式列表
    QString ProjectList;             //项目列表，以“+”为分隔 如：TP+hs-CRP
    int is_AddNew_flag;              //标志，新增/修改标志，（0：修改，1：新增）

signals:

public slots:
    //槽：新增
    void slt_btnAddNew_Clicked();
    //槽：修改
    void slt_btnModify_Clicked();
    //槽：删除
    void slt_btnDelete_Clicked();
    //槽：取消
    void slt_btnCancle_Clicked();
    //槽：保存
    void slt_btnSave_Clicked();

    //槽：更新项目选择区中的项目（常规项目）
    void slt_UpdateRoutineItem();
    //槽：计算功能键信号转发器
    void slt_sigMapper_Handle(int index);
    //槽：项目选择键信号转发器
    void slt_sigMapperProject_Handle(int index);

    //槽：更新选中组合项目的信息到右侧界面
    void slt_tbvSelectRow_info(QModelIndex index);

private:
    //计算项目清单
    QGroupBox *grbCalculateProjectList;
    QTableView *tbvCalculateProjectList;
    QStandardItemModel *modelCalculateProjectList;
    QGridLayout *lytCalculateProjectList;

    //计算项目
    QWidget *wdgCalculateProject;
    QGridLayout *lytCalculateProject;

    QGroupBox *grbCalculateInfo; //计算项目信息
    QLabel *labProjectNameEN;    //计算项目名称（英文简称）
    QLineEdit *editProjectNameEN;
    QLabel *labProjectNameCN;    //计算项目名称（中文全称）
    QLineEdit *editProjectNameCN;
    QLabel *labUnit;             //项目单位
    QLineEdit *editUnit;
    QLabel *labAccuracy;         //结果精度
    //QLineEdit *editAccuracy;
    QComboBox *cboAccuracy;
    QLabel *labRange;            //参考范围
    QLineEdit *editRange1;
    QLineEdit *editRange2;
    QGridLayout *lytCalculateInfo;

    QGroupBox *grbExpression;   //表达式显示区
    QLabel *labExpression;
    QLineEdit *editExpression;
    QHBoxLayout *hlytExpression;

    QGroupBox *grbCalculate; //计算功能键显示区
    QPushButton *btnCalculate[CALCULATE_BTN_COUNT];
    QGridLayout *lytCalculate;
    QSignalMapper *sigMapper;  //信号转发器

    QGroupBox *grbProjectList;   //项目选择区
    QRadioButton *rdoRoutineItem[PROJECT_COUNT];  //常规项目
    QGridLayout *lytProjectList;
    QSignalMapper *sigMapperProject;  //信号转发器

    QWidget *wdgButtonList;  //按键组
    QPushButton *btnAddNew;  //新增
    QPushButton *btnModify;  //修改
    QPushButton *btnDelete;  //删除
    QPushButton *btnCancle;  //取消
    QPushButton *btnSave;    //保存
    QHBoxLayout *hlytButtonList;

    //
    QHBoxLayout *layout;
};

#endif // CLACULATEPROJECTWIDGET_H
