#ifndef RESULTCURVEWIDGET_H
#define RESULTCURVEWIDGET_H

#include <QFile>
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QVector>
#include <QTimer>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QFileDialog>

#include "UI/Widget/QCustomPlot/qcustomplot.h"
#include "Parameter.h"

class ResultCurveWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ResultCurveWidget(int id);
    ~ResultCurveWidget();
    void setSelect_id(int id);            //设置查询id(临时结果表)
    void setSelect_History_id(int id);    //设置查询id（历史结果数据表）

    static ResultCurveWidget *getInstance();          //实例对象

private:
    static ResultCurveWidget *_instance;       //本类单例对象
    int History_id;
    void initData();                           //初始化数据
    void initLanguage();                       //语言设置
    void initForm();                           //初始化界面控件
    void initStyle();                          //初始化样式
    void initConnect();                        //连接信号与槽
    QVector<double> selectDB();                //查询AD数据
    void ClearTestInfo();                      //清空测试信息

    int idType; //查询类型（0：临时结果表， 1：历史结果数据表）

    void moveEvent(QMoveEvent *event);

    QTimer * timeUpdateTestingData;
    int start; //曲线上限
    int end;   //曲线下限
    int lightPos1, lightPos2; //测光点1，2


signals:

public slots:
    void slt_btnPrevious_Clicked();                //槽：上一页
    void slt_btnNext_Clicked();                    //槽：下一页
    void slt_btnExprot_Clicked();                  //槽：导出Excel
    void slt_btnSure_Clicked();                    //槽：更改
    void slt_timeUpdateTestingData_timeout();

private:
    //测试信息
    QGroupBox *grbTestInfo;
    QGridLayout *lytTestInfo;
    QLabel *labTestNo;         //测试编号
    QLineEdit *editTestNo;
    QLabel *labApplicateTime;  //申请时间
    QLineEdit *editApplicateTime;
    QLabel *labTestTime;  //测试时间
    QLineEdit *editTestTime;
    QLabel *labProjectName;    //项目名称
    QLineEdit *editProjectName;
    QLabel *labSamplePos;      //样本位置
    QLineEdit *editSamplePos;
    QLabel *labCup1;           //反应杯1
    QLineEdit *editCup1;
    QLabel *labCup2;           //反应杯2
    QLineEdit *editCup2;
    QLabel *labResult;         //结果浓度
    QLineEdit *editResult;
    QLabel *labAD_Range;       //幅度
    QLineEdit *editAD_Range;

    QLabel *lab_Hid;           //临时历史结果表中的id（测试用）

    QWidget *wdgBtn;
    QHBoxLayout *hlytBtn;
    QPushButton *btnExprot;
    QPushButton *btnPrevious;
    QPushButton *btnNext;

    //曲线设置
    QWidget *wdt;
    QLabel *labUp;
    QLineEdit *editUp;
    QLabel *labDown;
    QLineEdit *editDown;
    QGridLayout *lyt;
    QPushButton *btnSure;

    QCustomPlot *customPlot;
    QCPGraph *graphResult;    //AD曲线值
    QCPGraph *graphLightPos1; //采光点1
    QCPGraph *graphLightPos2; //采光点2


    QGridLayout *layout;
};

#endif // RESULTCURVEWIDGET_H
