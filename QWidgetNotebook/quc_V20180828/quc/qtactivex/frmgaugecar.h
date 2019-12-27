#ifndef FRMGAUGECAR_H
#define FRMGAUGECAR_H

#include <QWidget>

namespace Ui {
class frmGaugeCar;
}

class frmGaugeCar : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("ClassID",     "{4dc3f340-a6f7-44e4-a79b-3e9217695fb5}")
    Q_CLASSINFO("InterfaceID", "{9ee49617-7d5c-441a-b833-4b068d40d751}")
    Q_CLASSINFO("EventsID",    "{13eca64b-ee2a-4f3c-aa04-5d9d975979a7}")

public:
    explicit frmGaugeCar(QWidget *parent = 0);
    ~frmGaugeCar();

private:
    Ui::frmGaugeCar *ui;

private slots:
    void initForm();

};

#endif // FRMGAUGECAR_H
