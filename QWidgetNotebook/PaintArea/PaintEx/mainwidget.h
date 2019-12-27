#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGradient>
#include "paintarea.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected slots:
    void ShowShape(int);
    void ShowPenWidth(int);
    void ShowPenColor();
    void ShowPenStyle(int);
    void ShowPenCap(int);
    void ShowPenJoin(int);
    void ShowSpreadStyle();
    void ShowFillRule();
    void ShowBrushColor();
    void ShowBrush(int);

private:
    PaintArea *paintArea;

    QLabel *shapeLabel;
    QComboBox *shapeComboBox;

    QLabel *penWidthLabel;
    QSpinBox *penWidthSpinBox;

    QLabel *penColorLabel;
    QFrame *penColorFrame;
    QPushButton *penColorBtn;

    QLabel *penStyleLabel;
    QComboBox *penStyleComboBox;

    QLabel *penCapLabel;
    QComboBox *penCapComboBox;

    QLabel *penJoinLabel;
    QComboBox *penJoinComboBox;

    QLabel *fillRuleLabel;
    QComboBox *fillRuleComboBox;

    QLabel *spreadLabel;
    QComboBox *spreadComboBox;

    QGradient::Spread spread;

    QLabel *brushStyleLabel;
    QComboBox *brushStyleComboBox;

    QLabel *brushColorLabel;
    QFrame *brushColorFrame;
    QPushButton *brushColorBtn;

    QGridLayout *rightLayout;
};

#endif // MAINWIDGET_H
