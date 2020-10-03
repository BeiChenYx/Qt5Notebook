#ifndef HOVERINFOWIDGET_H
#define HOVERINFOWIDGET_H

#include <QWidget>

namespace Ui {
class HoverInfoWidget;
}

class HoverInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HoverInfoWidget(QWidget *parent = nullptr,
                             QString name="",
                             QString value="***",
                             QString symbol="°C");
    ~HoverInfoWidget();
    void setValue(QString value);
    void setName(QString name);
    void setSymbol(QString value);

private:
    Ui::HoverInfoWidget *ui;
};

#endif // HOVERINFOWIDGET_H
