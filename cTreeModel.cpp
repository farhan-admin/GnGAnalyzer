#include "cTreeModel.h"


cTreeModel::cTreeModel(const QStringList& headers, const QString& data, QObject* parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (const QString& header : headers)
        rootData << header;

    rootItem = new cTreeItem(rootData);
    //setupModelData(data.split('\n'), rootItem);
}


cTreeModel::~cTreeModel()
{
    delete rootItem;
}

cTreeItem* cTreeModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        cTreeItem* item = static_cast<cTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

int cTreeModel::rowCount(const QModelIndex& parent) const
{
    const cTreeItem* parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int cTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

Qt::ItemFlags cTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QModelIndex cTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    cTreeItem* parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    cTreeItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex cTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    cTreeItem* childItem = getItem(index);
    cTreeItem* parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}