﻿#pragma execution_character_set("utf-8")

#include "rulerline.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qdebug.h"

RulerLine::RulerLine(QWidget *parent) :	QWidget(parent)
{    
    minValue = 0;
    maxValue = 100;
    value = 0;

    precision = 0;
    longStep = 10;
    shortStep = 1;
    space = 20;

    animation = false;
    animationStep = 0.5;

    bgColorStart = QColor(100, 100, 100);
    bgColorEnd = QColor(60, 60, 60);
    lineColor = QColor(255, 255, 255);

    pointerColor = QColor(100, 184, 255, 180);
    pointerWidth = 3;

    reverse = false;
    currentValue = 0;
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));

    setFont(QFont("Arial", 8));
}

RulerLine::~RulerLine()
{
    if (timer->isActive()) {
        timer->stop();
    }
}

void RulerLine::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制渐变背景
    drawBg(&painter);
    //绘制标尺
    drawRuler(&painter);
    //绘制指示器
    drawPointer(&painter);
}

void RulerLine::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(QPointF(0, 0), QPointF(0, height()));
    bgGradient.setColorAt(0.0, bgColorStart);
    bgGradient.setColorAt(1.0, bgColorEnd);
    painter->setBrush(bgGradient);
    painter->drawRect(rect());
    painter->restore();
}

void RulerLine::drawRuler(QPainter *painter)
{
    painter->save();
    painter->setPen(lineColor);

    double initX = space;

    //绘制横向标尺上部分底部线
    double initTopY = space;
    QPointF lineTopLeftPot = QPointF(initX, initTopY);
    QPointF lineTopRightPot = QPointF(width() - initX, initTopY);
    painter->drawLine(lineTopLeftPot, lineTopRightPot);

    //绘制横向标尺下部分底部线
    double initBottomY = height() - space;
    QPointF lineBottomLeftPot = QPointF(initX, initBottomY);
    QPointF lineBottomRightPot = QPointF(width() - initX, initBottomY);
    painter->drawLine(lineBottomLeftPot, lineBottomRightPot);

    //绘制上部分及下部分横向标尺刻度
    double length = width() - 2 * space;
    //计算每一格移动多少
    double increment = length / (maxValue - minValue);
    //长线条短线条长度
    int longLineLen = 15;
    int shortLineLen = 10;

    //根据范围值绘制刻度值及刻度值 长线条需要移动10像素 短线条需要移动5像素
    for (int i = minValue; i <= maxValue; i = i + shortStep) {
        if (i % longStep == 0) {
            QPointF topPot = QPointF(initX, initTopY);
            QPointF bottomPot = QPointF(initX, initTopY + longLineLen);
            painter->drawLine(topPot, bottomPot);

            topPot = QPointF(initX, initBottomY);
            bottomPot = QPointF(initX, initBottomY - longLineLen);
            painter->drawLine(topPot, bottomPot);

            QString strValue = QString("%1").arg((double)i, 0, 'f', precision);
            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();

            QPointF textPot = QPointF(initX - textWidth / 2, initTopY + textHeight + longLineLen);
            painter->drawText(textPot, strValue);

            textPot = QPointF(initX - textWidth / 2, initBottomY - textHeight - longLineLen);
            painter->drawText(textPot, strValue);
        } else {
            if (i % (longStep / 2) == 0) {
                shortLineLen = 10;
            } else {
                shortLineLen = 6;
            }

            QPointF topPot = QPointF(initX, initTopY);
            QPointF bottomPot = QPointF(initX, initTopY + shortLineLen);
            painter->drawLine(topPot, bottomPot);

            topPot = QPointF(initX, initBottomY);
            bottomPot = QPointF(initX, initBottomY - shortLineLen);
            painter->drawLine(topPot, bottomPot);
        }

        initX += increment * shortStep;
    }

    painter->restore();
}

void RulerLine::drawPointer(QPainter *painter)
{
    painter->save();
    painter->setPen(QPen(pointerColor, pointerWidth));

    double length = width() - 2 * space;
    double increment = length / (maxValue - minValue);
    double initY = (currentValue - minValue) * increment + space;
    QPointF topPot(initY, space);
    QPointF bottomPot(initY, height() - space);

    painter->drawLine(topPot, bottomPot);
    painter->restore();
}

double RulerLine::getMinValue() const
{
    return this->minValue;
}

