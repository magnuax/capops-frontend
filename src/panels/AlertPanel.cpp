#include <QWidget>
#include <QPointer>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>

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
    _alertsContainer->layout()->addWidget(button);
    _activeAlerts.push_back(button);
}

QTabWidget *AlertPanel::buildAlertPanel()
{
    QWidget *tab = buildAlertsTab();

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(tab, "Alerts");

    return tabWidget;
}

QWidget *AlertPanel::buildAlertsTab()
{
    QWidget *inner = new QWidget();
    auto *innerLayout = new QVBoxLayout(inner);
    innerLayout->setAlignment(Qt::AlignTop);
    inner->setLayout(innerLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(inner);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *tab = new QWidget(this);
    auto *tabLayout = new QVBoxLayout(tab);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(scrollArea);
    tab->setLayout(tabLayout);

    _alertsContainer = inner;

    return tab;
}