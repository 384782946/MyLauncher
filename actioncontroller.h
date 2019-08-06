#ifndef ACTIONCONTROLLER_H
#define ACTIONCONTROLLER_H

#include <QObject>

class ActionController : public QObject
{
    Q_OBJECT
public:
    explicit ActionController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ACTIONCONTROLLER_H