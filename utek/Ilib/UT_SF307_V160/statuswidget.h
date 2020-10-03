#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);
    ~StatusWidget();
    void setInfo(QString hardware, QString mac, QString ip);

private:
    Ui::StatusWidget *ui;
};

#endif // STATUSWIDGET_H
