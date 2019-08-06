#include "launchermodel.h"

LauncherModel::LauncherModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int LauncherModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant LauncherModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

QHash<int, QByteArray> LauncherModel::roleNames() const
{
    return QHash<int,QByteArray>();
}

