#ifndef COMCONFIG_H
#define COMCONFIG_H

#include <QWidget>

namespace Ui {
class ComConfig;
}

class ComConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ComConfig(QWidget *parent = nullptr);
    ~ComConfig();

private:
    Ui::ComConfig *ui;
};

#endif // COMCONFIG_H
