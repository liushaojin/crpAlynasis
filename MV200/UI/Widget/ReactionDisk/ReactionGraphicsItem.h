#ifndef REACTIONGRAPHICSITEM_H
#define REACTIONGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsSceneDragDropEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItem>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPainter>
#include <QRectF>
#include <QPen>
#include <QVarLengthArray>
#include <QGraphicsView>
#include <QMimeData>
#include <QPointF>
#include <math.h>

class ReactionDiskGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit ReactionDiskGraphicsItem(double radius, int no);
    ~ReactionDiskGraphicsItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void setStatus(QString objName);    //设置状态
    void setTxtColor_FontSize(QColor color, int size);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void slt_CurrentID(int id);  //信号：当前选中的id号

private:
    int id;           //文本内容
    QColor txtColor;  //文本颜色
    int txtFontSize;  //文本字体大小
    double m_radius;  //半径
    QString pixMap;   //显示位图
};

#endif // REACTIONGRAPHICSITEM_H
