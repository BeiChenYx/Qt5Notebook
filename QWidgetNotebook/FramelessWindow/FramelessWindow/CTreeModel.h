#ifndef CTREEMODEL_H
#define CTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QWidget>
#include <vector>
#include <list>
#include <QDateTime>
#include <tuple>
#include <functional>
#include <memory>

class NodeIndex;
using NodePtr = std::shared_ptr<NodeIndex>;
using NodeWPtr = std::weak_ptr<NodeIndex>;

class NodeIndex
{
public:
    NodeIndex();
    NodeIndex(const QString path,
              const NodeWPtr &parent=NodeWPtr());
    ~NodeIndex();
    NodeIndex(const NodeIndex &rhs);
    NodeIndex& operator=(const NodeIndex &rhs);

    int rowNoInParent() const;
    void setRowNoInParent(const int &rowNo);

    QString name();
    QString path();

    bool setData(const QString &key, const QVariant &data);
    QVariant data(const QString &key, const QVariant &defaultData);

    NodeWPtr parent() const;
    NodePtr child(const int &index) const;
    NodePtr child(const QString &name) const;
    int chilrenCount() const;
//    bool exchangeChild(const int &index, const int &otherIndex);
    bool removeChiled(const int &index);
    void addChild(const NodePtr &pNode);

private:
    std::vector<NodePtr> m_childrenNode;
    NodeWPtr m_pParent;
    QString m_name;
    QString m_path;
    int m_rowNoInParent;
    std::map<QString, QVariant> m_data;
};


class CTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CTreeModel(QAbstractItemModel *parent = nullptr);
    NodeIndex* itemFromIndex(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

signals:

private:
    std::pair<NodePtr, bool> findNodeFromPath(const QStringList &path,
                                              const NodePtr &parent=NodePtr()) const;

private:
    std::vector<QString> m_headerName;
    NodePtr m_pRootItem;
};

#endif // CTREEMODEL_H
