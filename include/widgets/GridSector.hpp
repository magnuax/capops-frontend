#pragma once

#include <QFrame>

#include "domain/SectorStates.hpp"
#include "domain/DisplayMode.hpp"

class QMenu;

class GridSector : public QFrame
{
    Q_OBJECT

signals:
    void selected(GridSector *cell);

public:
    explicit GridSector(int row, int col, QWidget *parent = nullptr);
    explicit GridSector(int row, int col, int id, QWidget *parent = nullptr);

    int getRow() const;
    int getCol() const;

    void setRiskState(RiskState state);
    void setWeatherState(WeatherState state);
    void setTrafficState(TrafficState state);
    void setDisplayMode(DisplayMode mode);
    void setSelected(bool selected);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void createContextMenu();

    QColor getRiskStateColor();
    QColor getWeatherStateColor();
    QColor getTrafficStateColor();

    int _row;
    int _col;
    int _id;
    bool _isSelected;

    RiskState _riskState;
    WeatherState _weatherState;
    TrafficState _trafficState;
    DisplayMode _displayMode;

    QMenu *_contextMenu;
    QAction *_renameAction;
    QAction *_deleteAction;
    QAction *_customAction;
};