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