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

const double Track::getHeading() const
{
    return _headingDegrees;
}

const double Track::getGroundTrack() const
{
    return _groundTrackDegrees;
}

bool Track::operator==(const Track &other) const
{
    return _icao24 == other._icao24 &&
           _timestamp == other._timestamp &&
           _position == other._position &&
           _velocity == other._velocity &&
           _headingDegrees == other._headingDegrees &&
           _groundTrackDegrees == other._groundTrackDegrees;
}