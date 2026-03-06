#pragma once

#include <QFrame>

#include "domain/SectorStates.hpp"
#include "domain/DisplayMode.hpp"

class QMenu;

class GridSector : public QFrame
{
    Q_OBJECT

public:
    explicit GridSector(int row, int col, QWidget *parent = nullptr);

    void setRiskState(RiskState state);
    void setWeatherState(WeatherState state);
    void setTrafficState(TrafficState state);

    void setDisplayMode(DisplayMode mode);

    void setSelected(bool selected);

signals:
    void selected(int row, int col);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void createContextMenu();

    QColor getRiskStateColor();
    QColor getWeatherStateColor();
    QColor getTrafficStateColor();

private:
    int _id;
    int _row;
    int _col;

    RiskState _riskState;
    WeatherState _weatherState;
    TrafficState _trafficState;

    DisplayMode _displayMode;

    bool _isSelected;

    QMenu *_contextMenu;
    QAction *_renameAction;
    QAction *_deleteAction;
    QAction *_customAction;
};