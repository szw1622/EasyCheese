#include "ifstatement.h"

IfStatement::IfStatement(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant IfStatement::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex IfStatement::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex IfStatement::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int IfStatement::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int IfStatement::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant IfStatement::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
