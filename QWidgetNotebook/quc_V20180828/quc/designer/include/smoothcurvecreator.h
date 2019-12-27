#ifndef SMOOTHCURVECREATOR_H
#define SMOOTHCURVECREATOR_H

#include <QList>
#include <QPointF>
#include <QPainterPath>

class SmoothCurveCreator
{
public:
	static QPainterPath createSmoothCurve(const QList<QPointF> &points);

private:
	static void calculateFirstControlPoints(double *&result, const double *rhs, int n);
	static void calculateControlPoints(const QList<QPointF> &knots,
	                                   QList<QPointF> *firstControlPoints,
	                                   QList<QPointF> *secondControlPoints);
};

#endif // SMOOTHCURVECREATOR_H
