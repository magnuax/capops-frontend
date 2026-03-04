#pragma once

#include <QFrame>

class QWidget;
class GridSector;
class QGridLayout;

class StateGridPanel : public QFrame
{
    Q_OBJECT

public:
    explicit StateGridPanel(int numRows, int numCols, QWidget *parent = nullptr);

    void setMapSource(const QPixmap *mapSource);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QWidget *createGrid();
    QWidget *_gridWidget;
    QGridLayout *_gridLayout;
    QPixmap _mapSource;

    int _numRows;
    int _numCols;

    std::vector<GridSector *> _cells;
};