double RulerLine::getMaxValue() const
{
    return this->maxValue;
}

double RulerLine::getValue() const
{
    return this->value;
}

int RulerLine::getPrecision() const
{
    return this->precision;
}

int RulerLine::getLongStep() const
{
    return this->longStep;
}

int RulerLine::getShortStep() const
{
    return this->shortStep;
}

int RulerLine::getSpace() const
{
    return this->space;
}

bool RulerLine::getAnimation() const
{
    return this->animation;
}

double RulerLine::getAnimationStep() const
{
    return this->animationStep;
}

QColor RulerLine::getBgColorStart() const
{
    return this->bgColorStart;
}

QColor RulerLine::getBgColorEnd() const
{
    return this->bgColorEnd;
}

QColor RulerLine::getLineColor() const
{
    return this->lineColor;
}

QColor RulerLine::getPointerColor() const
{
    return this->pointerColor;
}

int RulerLine::getPointerWidth() const
{
    return this->pointerWidth;
}

QSize RulerLine::sizeHint() const
{
    return QSize(500, 120);
}

QSize RulerLine::minimumSizeHint() const
{
    return QSize(50, 50);
}

void RulerLine::setRange(double minValue, double maxValue)
{
    //如果最小值大于或者等于最大值则不设置
    if (minValue >= maxValue) {
        return;
    }

    this->minValue = minValue;
    this->maxValue = maxValue;

    //如果目标值不在范围值内,则重新设置目标值
    if (value < minValue || value > maxValue) {
        setValue(value);
    }

    update();
}

void RulerLine::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void RulerLine::setMinValue(double minValue)
{
    setRange(minValue, maxValue);
}

void RulerLine::setMaxValue(double maxValue)
{
    setRange(minValue, maxValue);
}

void RulerLine::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value) {
        return;
    }

    if (value > this->value) {
        reverse = false;
    } else if (value < this->value) {
        reverse = true;
    }

    this->value = value;
    emit valueChanged(value);

    if (!animation) {
        currentValue = this->value;
        update();
    } else {
        timer->start();
    }
}

void RulerLine::setValue(int value)
{
    setValue((double)value);
}

void RulerLine::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3 && this->precision != precision) {
        this->precision = precision;
        update();
    }
}

void RulerLine::setLongStep(int longStep)
{
    //短步长不能超过长步长
    if (longStep < shortStep) {
        return;
    }

    if (this->longStep != longStep) {
        this->longStep = longStep;
        update();
    }
}

void RulerLine::setShortStep(int shortStep)
{
    //短步长不能超过长步长
    if (longStep < shortStep) {
        return;
    }

    if (this->shortStep != shortStep) {
        this->shortStep = shortStep;
        update();
    }
}

void RulerLine::setSpace(int space)
{
    if (this->space != space) {
        this->space = space;
        update();
    }
}

void RulerLine::setAnimation(bool animation)
{
    if (this->animation != animation) {
        this->animation = animation;
        update();
    }
}

void RulerLine::setAnimationStep(double animationStep)
{
    if (this->animationStep != animationStep) {
        this->animationStep = animationStep;
        update();
    }
}

void RulerLine::setBgColorStart(const QColor &bgColorStart)
{
    if (this->bgColorStart != bgColorStart) {
        this->bgColorStart = bgColorStart;
        update();
    }
}

void RulerLine::setBgColorEnd(const QColor &bgColorEnd)
{
    if (this->bgColorEnd != bgColorEnd) {
        this->bgColorEnd = bgColorEnd;
        update();
    }
}

void RulerLine::setLineColor(const QColor &lineColor)
{
    if (this->lineColor != lineColor) {
        this->lineColor = lineColor;
        update();
    }
}

void RulerLine::setPointerColor(const QColor &pointerColor)
{
    if (this->pointerColor != pointerColor) {
        this->pointerColor = pointerColor;
        update();
    }
}

void RulerLine::setPointerWidth(int pointerWidth)
{
    if (this->pointerWidth != pointerWidth) {
        this->pointerWidth = pointerWidth;
        update();
    }
}

void RulerLine::updateValue()
{
    if (!reverse) {
        if (currentValue >= value) {
            currentValue = value;
            timer->stop();
        } else {
            currentValue += animationStep;
        }
    } else {
        if (currentValue <= value) {
            currentValue = value;
            timer->stop();
        } else {
            currentValue -= animationStep;
        }
    }

    update();
}
