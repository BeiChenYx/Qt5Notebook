#include "drilldownslice.h"
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>

QT_CHARTS_USE_NAMESPACE

CDrilldownSlice::CDrilldownSlice(const QString& strLabel, qreal value, QBarSeries *drilldownSeries)
    : m_drilldownSeries(drilldownSeries)
{
    setValue(value);
	setLabel(strLabel);
    setLabelFont(QFont("����", 9));
	setLabelColor(Qt::white);
    connect(this, &CDrilldownSlice::hovered, this, &CDrilldownSlice::showHighlight);
}

CDrilldownSlice::~CDrilldownSlice()
{

}

QAbstractSeries *CDrilldownSlice::drilldownSeries() const
{
    return m_drilldownSeries;
}

void CDrilldownSlice::showHighlight(bool show)
{
    setLabelVisible(show);
	if (show) {
		QBrush brush = this->brush();
		m_originalBrush = brush;
		brush.setColor(brush.color().lighter());
		setBrush(brush);

		// ����ͼ����ɫ�뱾�п�ͳһ��
		QList<QBarSet *> barSets = m_drilldownSeries->barSets();
		if (barSets.size() > 0)
		{
			barSets.at(0)->setBrush(m_originalBrush);
		}
	}
	else {
		setBrush(m_originalBrush);
	}
}

