#include "mainwidget.h"
#include <QColorDialog>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->paintArea = new PaintArea;

    // 形状选择下拉列表框
    this->shapeLabel = new QLabel(tr("形状: "));

    this->shapeComboBox = new QComboBox;
    // QComboBox 的 addItem() 函数可以仅插入文本，也可以同时插入与文本相对应
    // 的具体数据，通常为枚举型数据，便于后面操作时确定选择的是哪个数据.
    this->shapeComboBox->addItem(tr("Line"), PaintArea::Line);
    this->shapeComboBox->addItem(tr("Rectangle"), PaintArea::Rectangle);
    this->shapeComboBox->addItem(tr("RoundedRect"), PaintArea::RoundRect);
    this->shapeComboBox->addItem(tr("Ellipse"), PaintArea::Ellipse);
    this->shapeComboBox->addItem(tr("Polygon"), PaintArea::Polygon);
    this->shapeComboBox->addItem(tr("Polyline"), PaintArea::Polyline);
    this->shapeComboBox->addItem(tr("Points"), PaintArea::Points);
    this->shapeComboBox->addItem(tr("Arc"), PaintArea::Arc);
    this->shapeComboBox->addItem(tr("Path"), PaintArea::Path);
    this->shapeComboBox->addItem(tr("Text"), PaintArea::Text);
    this->shapeComboBox->addItem(tr("Pixmap"), PaintArea::Pixmap);
    connect(this->shapeComboBox, SIGNAL(activated(int)), this, SLOT(ShowShape(int)));

    // 画笔颜色选择控件
    this->penColorLabel = new QLabel(tr("画笔颜色: "));
    this->penColorFrame = new QFrame;
    this->penColorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    this->penColorFrame->setAutoFillBackground(true);
    this->penColorFrame->setPalette(QPalette(Qt::blue));
    this->penColorBtn = new QPushButton(tr("更改"));
    connect(this->penColorBtn, SIGNAL(clicked()), this, SLOT(ShowPenColor()));

    // 画笔线宽选择控件
    this->penWidthLabel = new QLabel(tr("画笔线宽: "));
    this->penWidthSpinBox = new QSpinBox;
    this->penWidthSpinBox->setRange(0, 20);
    connect(this->penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ShowPenWidth(int)));

    // 画笔风格选择下拉列表框
    this->penStyleLabel = new QLabel(tr("画笔风格: "));
    this->penStyleComboBox = new QComboBox;
    this->penStyleComboBox->addItem(tr("SolidLine"), static_cast<int>(Qt::SolidLine));
    this->penStyleComboBox->addItem(tr("DashLine"), static_cast<int>(Qt::DashLine));
    this->penStyleComboBox->addItem(tr("DotLine"), static_cast<int>(Qt::DotLine));
    this->penStyleComboBox->addItem(tr("DashDotLine"), static_cast<int>(Qt::DashDotLine));
    this->penStyleComboBox->addItem(tr("DashDotDotLine"), static_cast<int>(Qt::DashDotDotLine));
    this->penStyleComboBox->addItem(tr("CustomDashLine"), static_cast<int>(Qt::CustomDashLine));
    connect(this->penStyleComboBox, SIGNAL(activated(int)), this, SLOT(ShowPenStyle(int)));

    // 画笔顶帽风格选择下拉列表框
    this->penCapLabel = new QLabel(tr("画笔顶帽: "));
    this->penCapComboBox = new QComboBox;
    this->penCapComboBox->addItem(tr("SquareCap"), Qt::SquareCap);
    this->penCapComboBox->addItem(tr("FlatCap"), Qt::FlatCap);
    this->penCapComboBox->addItem(tr("RoundCap"), Qt::RoundCap);
    connect(this->penCapComboBox, SIGNAL(activated(int)), this, SLOT(ShowPenCap(int)));

    // 画笔连接点风格选择下拉列表框
    this->penJoinLabel = new QLabel(tr("画笔连接点: "));
    this->penJoinComboBox = new QComboBox;
    this->penJoinComboBox->addItem(tr("BevelJoin"), Qt::BevelJoin);
    this->penJoinComboBox->addItem(tr("MiterJoin"), Qt::MiterJoin);
    this->penJoinComboBox->addItem(tr("RoundJoin"), Qt::RoundJoin);
    connect(this->penJoinComboBox, SIGNAL(activated(int)), this, SLOT(ShowPenJoin(int)));

    // 填充模式选择下拉列表框
    this->fillRuleLabel = new QLabel(tr("填充模式: "));
    this->fillRuleComboBox = new QComboBox;
    this->fillRuleComboBox->addItem(tr("Odd Even"), Qt::OddEvenFill);
    this->fillRuleComboBox->addItem(tr("Winding"), Qt::WindingFill);
    connect(this->fillRuleComboBox, SIGNAL(activated(int)), this, SLOT(ShowFillRule()));

    // 铺展效果选择下拉列表框
    this->spreadLabel = new QLabel(tr("铺展效果: "));
    this->spreadComboBox = new QComboBox;
    this->spreadComboBox->addItem(tr("PadSpread"), QGradient::PadSpread);
    this->spreadComboBox->addItem(tr("RepeatSpread"), QGradient::RepeatSpread);
    this->spreadComboBox->addItem(tr("ReflectSpread"), QGradient::ReflectSpread);
    connect(this->spreadComboBox, SIGNAL(activated(int)), this, SLOT(ShowSpreadStyle()));

    // 画笔颜色选择控件
    this->brushColorLabel = new QLabel(tr("画刷颜色: "));
    this->brushColorFrame = new QFrame;
    this->brushColorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    this->brushColorFrame->setAutoFillBackground(true);
    this->brushColorFrame->setPalette(QPalette(Qt::green));
    this->brushColorBtn = new QPushButton(tr("更改"));
    connect(this->brushColorBtn, SIGNAL(clicked()), this, SLOT(ShowBrushColor()));

    // 画刷风格选择下拉列表框
    this->brushStyleLabel = new QLabel(tr("画刷风格: "));
    this->brushStyleComboBox = new QComboBox;
    this->brushStyleComboBox->addItem(tr("SolidPattern"), static_cast<int>(Qt::SolidPattern));
    this->brushStyleComboBox->addItem(tr("Dense1Pattern"), static_cast<int>(Qt::Dense1Pattern));
    this->brushStyleComboBox->addItem(tr("Dense2Pattern"), static_cast<int>(Qt::Dense2Pattern));
    this->brushStyleComboBox->addItem(tr("Dense3Pattern"), static_cast<int>(Qt::Dense3Pattern));
    this->brushStyleComboBox->addItem(tr("Dense4Pattern"), static_cast<int>(Qt::Dense4Pattern));
    this->brushStyleComboBox->addItem(tr("Dense5Pattern"), static_cast<int>(Qt::Dense5Pattern));
    this->brushStyleComboBox->addItem(tr("Dense6Pattern"), static_cast<int>(Qt::Dense6Pattern));
    this->brushStyleComboBox->addItem(tr("Dense7Pattern"), static_cast<int>(Qt::Dense7Pattern));
    this->brushStyleComboBox->addItem(tr("HorPattern"), static_cast<int>(Qt::HorPattern));
    this->brushStyleComboBox->addItem(tr("VerPattern"), static_cast<int>(Qt::VerPattern));
    this->brushStyleComboBox->addItem(tr("CrossPattern"), static_cast<int>(Qt::CrossPattern));
    this->brushStyleComboBox->addItem(tr("BDiagPattern"), static_cast<int>(Qt::BDiagPattern));
    this->brushStyleComboBox->addItem(tr("FDiagPattern"), static_cast<int>(Qt::FDiagPattern));
    this->brushStyleComboBox->addItem(tr("DiagCrossPattern"), static_cast<int>(Qt::DiagCrossPattern));
    this->brushStyleComboBox->addItem(tr("LinearGradientPattern"), static_cast<int>(Qt::LinearGradientPattern));
    this->brushStyleComboBox->addItem(tr("ConicalGradientPattern"), static_cast<int>(Qt::ConicalGradientPattern));
    this->brushStyleComboBox->addItem(tr("RadialGradientPattern"), static_cast<int>(Qt::RadialGradientPattern));
    this->brushStyleComboBox->addItem(tr("TextuarePattern"), static_cast<int>(Qt::TexturePattern));
    connect(this->brushStyleComboBox, SIGNAL(activated(int)), this, SLOT(ShowBrush(int)));

    this->rightLayout = new QGridLayout;
    this->rightLayout->addWidget(this->shapeLabel, 0, 0);
    this->rightLayout->addWidget(this->shapeComboBox, 0, 1);
    this->rightLayout->addWidget(this->penColorLabel, 1, 0);
    this->rightLayout->addWidget(this->penColorFrame, 1, 1);
    this->rightLayout->addWidget(this->penColorBtn, 1, 2);
    this->rightLayout->addWidget(this->penWidthLabel, 2, 0);
    this->rightLayout->addWidget(this->penWidthSpinBox, 2, 1);
    this->rightLayout->addWidget(this->penStyleLabel, 3, 0);
    this->rightLayout->addWidget(this->penStyleComboBox, 3, 1);
    this->rightLayout->addWidget(this->penCapLabel, 4, 0);
    this->rightLayout->addWidget(this->penCapComboBox, 4, 1);
    this->rightLayout->addWidget(this->penJoinLabel, 5, 0);
    this->rightLayout->addWidget(this->penJoinComboBox, 5, 1);
    this->rightLayout->addWidget(this->fillRuleLabel, 6, 0);
    this->rightLayout->addWidget(this->fillRuleComboBox, 6, 1);
    this->rightLayout->addWidget(this->spreadLabel, 7, 0);
    this->rightLayout->addWidget(this->spreadComboBox, 7, 1);
    this->rightLayout->addWidget(this->brushColorLabel, 8, 0);
    this->rightLayout->addWidget(this->brushColorFrame, 8, 1);
    this->rightLayout->addWidget(this->brushColorBtn, 8, 2);
    this->rightLayout->addWidget(this->brushStyleLabel, 9, 0);
    this->rightLayout->addWidget(this->brushStyleComboBox, 9 , 1);

    // 整体的布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(this->paintArea);
    mainLayout->addLayout((this->rightLayout));
    mainLayout->setStretchFactor(paintArea, 1);
    mainLayout->setStretchFactor(rightLayout, 0);

    // 显示默认图形
    this->ShowShape(this->shapeComboBox->currentIndex());
}

