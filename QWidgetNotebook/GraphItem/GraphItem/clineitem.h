#ifndef CLINEITEM_H
#define CLINEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>



class CLineItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    CLineItem(QObject *parent = nullptr);
    ~CLineItem(void);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setLineItem(QPointF startP, QPointF endP);
    void setColor(QColor color);

private:
    void createPointNodes();

private:
    QPointF endP;
    QPointF points[3];
    QColor color;
};

#endif // CLINEITEM_H
