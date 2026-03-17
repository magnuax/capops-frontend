#include <QDateTime>
#include <QString>

#include "domain/data/Track.hpp"

Track::Track(
    const QString& icao24,
    const QDateTime& timestamp,
    const TrackPosition& position,
    const TrackVelocity& velocity,
    const double headingDegrees,
    const double groundTrackDegrees
)
    : 
    _icao24(icao24),
    _timestamp(timestamp),
    _position(position),
    _velocity(velocity),
    _headingDegrees(headingDegrees),
    _groundTrackDegrees(groundTrackDegrees)
{}

const QString& Track::getIcao24() const
{
    return _icao24;
}

const QDateTime& Track::getTimestamp() const
{
    return _timestamp;
}

const Track::TrackPosition &Track::getPosition() const
{
    return _position;
}

const Track::TrackVelocity &Track::getVelocity() const
{
    return _velocity;
}

const double Track::getHeadingDegrees() const
{
    return _headingDegrees;
}

const double Track::getGroundTrackDegrees() const
{
    return _groundTrackDegrees;
}