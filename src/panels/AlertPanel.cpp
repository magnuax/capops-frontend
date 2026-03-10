#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "panels/AlertPanel.hpp"
#include "widgets/AlertButton.hpp"

AlertPanel::AlertPanel(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(buildAlertPanel());

    // PLACEHOLDER:
    QWidget *button = createAlertButton(1);
    _alertsTab->layout()->addWidget(button);

    setLayout(layout);
}

QTabWidget *AlertPanel::buildAlertPanel()
{
    QTabWidget *tabWidget = new QTabWidget(this);
    _logTab = buildLogTab();
    _alertsTab = buildAlertsTab();

    tabWidget->addTab(_alertsTab, "Alerts");
    tabWidget->addTab(_logTab, "Logs");

    return tabWidget;
}

QWidget *AlertPanel::buildLogTab()
{
    QWidget *logTab = new QWidget(this);

    return logTab;
};

QWidget *AlertPanel::buildAlertsTab()
{
    QWidget *alertsTab = new QWidget(this);
    auto *layout = new QVBoxLayout(alertsTab);
    layout->setAlignment(Qt::AlignTop);
    alertsTab->setLayout(layout);

    return alertsTab;
}

QWidget *AlertPanel::createAlertButton(int sectorId)
{
    // PLACEHOLDER:
    AlertButton *button = new AlertButton(sectorId, "PLACEHOLDER", this);
    _activeAlerts.push_back(button);

    return button;
}