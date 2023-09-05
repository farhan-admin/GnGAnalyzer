#include "cTreeItem.h"

cTreeItem::cTreeItem(const QVector<QVariant>& data, cTreeItem* parent)
    : itemData(data),
    parentItem(parent)
{}

cTreeItem::~cTreeItem()
{
    qDeleteAll(childItems);
}

cTreeItem* cTreeItem::parent()
{
    return parentItem;
}

cTreeItem* cTreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int cTreeItem::childCount() const
{
    return childItems.count();
}

int cTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<cTreeItem*>(this));
    return 0;
}

int cTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant cTreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size())
        return QVariant();
    return itemData.at(column);
}

bool cTreeItem::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

bool cTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        cTreeItem* item = new cTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool cTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool cTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (cTreeItem* child : qAsConst(childItems))
        child->insertColumns(position, columns);

    return true;
}