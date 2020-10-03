#ifndef SETFILTER_H
#define SETFILTER_H

#include <QWidget>
#include "handleserialprotocol.h"
#include "handlehid.h"

namespace Ui {
class SetFilter;
}

class SetFilter : public QWidget
{
    Q_OBJECT

public:
    explicit SetFilter(QWidget *parent = nullptr);
    ~SetFilter();
    void setHandle(HandleHid *pHanle){
        m_pHidWorkThread = pHanle;
    }
    void setHandleSerial(HandleSerialProtocol *pHanle){
        m_pHandleSerial = pHanle;
    }
    void onFilters(QList<QStringList> &filter);
    void onFilters(QVector<FilterConfig> &filterData);
    void onClearText();

signals:
    void readFilter();

public slots:
    void onSerialStatus(bool status);
    void onHidStatus(bool status);

private slots:
    void on_pushButton_filter_clicked();
    void on_pushButton_clicked();
    void on_pushButton_clear_filter_clicked();
    void setHidFilter();
    void clearHidFilter();
    void setSerialFilter();

private:
    Ui::SetFilter *ui;
    HandleHid *m_pHidWorkThread;
    HandleSerialProtocol *m_pHandleSerial;
    bool m_isSerial = false;
    bool m_isHid = false;
};

#endif // SETFILTER_H
