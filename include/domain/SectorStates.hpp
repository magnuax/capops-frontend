#pragma once
#include <QMetaType>

enum RiskState
{
    NORMAL,
    AT_RISK,
    CONGESTED,
};

enum WeatherState
{
    OK,
    DEGRADED,
    SEVERE,
    EXTREME,
};

enum TrafficState
{
    LIGHT,
    MODERATE,
    HEAVY,
};

Q_DECLARE_METATYPE(RiskState);
Q_DECLARE_METATYPE(WeatherState);
Q_DECLARE_METATYPE(TrafficState);
