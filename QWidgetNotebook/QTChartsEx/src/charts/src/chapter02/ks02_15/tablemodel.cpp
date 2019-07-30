/*!
* Copyright (C) 2018 女儿叫老白
* 版权所有。
* 代码仅用于课程《Qt Charts入门与提高》的学习，请勿传播。
* 免责声明:代码不保证稳定性，请勿用作商业用途，否则后果自负。

\file: tablemodel.cpp
\brief 示例cpp文件

\author 女儿叫老白  星点分享: http://xingdianketang.cn/
\Date 2018/9
*/
#include "tablemodel.h"
#include <QtCore/QVector>
#include <QtCore/QRandomGenerator>
#include <QtCore/QRect>
#include <QtGui/QColor>

CTableModel::CTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_columnCount = 4;
    m_rowCount = 15;

    // m_data
    for (int i = 0; i < m_rowCount; i++) {
        QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
        for (int k = 0; k < dataVec->size(); k++) {
            if (k % 2 == 0)
                dataVec->replace(k, i * 50 + QRandomGenerator::global()->bounded(20));
            else
                dataVec->replace(k, QRandomGenerator::global()->bounded(100*((k+1)/2)));
        }
        m_data.append(dataVec);
    }
}

int CTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int CTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::SizeHintRole) {
		if (orientation == Qt::Horizontal) {
			QVariant var = QSizeF(60, 30); // 该处仅影响列标题的高度，宽度通过QHeaderView::resizeSection()接口修改。
			return var;
		}
	}
	if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section % 2 == 0)
            return "x";
        else
            return "y";
    } else {
        return QString("%1").arg(section + 1);
    }
}

QVariant CTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        for (const QRect &rect : m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        // 默认用白色
        return QColor(Qt::white);
	}

    return QVariant();
}

bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CTableModel::addMapping(QString color, QRect area)
{
    m_mapping.insertMulti(color, area);
}
