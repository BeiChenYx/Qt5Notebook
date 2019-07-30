#ifndef CDrilldownSlice_H
#define CDrilldownSlice_H

#include <QBrush>
#include <QtCharts/QPieSlice>

QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
class QBarSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class CDrilldownSlice : public QPieSlice
{
    Q_OBJECT

public:
    CDrilldownSlice(const QString& strLabel, qreal value, QBarSeries *drilldownSeries);
    virtual ~CDrilldownSlice();
	QAbstractSeries *drilldownSeries() const;

public Q_SLOTS:
    void showHighlight(bool show);

private:
	QBarSeries *m_drilldownSeries;
	QBrush m_originalBrush;
};

#endif // CDrilldownSlice_H
