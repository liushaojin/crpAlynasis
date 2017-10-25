#include "Save2HistoryProgressBar.h"
#include "Thread/Scheduler.h"
#include <QDesktopWidget>
#include "UI/MainWidget/ApplicationWidget/SonWidget/SelectSampleWidget.h"

Save2HistoryProgressBar::Save2HistoryProgressBar(QWidget *parent)
    : QWidget(parent)
{
    initForm();         //初始化
    initStyle();        //界面风格
}

//初始化
void Save2HistoryProgressBar::initForm()
{
    labSavingStatus = new QLabel("转移到历史库...");       //
    barSavingStatus = new QProgressBar; //
    lytSaving = new QGridLayout;        //
    lytSaving->setMargin(10);
    lytSaving->setSpacing(10);
    lytSaving->addWidget(labSavingStatus, 0, 0, 2, 20, Qt::AlignCenter);
    lytSaving->addWidget(barSavingStatus, 2, 0, 2, 20);
    this->setLayout(lytSaving);


    //繁忙指示:如果最小值和最大值都设置为0，进度条会显示了一个繁忙指示，而不会显示当前的值。
    barSavingStatus->setOrientation(Qt::Horizontal);  // 水平方向
    barSavingStatus->setMinimum(0);  // 最小值
    barSavingStatus->setMaximum(0);  // 最大值
    //barSavingStatus->setValue(50);  // 当前进度
    //double dProgress = (barSavingStatus->value() - barSavingStatus->minimum()) * 100.0
    //            / (barSavingStatus->maximum() - barSavingStatus->minimum());
    //barSavingStatus->setFormat(QString::fromLocal8Bit("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));
    //barSavingStatus->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

//界面风格
void Save2HistoryProgressBar::initStyle()
{
    QString styleBar =
            "QProgressBar{background: transparent; border: none;color: white;text-align: center;background: rgb(68, 169, 73);}"
            "QProgressBar::chunk{background: transparent; border: none;background: rgb(0, 180, 230);}";
    QString styleBG = "QWidget{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #113845, stop: 0.2 #1878B1, stop: 1.0 #15A8FF); }";
    QString styleLab = "QLabel{background: transparent;font:20px Times New Roman;}";
    this->setStyleSheet(styleBar+styleBG+styleLab);
    this->setMinimumSize(600, 120);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

}

//开始转移到历史结果表中
void Save2HistoryProgressBar::StartSave2HistoryT()
{
    this->setFixedSize(600, 120);
    int x = ( QApplication::desktop()->width() - width() ) / 2;
    int y = ( QApplication::desktop()->height() - height() ) / 2;
    this->setGeometry(x, y, width(), height());
    this->show(); //显示等待窗体


    QStringList list;
    QString deleteStr; //删除临时任务表的sql语句
    QString appendStr; //新增到历史结果表的sql语句
    QString str  =
            " select id, no,statusID,applyTime,inspectionDate, testDate,reprotDate,userID,typeID,projectID,sampleCode,SamplePos,Cup0,Cup1,"
            " sampleTypeID,isPrint,isEmergency,result,amplitude,isRetest,isModify,modifyDate,modifierID,result1,lightPos1,lightPos2,path1,path2,"
            " isCalculateProj, calculateProjID, "
            " AD_A1, AD_A2, AD_A3, AD_A4, AD_A5, AD_A6, AD_A7, AD_A8, AD_A9, AD_A10, AD_A11, AD_A12, AD_A13, AD_A14, AD_A15, AD_A16, AD_A17, AD_A18,"
            " AD_A19, AD_A20, AD_A21, AD_A22, AD_A23, AD_A24, AD_A25, AD_A26, AD_A27, AD_A28, AD_A29, AD_A30, AD_A31, AD_A32, AD_A33, AD_A34, AD_A35,"
            " AD_A36, AD_A37, AD_A38, AD_A39, AD_A40, AD_A41, AD_A42, AD_A43, AD_A44, AD_A45, AD_A46, AD_A47, AD_A48, AD_A49, AD_A50,T1, T2, T3, T4, T5,"
            " T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, "
            " T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50 "
            " from TempHistoryT "
            //只查找完成状态的任务&普通样本测试
            " where statusID = " + QString::number(State_FINISHED) + " and typeID ="+ QString::number(MV_State_Normal) +";";
    //qDebug() << str;
    list.clear();
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    QString tempAD, tempDate, strAD_A;
    while(query.next())
    {
        //qDebug() << endl << query.value(0).toString()<< query.value(1).toString() << query.value(2).toString();

        strAD_A = "";
        tempAD = "";
        tempDate = "";
        for(int i = 0; i < 50; i ++)
        {
            if(query.value(30+i).toString() != "")
            {
                strAD_A += "AD_A" + QString::number(i +1) + ", ";
                tempAD += query.value(30+i).toString() +", ";
            }
            tempDate += "'" + query.value(30+i).toString() + "', ";
        }

        appendStr =
                "insert into HistoryT(no,statusID,applyTime,inspectionDate, testDate,reprotDate,userID,typeID,projectID,sampleCode,SamplePos,Cup0,Cup1,"
                " sampleTypeID,isPrint,isEmergency,result,amplitude,isRetest,isModify,modifyDate, isCalculateProj, calculateProjID, "
                + strAD_A
                + " T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, "
                  " T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50,"
                  " remarks ) "
                  " values("
                  "'" + query.value(1).toString() + "', "  //no
                + query.value(2).toString() + ", "         //statusID

                + "'" + query.value(3).toString() + "', "  //applyTime
                + "'" + query.value(4).toString() + "', "  //inspectionDate
                + "'" + query.value(5).toString() + "', "  //testDate
                + "'" + query.value(6).toString() + "', "  //reprotDate

                + query.value(7).toString() + ", "  //userID
                + query.value(8).toString() + ", "  //typeID
                + query.value(9).toString() + ", "  //projectID

                + "'" + query.value(10).toString() + "', "   //sampleCode
                + "'" + query.value(11).toString() + "', "   //SamplePos
                + "'" + query.value(12).toString() + "', "   //Cup0
                + "'" + query.value(13).toString() + "', "   //Cup1

                + query.value(14).toString() + ", "  //sampleTypeID
                + query.value(15).toString() + ", "  //isPrint
                +  "0, "  //isEmergency（mv200没有急诊）
                + query.value(17).toString() + ", "  //result
                + query.value(18).toString() + ", "  //amplitude
                + query.value(19).toString() + ", "  //isRetest
                + query.value(20).toString() + ", "  //isModify
                + "'" + query.value(21).toString() + "', "   //modifyDate
                //+ query.value(22).toString() + ", "  //modifierID
                //+ query.value(23).toString() + ", "  //result1
                //+ query.value(24).toString() + ", "  //lightPos1
                //+ query.value(25).toString() + ", "  //lightPos2
                //+ query.value(26).toString() + ", "  //path1
                //+ query.value(27).toString() + ", "  //path2

                + query.value(28).toString() + ", "  //
                + query.value(29).toString() + ", "  //

                + tempAD     //AD_?
                + tempDate   //T_?
                +" '' );";   //remarks
        list.append(appendStr);

        //删除已经转移成功的记录
        deleteStr = "delete from TempHistoryT where id="+ query.value(0).toString() +";";
        list.append(deleteStr);
    }

    //删除 未完成&测试中 的所有测试
    deleteStr = "delete from TempHistoryT where statusID="+ QString::number(State_unFINISHED) +";";
    list.append(deleteStr);
    deleteStr = "delete from TempHistoryT where statusID=" + QString::number(State_TESTING) +";";
    list.append(deleteStr);
    //重置
    gParameter->setSampleNo(1);
    gSelectSampleWidget->setSampleNoStart(1);
    //gSampleApplicationSampleDiskInfo->setSampleNoStart(1);
    str = "update SysParamT set SampleNO=1;";
    list.append(str);

    //qDebug() << list;
    if(list.count() == 0)
    {
        this->close();
        return;
    }
    else
    {
        if( SQLite::getInstance()->Transaction(list) )
        {
            this->close();

            gMessageBox->setInfo(QString("提示"), QString("转移结果到历史库成功!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "返回");
            if(gMessageBox->exec() == MVMessageBox::Accepted)
                return;
        }
    }
}
