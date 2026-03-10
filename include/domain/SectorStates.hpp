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

QString toString(RiskState state);
QString toString(WeatherState state);
QString toString(TrafficState state);

QString toIconPath(RiskState state);
QString toIconPath(WeatherState state);
QString toIconPath(TrafficState state);

Q_DECLARE_METATYPE(RiskState);
Q_DECLARE_METATYPE(WeatherState);
Q_DECLARE_METATYPE(TrafficState);
