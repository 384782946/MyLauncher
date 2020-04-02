#ifndef LAUNCHERMODEL_H
#define LAUNCHERMODEL_H

#include <QAbstractListModel>
#include <QDateTime>

class LauncherItem{
    enum DataRole{
        DR_NAME,
        DR_PATH,
        DR_TYPE,
        DR_RECENTCOUNT,
        DR_LASTOPRATOR
    };

public:
    QString name;
    QString path;
    QDateTime lastOprator; //上次操作时间
    int recentCount; //最近使用频率
    int type; //类型：1 目录 2 文件
    QString extra; //备用字段
};

class LauncherModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LauncherModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

private:
};

#endif // LAUNCHERMODEL_H
