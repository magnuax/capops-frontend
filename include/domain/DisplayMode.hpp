#pragma once
#include <QMetaType>

enum DisplayMode
{
    RISK,
    WEATHER,
    TRAFFIC,
    NONE
};

Q_DECLARE_METATYPE(DisplayMode);
