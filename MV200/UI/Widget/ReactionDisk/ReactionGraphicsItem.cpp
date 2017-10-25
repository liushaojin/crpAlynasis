#include "ReactionGraphicsItem.h"
#include <qdebug.h>

ReactionDiskGraphicsItem::ReactionDiskGraphicsItem(double radius, int no)
    : QObject()
{
    id = no;
    setStatus("Wait");
    m_radius = radius;
    txtColor = Qt::black;
    txtFontSize = 15;

    setFlags(ItemIsSelectable | ItemAcceptsInputMethod);
}


ReactionDiskGraphicsItem::~ReactionDiskGraphicsItem()
{

}

QRectF ReactionDiskGraphicsItem::boundingRect() const
{
    //这个函数的返回值是一个QRectF（一个正方形的区域）,
    //当我们需要处理我们所写的MyItem接受到的某些事件（比如鼠标按下、拖动等）时，这些事件就被规定只能发生在这个返回的矩形区域当中时才会起被接收到
    return QRectF(0, 0, 2*m_radius, 2*m_radius);//每个item都有自己的一个坐标系，
    //这个设置相当于把item相对于自身的坐标系的原点（0,0）放到自己的正中央。
}

void ReactionDiskGraphicsItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    QPen pen(txtColor);
    painter->setPen(pen);

#ifdef Q_OS_WIN//如果是windows系统
    txtFontSize = 15;
#else          //如果是unix或者其他系统
    txtFontSize = 50;
#endif
    //QFont font("",txtFontSize,QFont::Normal,false);//设置字体的类型，大小，加粗，斜体
    QFont font("",txtFontSize);//设置字体的类型，大小，加粗，斜体
    painter->setFont(font);//添加字体

    //painter->drawEllipse(QPointF(0, 0), 15, 15);      //画园
    //painter->drawText(0, 0, 20, 20, 0, number);   //显示文本数字
    painter->drawPixmap(0, 0, 35, 35, QPixmap(pixMap));//画图

    if(txtFontSize == 10)
        painter->drawText(11, 11, 30, 30, 0, QString::number(id));   //显示文本数字
    else
        painter->drawText(8, 8, 50, 50, 0, QString::number(id));   //显示文本数字
}

void ReactionDiskGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit slt_CurrentID(id);
    QGraphicsItem::mousePressEvent(event);
    update();
}

//设置状态
void ReactionDiskGraphicsItem::setStatus(QString objName)
{
    if(objName == "Wait")
        pixMap =":/resource/resource/ReactionDiskStatus/Wait.png";
    else  if(objName == "IDLE")
        pixMap =":/resource/resource/ReactionDiskStatus/IDLE.png";
    else  if(objName == "Clean")
        pixMap =":/resource/resource/ReactionDiskStatus/Clean.png";
    else  if(objName == "Dirty")
        pixMap =":/resource/resource/ReactionDiskStatus/Dirty.png";
    else  if(objName == "Dilution")
        pixMap =":/resource/resource/ReactionDiskStatus/Dilution.png";
    else  if(objName == "R1")
        pixMap =":/resource/resource/ReactionDiskStatus/R1.png";
    else  if(objName == "Sample")
        pixMap =":/resource/resource/ReactionDiskStatus/Sample.png";
    else  if(objName == "R2")
        pixMap =":/resource/resource/ReactionDiskStatus/R2.png";
    else  if(objName == "End1")
        pixMap =":/resource/resource/ReactionDiskStatus/End1.png";
    else  if(objName == "End2")
        pixMap =":/resource/resource/ReactionDiskStatus/End2.png";
}

void ReactionDiskGraphicsItem::setTxtColor_FontSize(QColor color, int size)
{
    txtFontSize = size;
    txtColor = color;
//    update();
}

