#ifndef REALHUMITURE_H
#define REALHUMITURE_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QTimer>
#include "task.h"


class RealHumiture : public QObject
{
    Q_OBJECT
public:
    explicit RealHumiture(QObject *parent = nullptr);

signals:
    void humiture(int deviceAddr, qint64, double temperature, double humidity);


public slots:
    void onHomeResult(QVariant msg);

private:
};

#endif // REALHUMITURE_H
