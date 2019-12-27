#include "paintarea.h"
#include <QPainter>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    // 完成对窗体背景色的设置
    this->setPalette(QPalette(Qt::white));
    this->setAutoFillBackground(true);

    setMinimumSize(400, 400);
}

void PaintArea::setShape(Shape s)
{
    this->shape = s;
    // 重画绘制区窗体
    this->update();
}

void PaintArea::setPen(QPen p)
{
    this->pen = p;
    this->update();
}

void PaintArea::setBrush(QBrush b)
{
    this->brush = b;
    this->update();
}

void PaintArea::setFillRule(Qt::FillRule rule)
{
    this->fillRule = rule;
    this->update();
}

void PaintArea::paintEvent(QPaintEvent *)
{
    // 新建一个 QPainter 对象
    QPainter p(this);
    // 设置 QPainter 对象的画笔和画刷
    p.setPen(this->pen);
    p.setBrush(this->brush);

    // 设置一个方形区域，为画长方形、圆角方形、椭圆等做准备.
    QRect rect(50, 100, 300, 200);

    // 创建一个 QPoint 的数组，包含四个点，为画多边形、多边线及点做准备
    static const QPoint points[4] = {
        QPoint(150, 100),
        QPoint(300, 150),
        QPoint(350, 250),
        QPoint(100, 300)
    };
    // startAngle表示起始角，为弧形的起始点与圆心之间连线与水平方向的夹角
    // spanAngle表示的是跨度角，为弧形起点、终点分别与圆心连线之间的夹角
    // 用Qpainter画弧形所使用的角度值，是以 1/16 度为单位的，在画弧时即
    // 1° 用 16 表示
    int startAngle = 30 * 16; // 30 度
    int spanAngle = 120 * 16; // 120 度

    // QPainterPath 新建一个 QPainterPath 对象，为画路径做准备
    QPainterPath path;
    path.addRect(150, 150, 100, 100);
    path.moveTo(100, 100);

    // cubicTo() 函数绘制的是贝塞尔曲线
    path.cubicTo(300, 100, 200, 200, 300, 300);
    path.cubicTo(100, 300, 200, 200, 100, 100);
    path.setFillRule(fillRule);

    // 根据 shape 判断需要画的形状调用不同的 draw() 函数
    switch (this->shape) {
    case Line:
        p.drawLine(rect.topLeft(),rect.bottomRight());
        break;
    case Rectangle:
        p.drawRect(rect);
        break;
    case RoundRect:
        p.drawRoundRect(rect);
        break;
    case Ellipse:
        p.drawEllipse(rect);
        break;
    case Polygon:
        p.drawPolygon(points, 4);
        break;
    case Polyline:
        p.drawPolyline(points, 4);
        break;
    case Points:
        p.drawPoints(points, 4);
        break;
    case Arc:
        p.drawArc(rect, startAngle, spanAngle);
        break;
    case Path:
        p.drawPath(path);
        break;
    case Text:
        p.drawText(rect, Qt::AlignCenter, tr("Hello Qt!"));
        break;
    case Pixmap:
        p.drawPixmap(150, 150, QPixmap("./ico.png"));
        break;
    default:
        ;
        break;
    }
}








