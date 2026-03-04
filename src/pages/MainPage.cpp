#include <QVBoxLayout>

#include "pages/MainPage.hpp"
#include "panels/StateGridPanel.hpp"
#include "panels/OperatorPanel.hpp"

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(createStateGrid());
    mainLayout->addWidget(createOperatorPanel());
    mainLayout->setStretch(0, 3); 
    mainLayout->setStretch(1, 1);
    setLayout(mainLayout);
}

QWidget *MainPage::createStateGrid()
{
    int PLACEHOLDER_rows = 10;
    int PLACEHOLDER_cols = 10;

    QWidget *stateGrid = new StateGridPanel(
        PLACEHOLDER_rows,
        PLACEHOLDER_cols,
        this);

    stateGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return stateGrid;
};

QWidget *MainPage::createOperatorPanel()
{
    QWidget *operatorPanel = new OperatorPanel(this);
    return operatorPanel;
}