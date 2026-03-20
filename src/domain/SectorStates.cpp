#include "domain/SectorStates.hpp"

QString toString(RiskState state)
{
    switch (state)
    {
    case RiskState::NORMAL:
        return "Normal";
    case RiskState::AT_RISK:
        return "At Risk";
    case RiskState::CONGESTED:
        return "Congested";
    default:
        return "Unknown";
    }
}

QString toString(WeatherState state)
{
    switch (state)
    {
    case WeatherState::OK:
        return "Ok";
    case WeatherState::DEGRADED:
        return "Degraded";
    case WeatherState::SEVERE:
        return "Severe";
    case WeatherState::EXTREME:
        return "Extreme";
    default:
        return "Unknown";
    }
}

QString toString(TrafficState state)
{
    switch (state)
    {
    case TrafficState::LIGHT:
        return "Light";
    case TrafficState::MODERATE:
        return "Moderate";
    case TrafficState::HEAVY:
        return "Heavy";
    default:
        return "Unknown";
    }
}

RiskState riskStateFromString(const QString &str)
{
    QString upperStr = str.toUpper();

    if (upperStr == "NORMAL")
        return RiskState::NORMAL;
    else if (upperStr == "AT_RISK" || upperStr == "AT RISK")
        return RiskState::AT_RISK;
    else if (upperStr == "CONGESTED")
        return RiskState::CONGESTED;
    else
        return RiskState::UNKNOWN;
}

WeatherState weatherStateFromString(const QString &str)
{
    QString upperStr = str.toUpper();

    if (upperStr == "OK")
        return WeatherState::OK;
    else if (upperStr == "DEGRADED")
        return WeatherState::DEGRADED;
    else if (upperStr == "SEVERE")
        return WeatherState::SEVERE;
    else if (upperStr == "EXTREME")
        return WeatherState::EXTREME;
    else
        return WeatherState::UNKNOWN;
}

TrafficState trafficStateFromString(const QString &str)
{
    QString upperStr = str.toUpper();

    if (upperStr == "LIGHT")
        return TrafficState::LIGHT;
    else if (upperStr == "MODERATE")
        return TrafficState::MODERATE;
    else if (upperStr == "HEAVY")
        return TrafficState::HEAVY;
    else
        return TrafficState::UNKNOWN;
}

TrafficState computeTrafficState(int aircraftCount, int baseCapacity)
{
    double loadFraction = static_cast<double>(aircraftCount) / baseCapacity;

    if (loadFraction < 0.4)
        return TrafficState::LIGHT;
    else if (loadFraction < 0.8)
        return TrafficState::MODERATE;
    else
        return TrafficState::HEAVY;
}

QString toIconPath(RiskState state)
{
    switch (state)
    {
    case RiskState::NORMAL:
        return ":/icons/warning-level-0.png";
    case RiskState::AT_RISK:
        return ":/icons/warning-level-1.png";
    case RiskState::CONGESTED:
        return ":/icons/warning-level-2.png";
    default:
        return ":/icons/warning-level-0.png";
    }
}

QString toIconPath(WeatherState state)
{
    switch (state)
    {
    case WeatherState::OK:
        return ":/icons/warning-level-0.png";
    case WeatherState::DEGRADED:
        return ":/icons/warning-level-1.png";
    case WeatherState::SEVERE:
        return ":/icons/warning-level-1.png";
    case WeatherState::EXTREME:
        return ":/icons/warning-level-2.png";
    default:
        return ":/icons/warning-level-0.png";
    }
}

QString toIconPath(TrafficState state)
{
    switch (state)
    {
    case TrafficState::LIGHT:
        return ":/icons/warning-level-0.png";
    case TrafficState::MODERATE:
        return ":/icons/warning-level-1.png";
    case TrafficState::HEAVY:
        return ":/icons/warning-level-2.png";
    default:
        return ":/icons/warning-level-0.png";
    }
}