MainWidget::~MainWidget()
{

}

void MainWidget::ShowShape(int value)
{
    PaintArea::Shape shape = PaintArea::Shape(shapeComboBox->itemData(value, Qt::UserRole).toInt());
    paintArea->setShape(shape);
}

void MainWidget::ShowPenWidth(int value)
{
    QColor color = penColorFrame->palette().color(QPalette::Window);
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(penStyleComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(penCapComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(penJoinComboBox->currentIndex(), Qt::UserRole).toInt());
    paintArea->setPen(QPen(color, value, style, cap, join));
}

void MainWidget::ShowPenColor()
{
    QColor color = QColorDialog::getColor(static_cast<int>(Qt::blue));
    this->penColorFrame->setPalette(QPalette(color));
    int value = penWidthSpinBox->value();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(penStyleComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(penCapComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(penJoinComboBox->currentIndex(), Qt::UserRole).toInt());

    paintArea->setPen(QPen(color, value, style, cap, join));
}

void MainWidget::ShowPenStyle(int styleValue)
{
    QColor color = penColorFrame->palette().color(QPalette::Window);
    int value = penWidthSpinBox->value();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(styleValue, Qt::UserRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(penCapComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(penJoinComboBox->currentIndex(), Qt::UserRole).toInt());
    paintArea->setPen(QPen(color, value, style, cap, join));
}

void MainWidget::ShowPenCap(int capValue)
{
    QColor color = penColorFrame->palette().color(QPalette::Window);
    int value = penWidthSpinBox->value();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(penStyleComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(capValue, Qt::UserRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(penJoinComboBox->currentIndex(), Qt::UserRole).toInt());
    paintArea->setPen(QPen(color, value, style, cap, join));
}

void MainWidget::ShowPenJoin(int joinValue)
{
    QColor color = penColorFrame->palette().color(QPalette::Window);
    int value = penWidthSpinBox->value();
    Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(penStyleComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(penCapComboBox->currentIndex(), Qt::UserRole).toInt());
    Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(penJoinComboBox->currentIndex(), Qt::UserRole).toInt());
    paintArea->setPen(QPen(color, value, style, cap, join));
}

void MainWidget::ShowSpreadStyle()
{
    this->spread = QGradient::Spread(spreadComboBox->itemData(spreadComboBox->currentIndex(), Qt::UserRole).toInt());
}

void MainWidget::ShowFillRule()
{
    Qt::FillRule rule = Qt::FillRule(fillRuleComboBox->itemData(fillRuleComboBox->currentIndex(), Qt::UserRole).toInt());
    paintArea->setFillRule(rule);
}

void MainWidget::ShowBrushColor()
{
    QColor color = QColorDialog::getColor(static_cast<int>(Qt::blue));
    brushColorFrame->setPalette(QPalette(color));
    ShowBrush(brushStyleComboBox->currentIndex());
}

void MainWidget::ShowBrush(int value)
{
    QColor color = brushColorFrame->palette().color(QPalette::Window);
    Qt::BrushStyle style = Qt::BrushStyle(brushStyleComboBox->itemData(value, Qt::UserRole).toInt());
    if(style == Qt::LinearGradientPattern){
        QLinearGradient linearGradient(0, 0, 400, 400);
        linearGradient.setColorAt(0.0, Qt::white);
        linearGradient.setColorAt(0.2, color);
        linearGradient.setColorAt(1.0, Qt::black);
        linearGradient.setSpread(spread);
        paintArea->setBrush(linearGradient);
    }else if(style == Qt::RadialGradientPattern){
        QRadialGradient radialGradient(200, 200, 150, 150, 100);
        radialGradient.setColorAt(0.0, Qt::white);
        radialGradient.setColorAt(0.2, color);
        radialGradient.setColorAt(1.0, Qt::black);
        radialGradient.setSpread(spread);
        paintArea->setBrush(radialGradient);
    }else if(style == Qt::ConicalGradientPattern){
        QConicalGradient conicalGradient(200, 200, 30);
        conicalGradient.setColorAt(0.0, Qt::white);
        conicalGradient.setColorAt(0.2, color);
        conicalGradient.setColorAt(1.0, Qt::black);
        conicalGradient.setSpread(spread);
        paintArea->setBrush(conicalGradient);
    }else if(style == Qt::TexturePattern){
        paintArea->setBrush(QBrush(QPixmap("./ico.png")));
    }else{
        paintArea->setBrush(QBrush(color, style));
    }
}

