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
    AlertButton *button_high = createAlertButton(1, "PLACEHOLDER MSG", QDateTime::currentDateTime());
    button_high->setRiskState(RiskState::CONGESTED);
    _alertsTab->layout()->addWidget(button_high);

    AlertButton *button_medium = createAlertButton(2, "PLACEHOLDER MSG", QDateTime::currentDateTime());
    button_medium->setRiskState(RiskState::AT_RISK);
    _alertsTab->layout()->addWidget(button_medium);

    setLayout(layout);
}

QWidget *AlertPanel::addAlert(const RiskEvent &riskEvent)
{

    QWidget *button = createAlertButton(
        riskEvent.getSectorId(),
        riskEvent.getMessage(),
        riskEvent.getCreatedTimestamp());

    _alertsTab->layout()->addWidget(button);

    return button;
}

QTabWidget *AlertPanel::buildAlertPanel()
{
    _alertsTab = buildAlertsTab();

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(_alertsTab, "Alerts");

    return tabWidget;
}

QWidget *AlertPanel::buildAlertsTab()
{
    QWidget *alertsTab = new QWidget(this);
    auto *layout = new QVBoxLayout(alertsTab);
    layout->setAlignment(Qt::AlignTop);
    alertsTab->setLayout(layout);

    return alertsTab;
}

AlertButton *AlertPanel::createAlertButton(int sectorId, const QString &label, const QDateTime &timestamp)
{
    AlertButton *button = new AlertButton(sectorId, label, timestamp, this);
    _activeAlerts.push_back(button);

    return button;
}