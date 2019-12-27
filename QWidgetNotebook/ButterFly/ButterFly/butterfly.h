#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

class Butterfly : public QObject, QGraphicsItem
{
    Q_OBJECT
public:
    explicit Butterfly(QObject *parent = nullptr);
    void timerEvent(QTimerEvent *);
    QRectF boundingRect() const;

signals:

public slots:
protected:
    void paint(QPainter *paiter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

};

#endif // BUTTERFLY_H
