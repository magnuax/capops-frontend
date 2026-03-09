#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "panels/AlertPanel.hpp"

AlertPanel::AlertPanel(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(createAlertPanel());
    setLayout(layout);
}

QTabWidget *AlertPanel::createAlertPanel()
{
    QTabWidget *tabWidget = new QTabWidget(this);
    _logTab = createLogTab();
    _alertsTab = createAlertsTab();

    tabWidget->addTab(_logTab, "Logs");
    tabWidget->addTab(_alertsTab, "Alerts");

    return tabWidget;
}

QWidget *AlertPanel::createLogTab()
{
    QWidget *logTab = new QWidget(this);

    return logTab;
};

QWidget *AlertPanel::createAlertsTab()
{
    QWidget *alertsTab = new QWidget(this);

    return alertsTab;
}