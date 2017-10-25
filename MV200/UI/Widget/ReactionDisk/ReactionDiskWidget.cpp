#include "ReactionDiskWidget.h"
#include <qdebug.h>

ReactionDiskWidget::ReactionDiskWidget(QWidget *parent) : QWidget(parent)
{
    isFirstTimeClean = 0;

    initReactionDisk(REACTION_CUP_COUNT);
    initStatusInfoForm();
    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(view,           0,  0, 100, 100);
    layout->addWidget(wdgStatusInfo, 30, 30,  40,  40);
    this->setLayout(layout);
    initStyle();
}

void ReactionDiskWidget::initReactionDisk(int num)
{
    item[0] = NULL;
    m_scene = new QGraphicsScene();
    //设置“画布”矩形区域，坐标（x,y）为左上角，这就意味着scene的中心位置为(0,0).这个坐标系就是scene的坐标系

    //把“画布” “挂”到展示的窗体QGraphicsView当中
    view = new QGraphicsView(m_scene);
    m_scene->addEllipse(-265, -265, 565, 565, QPen(Qt::black, 3)); //反应盘内圈
    m_scene->addEllipse(-300, -300, 635, 635, QPen(Qt::black, 3)); //反应盘外圈

    double angle = 1.0*3.1415*2/num; //每个小圆占的角度
    int radius = 15;                 //半径

    for(int i=0; i<num; i++)
    {
        item[i] = new ReactionDiskGraphicsItem(radius, i+1); //注意：id已经加1了
        //item->setPos(x,y)设置MyItem在“画布”scene的位置，此时，就是把item相对于自己的坐标系的原点放到scene的（x,y）位置
        item[i]->setPos(-300*sin(angle * i), -300*cos(angle * i));
        item[i]->setToolTip(QString::number(i+1));
        m_scene->addItem(item[i]);   //往“画布”上绘制
        connect(item[i], SIGNAL(slt_CurrentID(int)), this, SLOT(slt_SelectCup(int)));
    }
    m_scene->update();
}

void ReactionDiskWidget::initStatusInfoForm()
{
    //待清洗、空闲、清洗中、脏杯、稀释、R1、Sample、R2、End1、End2
    wdgStatusInfo = new QWidget;
    lytStatusInfo = new QGridLayout;
    lytStatusInfo->setSpacing(5);
    lytStatusInfo->setMargin(5);

    //QString str = "待清洗+空闲+清洗中+脏杯+稀释+R1+Sample+R2+加样完成+测试完成"; //mv100测试状态
    QString str = "待清洗+空闲+清洗中+脏杯+加样中+加样完成+测试完成";
    for(int i = 0; i < STATUSCOUNT; i ++ )
    {
        if( i > str.split("+").count()-1 ) break;

        labStatusName[i] = new QLabel(str.split("+").at(i)+":"); //状态名称
        labStatusPic[i] = new QLabel;              //状态图标
        labStatusPic[i]->setFixedSize(30, 30);
        if( i < STATUSCOUNT/2 )
        {
            lytStatusInfo->addWidget(labStatusName[i], i, 0, 2, 5, Qt::AlignRight);
            lytStatusInfo->addWidget(labStatusPic[i],  i, 5, 2, 5, Qt::AlignLeft);
        }
        else
        {
            lytStatusInfo->addWidget(labStatusName[i], i-STATUSCOUNT/2, 12, 2, 5, Qt::AlignRight);
            lytStatusInfo->addWidget(labStatusPic[i],  i-STATUSCOUNT/2, 17, 2, 5, Qt::AlignLeft);
        }

    }
    wdgStatusInfo->setLayout(lytStatusInfo);
}

