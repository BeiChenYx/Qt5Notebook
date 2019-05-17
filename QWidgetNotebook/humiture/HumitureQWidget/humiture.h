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

    // 缩放 宽高比例 16:9, 正常尺寸是 960 * 768
    // 最大尺寸 1920 * 1080 (背景图像素)
    double  m_rate;
};

#endif // HUMITURE_H
