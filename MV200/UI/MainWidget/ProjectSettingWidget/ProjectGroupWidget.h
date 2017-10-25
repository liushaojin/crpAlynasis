#ifndef PROJECTGROUPWIDGET_H
#define PROJECTGROUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "Parameter.h"
#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"

class ProjectGroupWidget : public QWidget
{
    Q_OBJECT
public:

    static ProjectGroupWidget *getInstance();

private:
    explicit ProjectGroupWidget(QWidget *parent = 0);
    static ProjectGroupWidget *_instance;  //本类单例对象
    void initForm();                 //初始化
    void initTableFormat();          //格式化表格
    void initStyle();                //界面风格
    void initConnect();              //连接信号与槽
    void initData();                 //初始化数据
    void BindingCombProject();       //绑定组合项目到列表中

    QString CombineProjectNameList();  //组合项目成员，以“+”为分隔
    QString CombineProject_idList();   //组合项目成员ID，以"-"分隔
    bool is_AddNew_flag;             //标志，新增/修改标志，（0：修改，1：新增）

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

    //槽：更新选中组合项目的信息到右侧界面
    void slt_tbvSelectRow_info(QModelIndex);

private:
    //组合项目清单
    QGroupBox *grbCombProjectList;
    QTableView *tbvCombProjectList;
    QStandardItemModel *modelCombProjectList;
    QGridLayout *lytCombProjectList;

    //组合项目
    QWidget *wdgCombProject;
    QGridLayout *lytCombProject;
    QGroupBox *grbCombInfo;      //组合项目信息
    QLabel *labProjectNameEN;    //组合项目名称（英文简称）
    QLineEdit *editProjectNameEN;
    QLabel *labProjectNameCN;    //组合项目名称（中文全称）
    QLineEdit *editProjectNameCN;
    QGridLayout *lytCombInfo;
    QGroupBox *grbProjectList;   //项目选择区
    QRadioButton *rdoRoutineItem[PROJECT_COUNT];  //常规项目
    QGridLayout *lytProjectList;
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
#endif // PROJECTGROUPWIDGET_H
