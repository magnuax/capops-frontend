#pragma once
#include <QObject>

class IFlightDataEvents : public QObject
{
    Q_OBJECT

signals:
    void dataReloaded();

public:
    using QObject::QObject;
    virtual ~IFlightDataEvents() = default;
};