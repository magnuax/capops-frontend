#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "panels/SectorDetailsPanel.hpp"

SectorDetailsPanel::SectorDetailsPanel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QTabWidget(this));
}