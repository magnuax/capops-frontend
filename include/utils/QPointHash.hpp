#pragma once
#include <QPoint>
#include <QSize>

struct QPointHash
{
    std::size_t operator()(const QPoint &p) const
    {
        std::size_t h1 = std::hash<int>{}(p.x());
        std::size_t h2 = std::hash<int>{}(p.y());
        
        return h1 ^ (h2 << 32 | h2 >> 32);
    }
};