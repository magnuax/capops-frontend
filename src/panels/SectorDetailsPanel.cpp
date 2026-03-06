#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

#include "panels/SectorDetailsPanel.hpp"

SectorDetailsPanel::SectorDetailsPanel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(createTabs(), 0, Qt::AlignTop);
}

void SectorDetailsPanel::setSector(int row, int col)
{
    // Replace with (row,col) -> ID logic when integrating backend
    _selectedSector = {row, col};

    if (_rowLabel)
    {
        _rowLabel->setText("Row: " + QString::number(row));
    }

    if (_colLabel)
    {
        _colLabel->setText("Col: " + QString::number(col));
    }
}

QTabWidget *SectorDetailsPanel::createTabs()
{
    QTabWidget *tabs = new QTabWidget(this);

    _rowLabel = new QLabel(this);
    _colLabel = new QLabel(this);

    _rowLabel->setText("Row: " + QString::number(_selectedSector.x()));
    _colLabel->setText("Col: " + QString::number(_selectedSector.y()));

    _rowLabel->setStyleSheet("font-size: 32px; font-weight: bold;");
    _colLabel->setStyleSheet("font-size: 32px; font-weight: bold;");

    QWidget *placeholderTab = new QWidget();
    placeholderTab->setLayout(new QVBoxLayout());
    placeholderTab->layout()->addWidget(_rowLabel);
    placeholderTab->layout()->addWidget(_colLabel);

    tabs->addTab(placeholderTab, "Details");

    return tabs;
}