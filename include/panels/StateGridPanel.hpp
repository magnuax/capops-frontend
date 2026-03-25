#pragma once

#include <QFrame>
#include <QMargins>
#include <QPixmap>

#include "domain/DisplayMode.hpp"
#include "domain/SectorStates.hpp"
#include "domain/MapProjection.hpp"
#include "domain/data/SectorSummaryData.hpp"

class QWidget;
class GridCanvas;
class GridTrackOverlay;
class GridSector;
class QGridLayout;
class QHBoxLayout;
class QTabBar;
class QLabel;

class IFlightDataService;

class StateGridPanel : public QFrame
{
    Q_OBJECT

signals:
    void sectorSelected(int sectorId);
    void trackSelected(const QString &icao24);

public slots:
    void refresh();
    void updateCoordinateLabel(double lat, double lon);
    void selectTrack(const QString &icao24);

public:
    explicit StateGridPanel(IFlightDataService *dataService, QWidget *parent = nullptr);

    void setDisplayMode(DisplayMode mode);

    void setMapSource(const QPixmap &mapSource, const MapProjection &projection);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void buildPanel();
    QHBoxLayout *buildTabLayout();
    QTabBar *buildTabWidget();
    QWidget *buildGridContainer(const SectorSummaryData &data);
    GridCanvas *buildGridWidget(const SectorSummaryData &data);
    QLabel *buildCoordinateLabel();

    void wireConnections();
    void rebuildGrid(const SectorSummaryData &data);

    void handleSectorSelection(GridSector *cell);
    void handleTabChange(int tabIndex);

    int _numRows = 0;
    int _numCols = 0;

    QTabBar *_tabBar = nullptr;
    QGridLayout *_gridLayout = nullptr;

    QLabel *_coordLabel = nullptr;
    GridCanvas *_gridWidget = nullptr;
    GridTrackOverlay *_trackOverlay = nullptr;

    GridSector *_selectedCell = nullptr;
    std::vector<GridSector *> _cells;
    QPixmap _mapSource;

    IFlightDataService *_dataService = nullptr;

    static constexpr int TAB_IDX_RISK = 0;
    static constexpr int TAB_IDX_WEATHER = 1;
    static constexpr int TAB_IDX_TRAFFIC = 2;
    static constexpr int TAB_IDX_NONE = 3;

    static constexpr QMargins PANEL_OUTER_MARGINS = QMargins(0, 0, 0, 8);
    static constexpr QMargins GRID_CONTAINER_MARGINS = QMargins(12, 12, 12, 12);
};