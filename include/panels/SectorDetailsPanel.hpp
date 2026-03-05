#pragma once

#include <QWidget>

class QPoint;
class QLabel;
class QTabWidget;

class SectorDetailsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SectorDetailsPanel(QWidget *parent = nullptr);

public slots:
    void setSector(int row, int col);

private:
    QTabWidget* createTabs();

    QLabel* _colLabel = nullptr;
    QLabel* _rowLabel = nullptr;
    QPoint _selectedSector = {0, 0};
};