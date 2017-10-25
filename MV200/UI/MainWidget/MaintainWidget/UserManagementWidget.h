#ifndef USERMANAGEMENTWIDGET_H
#define USERMANAGEMENTWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFile>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include "SubInterface/AddNewUserWidget.h"

#define MAX_USER_COUNT 60     //最多显示60个用户信息

class UserManagementWidget : public QWidget
{
    Q_OBJECT
public:
    static UserManagementWidget *getInstance(); //实例对象

private:
    explicit UserManagementWidget(QWidget *parent = 0);
    static UserManagementWidget *_instance;  //本类单例对象

private:
    void initForm();              //初始化
    void initTableFormat();       //初始化用户列表
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据
    void SelectAllUser();         //查看所有用户

    bool isAllNull_Flag;      //全选/反选标志

signals:

public slots:
    void slt_btnAllNone_Clicked(); //槽：全选/反选
    void slt_btnDelect_Clicked();  //槽：删除
    void slt_btnAddNew_Clicked();  //槽：新增
    void slt_UpdateUserData();     //槽：更新用户信息

private:
    QStandardItemModel *ModelList;
    QTableView *tbvList;
    QPushButton *btnAllNone;
    QPushButton *btnDelect;
    QPushButton *btnAddNew;
    QGridLayout *layout;

};

#endif // USERMANAGEMENTWIDGET_H
