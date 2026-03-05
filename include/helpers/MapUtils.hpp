#pragma once
#include <QObject>
#include <QPixmap>
#include <cmath>

const double MAX_LAT = 85.05112878;
const double MIN_LAT = -85.05112878;
const double MAX_LON = 180.0;
const double MIN_LON = -180.0;

struct GeodeticCoordinate
{
    double value = 0.0;

    double rad() const { return value * M_PI / 180.0; }

    double deg() const { return value; }

    operator double() const { return deg(); }
};

struct Latitude : public GeodeticCoordinate
{
    Latitude() = default;
    Latitude(double val) { value = std::clamp(val, MIN_LAT, MAX_LAT); }
};

struct Longitude : public GeodeticCoordinate
{
    Longitude() = default;
    Longitude(double val) { value = std::clamp(val, MIN_LON, MAX_LON); }
};

struct Coordinates
{
    Longitude lon;
    Latitude lat;
};