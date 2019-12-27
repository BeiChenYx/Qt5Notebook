#include "clineitem.h"

CLineItem::CLineItem(QObject *parent) : QObject(parent)
{
    this->setFlag(ItemIsSelectable);
    this->setAcceptHoverEvents(true);
    color = Qt::white;
}

CLineItem::~CLineItem()
{

}
