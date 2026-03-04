#include <QVBoxLayout>

#include "pages/MainPage.hpp"
#include "panels/StateGridPanel.hpp"
#include "panels/OperatorPanel.hpp"
#include "panels/SectorDetailsPanel.hpp"

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(createSectorDetailsPanel());
    mainLayout->addWidget(createStateGrid());
    mainLayout->addWidget(createOperatorPanel());

    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);
    mainLayout->setStretch(2, 1);

    setLayout(mainLayout);
}

QWidget *MainPage::createStateGrid()
{
    int PLACEHOLDER_rows = 30;
    int PLACEHOLDER_cols = 30;
    QPixmap PLACEHOLDER_map("resources/placeholder.png");

    StateGridPanel *stateGrid = new StateGridPanel(
        PLACEHOLDER_rows,
        PLACEHOLDER_cols,
        this);

    stateGrid->setMapSource(&PLACEHOLDER_map);
    stateGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return stateGrid;
};

QWidget *MainPage::createOperatorPanel()
{
    QWidget *operatorPanel = new OperatorPanel(this);
    return operatorPanel;
}

QWidget *MainPage::createSectorDetailsPanel()
{
    QWidget *sectorDetailsPanel = new SectorDetailsPanel(this);
    return sectorDetailsPanel;
}