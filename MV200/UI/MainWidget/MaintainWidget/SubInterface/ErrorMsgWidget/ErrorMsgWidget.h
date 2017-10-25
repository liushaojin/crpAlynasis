#ifndef ERRORMSGWIDGET_H
#define ERRORMSGWIDGET_H

#include <QWidget>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include <qpushbutton.h>
#include <QGridLayout>

#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"
#include "TermsWidget/TermsWidget.h"

class ErrorMsgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorMsgWidget(QWidget *parent = 0);
    static ErrorMsgWidget *getInstance();          //实例对象


private:
    static ErrorMsgWidget *_instance;  //本类单例对象

private:
    void initForm();              //初始化
    void initTableFormat();       //格式化表格
    void initStyle();             //界面风格
    void initData();              //初始化数据
    void initConnect();           //连接信号与槽
    int QueryErrorMsg();          //查询错误信息

    int Count;                    //查询计数
    QString queryTerms;           //查询条件

signals:

public slots:
    void slt_ChangeQueryTerms(QString term); //槽：改变查询条件
    void slt_btnQueryTerms_Clicked(); //槽：查询条件
    void slt_btnReQuery_Clicked();    //槽：重新查询
    void slt_btnFormer_Clicked();     //槽：上一页
    void slt_btnNext_Clicked();       //槽：下一页

private:
    QTableView *tbvList;            //列表
    QStandardItemModel *modelList;  //列表模型

    //
    QPushButton *btnQueryTerms;     //查询条件
    QPushButton *btnReQuery;        //重新查询
    QPushButton *btnFormer;         //上一页
    QPushButton *btnNext;           //下一页
    QPushButton *btnBack;           //返回

    //查询条件
    TermsWidget *termsWidget;

    QGridLayout *layout;

};

#endif // ERRORMSGWIDGET_H