void ReactionDiskWidget::initStyle()
{
    QString str =
            "QLabel#Wait     { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/Wait.png);}"
            "QLabel#IDLE     { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/IDLE.png); }"
            "QLabel#Clean    { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/Clean.png); }"
            "QLabel#Dirty    { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/Dirty.png); }"
            "QLabel#End1     { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/End1.png); }"
            "QLabel#End2     { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/End2.png); }"

            "QLabel#Dilution { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/Dilution.png); }"
            "QLabel#R1       { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/R1.png); }"
            "QLabel#Sample   { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/Sample.png); }"
            "QLabel#R2       { font-size:20px; image: url(:/resource/resource/ReactionDiskStatus/R2.png); }"

            ;
    this->setStyleSheet(str);

    labStatusPic[0]->setObjectName("Wait");
    labStatusPic[1]->setObjectName("IDLE");
    labStatusPic[2]->setObjectName("Clean");
    labStatusPic[3]->setObjectName("Dirty");
    labStatusPic[4]->setObjectName("R2");
    labStatusPic[5]->setObjectName("End1");
    labStatusPic[6]->setObjectName("End2");
    //    labStatusPic[4]->setObjectName("Dilution");
    //    labStatusPic[5]->setObjectName("R1");
    //    labStatusPic[6]->setObjectName("Sample");
    //    labStatusPic[7]->setObjectName("R2");
    //    labStatusPic[8]->setObjectName("End1");
    //    labStatusPic[9]->setObjectName("End2");
}

//设置反应杯的状态
void ReactionDiskWidget::setCupStatus(int cup, QString objName)
{ 
    mutex.lock();
    //qDebug() << "设置反应杯状态:" << cup << objName;
    if(cup<=0)
    {
        mutex.unlock();
        return;
    }
    if(cup-1 <= REACTION_CUP_COUNT)
    {
        item[cup-1]->setStatus(objName);
        item[cup-1]->update();
    }
    else
    {
        mutex.unlock();
        return;
    }

    //清洗反应杯
    if(objName == "Clean")
    {
        int cup1, cup2, cup3, cup4, cup5, cup6;
        if( cup == 1 ) cup1 = REACTION_CUP_COUNT;
        else cup1 =cup-1;
        if( cup1 == 1 ) cup2 = REACTION_CUP_COUNT;
        else cup2 =cup1-1;
        if( cup2 == 1 ) cup3 = REACTION_CUP_COUNT;
        else cup3 =cup2-1;
        if( cup3 == 1 ) cup4 = REACTION_CUP_COUNT;
        else cup4 =cup3-1;
        if( cup4 == 1 ) cup5 = REACTION_CUP_COUNT;
        else cup5 =cup4-1;
        if( cup5 == 1 ) cup6 = REACTION_CUP_COUNT;
        else cup6 =cup5-1;

        //qDebug() << cup << cup1 << cup2 << cup3;
        item[cup1-1]->setStatus("Clean");
        item[cup2-1]->setStatus("Clean");
        item[cup3-1]->setStatus("Clean");
        item[cup1-1]->update();
        item[cup2-1]->update();
        item[cup3-1]->update();

        if(isFirstTimeClean < 4)
        {
            isFirstTimeClean ++;

            item[cup4-1]->setStatus("Wait");
            item[cup5-1]->setStatus("Wait");
            item[cup6-1]->setStatus("Wait");
            item[cup4-1]->update();
            item[cup5-1]->update();
            item[cup6-1]->update();
        }
        else
        {
            item[cup4-1]->setStatus("IDLE");
            item[cup5-1]->setStatus("IDLE");
            item[cup6-1]->setStatus("IDLE");
            item[cup4-1]->update();
            item[cup5-1]->update();
            item[cup6-1]->update();
        }
    }
    mutex.unlock();
}

void ReactionDiskWidget::slt_SelectCup(int id)
{
    reFlash(id);
    emit sig_CurrentText( QString::number(id) );
}

void ReactionDiskWidget::reFlash(int unFlashID)
{
    for(int i=0; i<REACTION_CUP_COUNT; i++)
        item[i]->setTxtColor_FontSize(Qt::black, 15);

    item[unFlashID-1]->setTxtColor_FontSize(Qt::magenta, 14);
    m_scene->update();
}
