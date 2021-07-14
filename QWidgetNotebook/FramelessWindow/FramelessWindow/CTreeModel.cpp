#include "CTreeModel.h"


NodeIndex::NodeIndex()
{

}

NodeIndex::NodeIndex(const QString path, const NodeWPtr &parent/*=NodeWPtr()*/)
    : m_pParent(parent),
      m_path(path),
      m_rowNoInParent(0)
{
    if (!m_path.isEmpty() && !m_path.endsWith("/")) {
        m_name = m_path.mid(m_path.lastIndexOf("/") + 1);
    }
}
NodeIndex::~NodeIndex()
{
    m_childrenNode.clear();
    m_pParent.reset();
    m_data.clear();
}


NodeIndex::NodeIndex(const NodeIndex &rhs)
{
    this->m_childrenNode = rhs.m_childrenNode;
    this->m_pParent = rhs.m_pParent;
    this->m_name = rhs.m_name;
    this->m_path = rhs.m_path;
    this->m_rowNoInParent = rhs.m_rowNoInParent;
    this->m_data = rhs.m_data;
}


NodeIndex& NodeIndex::operator=(const NodeIndex &rhs)
{
    if (this != &rhs) {
        this->m_childrenNode.clear();
        this->m_data.clear();
        this->m_childrenNode = rhs.m_childrenNode;
        this->m_pParent = rhs.m_pParent;
        this->m_name = rhs.m_name;
        this->m_path = rhs.m_path;
        this->m_rowNoInParent = rhs.m_rowNoInParent;
        this->m_data = rhs.m_data;
    }
    return *this;
}

int NodeIndex::rowNoInParent() const
{
    return m_rowNoInParent;
}

void NodeIndex::setRowNoInParent(const int &rowNo)
{
    m_rowNoInParent = rowNo;
}
QString NodeIndex::name()
{
    return m_name;
}

QString NodeIndex::path()
{
    return m_path;
}

bool NodeIndex::setData(const QString &key, const QVariant &data)
{
    m_data[key] = data;
    return true;
}


QVariant NodeIndex::data(const QString &key, const QVariant &defaultData)
{
    if (m_data.find(key) == m_data.cend()) {
        return defaultData;
    }
    return m_data.at(key);
}

NodeWPtr NodeIndex::parent() const
{
    return m_pParent;
}

NodePtr NodeIndex::child(const int &index) const
{
    if (static_cast<size_t>(index) < m_childrenNode.size() && index >= 0) {
        return m_childrenNode.at(index);
    }
    return std::make_shared<NodeIndex>();
}

NodePtr NodeIndex::child(const QString &name) const
{
    if (name.isEmpty()) {
        return std::make_shared<NodeIndex>();
    }
    auto pItem = std::find_if(m_childrenNode.cbegin(), m_childrenNode.cend(),
                              [name](const NodePtr &item){
        return item->name() == name;
    });
    if (pItem == m_childrenNode.cend()) {
        return std::make_shared<NodeIndex>();
    }
    return *pItem;
}

int NodeIndex::chilrenCount() const
{
    return static_cast<int>(m_childrenNode.size());
}
//bool NodeIndex::exchangeChild(const int &index, const int &otherIndex)

//{
//}

bool NodeIndex::removeChiled(const int &index)
{
    int maxSize = static_cast<int>(m_childrenNode.size());
    if (index < maxSize && index >= 0) {
        m_childrenNode.erase(m_childrenNode.begin() + index);
        return true;
    }
    return false;
}

void NodeIndex::addChild(const NodePtr &pNode)
{
    m_childrenNode.push_back(pNode);
    pNode->setRowNoInParent(m_childrenNode.size());
}


CTreeModel::CTreeModel(QAbstractItemModel *parent/* = nullptr*/)
    : QAbstractItemModel(parent),
      m_pRootItem(std::make_shared<NodeIndex>("/root"))
{
    m_headerName = {"A", "B", "C", "D", "E", "F"};
    auto headerSize = m_headerName.size();
    for (int i=0; i<10; ++i) {
        auto path = QString("%1/%2").arg(m_pRootItem->path()).arg(QString::number(i));
        auto pTmp = std::make_shared<NodeIndex>(path, m_pRootItem);

        for (size_t col=0; col<headerSize; ++col) {
            auto data = m_headerName.at(col);
            pTmp->setData(data, QString("%1%2%3").arg(data).arg(i).arg(col));
        }
        m_pRootItem->addChild(pTmp);
        for (int j=0; j<5; ++j) {
            auto childPath = QString("%1/%2").arg(path).arg(QString::number(i*10 + j + 1));
            auto pChildTmp = std::make_shared<NodeIndex>(childPath, pTmp);
            pTmp->addChild(pChildTmp);
            for (size_t col=0; col<headerSize; ++col) {
                auto data = m_headerName.at(col);
                pChildTmp->setData(data, QString("%1%2%3").arg(data).arg(data).arg(col));
            }
        }
    }
}
NodeIndex* CTreeModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        return static_cast<NodeIndex*>(index.internalPointer());
    }
    return m_pRootItem.get();
}

int CTreeModel::rowCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
    if (parent.column() > 0) {
        return 0;
    }

    auto item = this->itemFromIndex(parent);
    return item->chilrenCount();
}

int CTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headerName.size();
}

QVariant CTreeModel::data(const QModelIndex &index, int role/* = Qt::DisplayRole*/) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        auto col = index.column();
        auto item = this->itemFromIndex(index);
        return item->data(m_headerName.at(col), "");
    }
    return QVariant();
}

bool CTreeModel::setData(const QModelIndex &index,
                         const QVariant &value, int role/* = Qt::EditRole*/)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

QModelIndex CTreeModel::index(int row, int column,
                              const QModelIndex &parent/* = QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    auto parentItem = this->itemFromIndex(parent);
    NodePtr item = parentItem->child(row);
    if (item != nullptr) {
        return createIndex(row, column, item.get());
    } else {
        return QModelIndex();
    }
}

QModelIndex CTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) { return QModelIndex(); }

    NodeIndex *item = this->itemFromIndex(index);
    NodeWPtr parentItem = item->parent();

    auto pParentItem = parentItem.lock();
    if (pParentItem == m_pRootItem || pParentItem == nullptr) {
        return QModelIndex();
    }
    return createIndex(pParentItem->rowNoInParent(), 0, pParentItem.get());
}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    if(orientation == Qt::Horizontal){
        if(section < 0 || section > static_cast<int>(m_headerName.size())) {
            return  QVariant();
        }
        return QVariant(m_headerName.at(section));
    }else {
        return QVariant(QString::number(section));
    }
}
bool CTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role != Qt::EditRole){
        return false;
    }
    if(orientation == Qt::Horizontal){
        m_headerName[section] = value.toString();
        emit headerDataChanged(Qt::Horizontal, section, section);
        return true;
    }else{
        return false;
    }
}


