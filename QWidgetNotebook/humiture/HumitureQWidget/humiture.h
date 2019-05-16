#ifndef HUMITURE_H
#define HUMITURE_H

#include <QWidget>

namespace Ui {
class Humiture;
}

class Humiture : public QWidget
{
    Q_OBJECT

public:
    explicit Humiture(QWidget *parent = nullptr);
    ~Humiture();

    void initUI();

private:
    Ui::Humiture *ui;
};

#endif // HUMITURE_H
