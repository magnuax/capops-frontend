#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "panels/AlertPanel.hpp"
#include "widgets/AlertButton.hpp"
#include "services/interfaces/IFlightDataService.hpp"

AlertPanel::AlertPanel(IFlightDataService *_dataService, QWidget *parent) : QFrame(parent), _dataService(_dataService)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(buildAlertPanel());

    // PLACEHOLDER:
    RiskEvent testEvent3(1, 1, false, RiskState::CONGESTED, QDateTime::currentDateTime(), QDateTime(), "Escalated to CONGESTED");
    RiskEvent testEvent2(1, 1, false, RiskState::CONGESTED, QDateTime::currentDateTime().addSecs(-60), QDateTime(), "De-escalated to AT_RISK");
    RiskEvent testEvent1(1, 1, false, RiskState::CONGESTED, QDateTime::currentDateTime().addSecs(-120), QDateTime(), "Escalated to CONGESTED");

    MergedRiskEvent mergedEvent(1, {testEvent1, testEvent2, testEvent3});
    addMergedAlert(mergedEvent);

    RiskEvent testEvent4(2, 2, false, RiskState::AT_RISK, QDateTime::currentDateTime(), QDateTime(), "Escalated to AT_RISK");
    addMergedAlert(MergedRiskEvent(2, {testEvent4}));

    setLayout(layout);
}

void AlertPanel::setRiskEventData(const RiskEventData &data)
{
    clearAlerts();

    for (const MergedRiskEvent &mergedEvent : data.getMergedRiskEvents())
        addMergedAlert(mergedEvent);
}

void AlertPanel::clearAlerts()
{
    for (AlertButton *button : _activeAlerts)
        button->deleteLater();

    _activeAlerts.clear();
}

void AlertPanel::wireAcknowledgeButton(AlertButton *button)
{
    connect(button, &AlertButton::alertAcknowledged,
            _dataService, &IFlightDataService::acknowledgeRiskEvents);

    connect(_dataService, &IFlightDataService::acknowledgeSucceeded,
            this, [this, button]()
            {
            _activeAlerts.erase(
                std::remove(_activeAlerts.begin(), _activeAlerts.end(), button),
                _activeAlerts.end());
            button->deleteLater(); });
}

void AlertPanel::addMergedAlert(const MergedRiskEvent &mergedEvent)
{
    if (mergedEvent.getRiskEvents().empty())
        return;

    AlertButton *button = new AlertButton(mergedEvent, this);

    wireAcknowledgeButton(button);
    _alertsTab->layout()->addWidget(button);
    _activeAlerts.push_back(button);
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