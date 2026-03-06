#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "panels/OperatorPanel.hpp"

OperatorPanel::OperatorPanel(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(createOperatorPanel());
    setLayout(layout);
}

QTabWidget *OperatorPanel::createOperatorPanel()
{
    QTabWidget *tabWidget = new QTabWidget(this);
    _logTab = createLogTab();
    _alertsTab = createAlertsTab();

    tabWidget->addTab(_logTab, "Logs");
    tabWidget->addTab(_alertsTab, "Alerts");

    return tabWidget;
}

QWidget *OperatorPanel::createLogTab()
{
    QWidget *logTab = new QWidget(this);

    return logTab;
};

QWidget *OperatorPanel::createAlertsTab()
{
    QWidget *alertsTab = new QWidget(this);

    return alertsTab;
}