#include <QWidget>
#include <QPointer>
#include <QTabWidget>
#include <QVBoxLayout>
#include <algorithm>

#include "panels/AlertPanel.hpp"
#include "widgets/AlertButton.hpp"
#include "services/interfaces/IFlightDataService.hpp"

AlertPanel::AlertPanel(IFlightDataService *_dataService, QWidget *parent) : QFrame(parent), _dataService(_dataService)
{
    setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(buildAlertPanel());

    setLayout(layout);
}

void AlertPanel::setRiskEventData(const RiskEventData &data)
{
    clearAlerts();

    for (const MergedRiskEvent &mergedEvent : data.getMergedRiskEvents())
    {
        if (mergedEvent.getLastRiskState() == RiskState::NORMAL)
            continue;

        addMergedAlert(mergedEvent);
    }
}

void AlertPanel::clearAlerts()
{
    for (AlertButton *button : _activeAlerts)
        button->deleteLater();

    _activeAlerts.clear();
}

void AlertPanel::clearPlaceholderAlerts()
{
    for (AlertButton *button : _placeholderAlerts)
    {
        button->deleteLater();
    }
    _placeholderAlerts.clear();
}

void AlertPanel::wireAcknowledgeButton(AlertButton *button)
{
    connect(button, &AlertButton::alertAcknowledged,
            _dataService, &IFlightDataService::acknowledgeRiskEvents);

    QPointer<AlertButton> safeButton(button);

    connect(_dataService, &IFlightDataService::acknowledgeSucceeded,
            button, [this, button]()
            { handleAlertAcknowledged(button); });
}

void AlertPanel::handleAlertAcknowledged(AlertButton *button)
{
    if (!button)
        return;

    _activeAlerts.erase(
        std::remove(_activeAlerts.begin(), _activeAlerts.end(), button),
        _activeAlerts.end());

    button->deleteLater();
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