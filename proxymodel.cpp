#include "proxymodel.h"

ProxyModel::ProxyModel()
{

}

bool ProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    return true;
}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return true;
}
