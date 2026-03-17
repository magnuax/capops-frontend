#include "domain/SectorStates.hpp"

QString toString(RiskState state)
{
    switch (state)
    {
        case NORMAL:
            return "Normal";
        case AT_RISK:
            return "At Risk";
        case CONGESTED:
            return "Congested";
        default:
            return "Unknown";
    }
}

QString toString(WeatherState state)
{
    switch (state)
    {
        case OK:
            return "Ok";
        case DEGRADED:
            return "Degraded";
        case SEVERE:
            return "Severe";
        case EXTREME:
            return "Extreme";
        default:
            return "Unknown";
    }
}

QString toString(TrafficState state)
{
    switch (state)
    {
        case LIGHT:
            return "Light";
        case MODERATE:
            return "Moderate";
        case HEAVY:
            return "Heavy";
        default:
            return "Unknown";
    }
}

RiskState riskStateFromString(const QString &str)
{
    QString upperStr = str.toUpper();

    if (upperStr == "NORMAL")
        return NORMAL;
    else if (upperStr == "AT_RISK" || upperStr == "AT RISK")
        return AT_RISK;
    else if (upperStr == "CONGESTED")
        return CONGESTED;
    else
        return NORMAL;
}

WeatherState weatherStateFromString(const QString &str)
{
    QString upperStr = str.toUpper();

    if (upperStr == "OK")
        return OK;
    else if (upperStr == "DEGRADED")
        return DEGRADED;
    else if (upperStr == "SEVERE")
        return SEVERE;
    else if (upperStr == "EXTREME")
        return EXTREME;
    else
        return OK;
}

TrafficState trafficStateFromString(const QString &str)
{
    QString upperStr = str.toUpper();

    if (upperStr == "LIGHT")
        return LIGHT;
    else if (upperStr == "MODERATE")
        return MODERATE;
    else if (upperStr == "HEAVY")
        return HEAVY;
    else
        return LIGHT;
}


QString toIconPath(RiskState state)
{
    switch (state)
    {
        case NORMAL:
            return ":/icons/warning-level-0.png";
        case AT_RISK:
            return ":/icons/warning-level-1.png";
        case CONGESTED:
            return ":/icons/warning-level-2.png";
        default:
            return ":/icons/warning-level-0.png";
    }
}

QString toIconPath(WeatherState state)
{
    switch (state)
    {
        case OK:
            return ":/icons/warning-level-0.png";
        case DEGRADED:
            return ":/icons/warning-level-1.png";
        case SEVERE:
            return ":/icons/warning-level-1.png";
        case EXTREME:
            return ":/icons/warning-level-2.png";
        default:
            return ":/icons/warning-level-0.png";
    }
}

QString toIconPath(TrafficState state)
{
    switch (state)
    {
        case LIGHT:
            return ":/icons/warning-level-0.png";
        case MODERATE:
            return ":/icons/warning-level-1.png";
        case HEAVY:
            return ":/icons/warning-level-2.png";
        default:
            return ":/icons/warning-level-0.png";
    }
}