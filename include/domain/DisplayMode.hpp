#pragma once
#include <QMetaType>

enum class DisplayMode
{
    RISK,
    WEATHER,
    TRAFFIC,
    NONE
};

Q_DECLARE_METATYPE(DisplayMode);
