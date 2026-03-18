#pragma once
#include <QMetaType>

enum class RiskState
{
    NORMAL,
    AT_RISK,
    CONGESTED,
    UNKNOWN
};

enum class WeatherState
{
    OK,
    DEGRADED,
    SEVERE,
    EXTREME,
    UNKNOWN
};

enum class TrafficState
{
    LIGHT,
    MODERATE,
    HEAVY,
    UNKNOWN
};

QString toString(RiskState state);
QString toString(WeatherState state);
QString toString(TrafficState state);

RiskState riskStateFromString(const QString &str);
WeatherState weatherStateFromString(const QString &str);
TrafficState trafficStateFromString(const QString &str);

QString toIconPath(RiskState state);
QString toIconPath(WeatherState state);
QString toIconPath(TrafficState state);

Q_DECLARE_METATYPE(RiskState);
Q_DECLARE_METATYPE(WeatherState);
Q_DECLARE_METATYPE(TrafficState);
