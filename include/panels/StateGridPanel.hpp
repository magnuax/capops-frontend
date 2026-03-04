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

protected:
    void resizeEvent(QResizeEvent *e) override;

private:
    QWidget *createGrid();
    QWidget *_gridWidget;
    QGridLayout *_gridLayout;

    int _numRows;
    int _numCols;

    std::vector<GridSector *> _cells;
};