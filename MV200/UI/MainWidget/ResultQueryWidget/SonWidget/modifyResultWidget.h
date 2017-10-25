#ifndef MODIFYRESULTWIDGET_H
#define MODIFYRESULTWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QDateTime>

#include "Parameter.h"
#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"
#include "Object/Logic5P/MV_Logic5P.h"

//结果修改
class ModifyResultWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyResultWidget();
    void setList(QList<QStringList> list);

private:
    void initForm();             //初始化界面控件
    void initStyle();            //初始化样式
    void initData();             //初始化数据
    void initConnect();          //连接信号与槽
    void initLanguage();         //语言设置

    double ReCalculateResult(int projectID, int R, int sampleType);//重新计算结果
    QStringList idList;          //id列表
    QStringList SampleTypeList;          //样本类型列表

signals:
    void sig_ModifyResult();     //修改返回
    
public slots:
    void slt_btnSure_clicked();
    void slt_btnCancel_clicked();

private:
    QLabel * labTitle;
    QLabel * labNo0;                            //编号
    QLabel * labProject0;                       //
    QLabel * labName0;                          //
    QLabel * labResult0;                        //
    QLabel * labModifyResult0;                  //

    QLabel * labNo[ROWCOUNT];                //
    QLabel *labProject[ROWCOUNT];
    QLabel * labName[ROWCOUNT];              //
    QLabel * labResult[ROWCOUNT];            //
    QLineEdit * edit[ROWCOUNT];              //
    QPushButton * btnSure;
    QPushButton * btnCancel;
    QGridLayout *layout;
};

#endif // MODIFYRESULTWIDGET_H
