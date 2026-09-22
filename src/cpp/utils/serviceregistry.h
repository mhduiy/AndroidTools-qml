#pragma once

#include <QObject>

class ServiceRegistry : public QObject
{
    Q_OBJECT
public:
    explicit ServiceRegistry(QObject *parent = nullptr);
    ~ServiceRegistry();

    void initialize();

private:
    void initLeafServices();
    void initADBInfrastructure();
    void initPageTools();
    void initScrcpyServices();
};
