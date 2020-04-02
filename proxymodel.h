#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>

class ProxyModel:public QSortFilterProxyModel
{
public:
    ProxyModel();

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // PROXYMODEL_H
