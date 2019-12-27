#ifndef FRMPROPERTY_H
#define FRMPROPERTY_H

#include <QWidget>

namespace Ui {
class frmProperty;
}

class frmProperty : public QWidget
{
    Q_OBJECT

public:
    explicit frmProperty(QWidget *parent = 0);
    ~frmProperty();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *);

private:
    Ui::frmProperty *ui;
    QPixmap bgPix;

private slots:
    void initForm();

private slots:
    void on_btnOpen_clicked();
    void on_btnSave_clicked();
};

#endif // FRMPROPERTY_H
