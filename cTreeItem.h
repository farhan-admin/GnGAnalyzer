#pragma once
#include <QVector>
#include <QVariant>


/// <summary>
/// Class taken from 
/// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html
/// </summary>
class cTreeItem
{
    public:
        explicit cTreeItem(const QVector<QVariant>& data, cTreeItem* parent = nullptr);
        ~cTreeItem();

        cTreeItem* child(int number);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        bool insertChildren(int position, int count, int columns);
        bool insertColumns(int position, int columns);
        cTreeItem* parent();
        bool removeChildren(int position, int count);
        bool removeColumns(int position, int columns);
        int childNumber() const;
        bool setData(int column, const QVariant& value);

    private:
        QVector<cTreeItem*> childItems;
        QVector<QVariant> itemData;
        cTreeItem* parentItem;

};

