#include <QWidget>
#include <QPointer>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>

#include <algorithm>

#include "panels/AlertPanel.hpp"
#include "widgets/AlertButton.hpp"
#include "services/interfaces/IFlightDataService.hpp"

AlertPanel::AlertPanel(IFlightDataService *dataService, QWidget *parent) : QFrame(parent), _dataService(dataService)
{
    setMinimumWidth(400);

    connect(_dataService, &IFlightDataService::acknowledgeSucceeded,
            this, &AlertPanel::onAcknowledgeSucceeded);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(buildAlertPanel());

    setLayout(layout);
}

void AlertPanel::setRiskEventData(const RiskEventData &data)
{
    QSet<int> incomingSectorIds;

    for (const MergedRiskEvent &mergedEvent : data.getMergedRiskEvents())
    {
        if (mergedEvent.getLastRiskState() == RiskState::NORMAL)
            continue;

        incomingSectorIds.insert(mergedEvent.getSectorId());

        AlertButton *existing = findButtonForSector(mergedEvent.getSectorId());
        if (existing)
            existing->setMergedRiskEvent(mergedEvent); // update in place
        else
            addMergedAlert(mergedEvent); // new alert
    }

    // Remove buttons whose sector is no longer in the incoming data
    for (AlertButton *button : _activeAlerts)
    {
        if (button && !incomingSectorIds.contains(button->getMergedRiskEvent().getSectorId()))
        {
            _activeAlerts.removeOne(button);
            button->deleteLater();
        }
    }
}

AlertButton *AlertPanel::findButtonForSector(int sectorId)
{
    for (AlertButton *button : _activeAlerts)
        if (button && button->getMergedRiskEvent().getSectorId() == sectorId)
            return button;
            
    return nullptr;
}

void AlertPanel::clearAlerts()
{
    for (AlertButton *button : _activeAlerts)
        if (button)
            button->deleteLater();

    _activeAlerts.clear();
}

void AlertPanel::wireAcknowledgeButton(AlertButton *button)
{
    connect(button, &AlertButton::alertAcknowledged,
            _dataService, &IFlightDataService::acknowledgeRiskEvents);

}

void AlertPanel::onAcknowledgeSucceeded(const MergedRiskEvent &mergedEvent)
{
    AlertButton *match = nullptr;

    for (AlertButton *button : _activeAlerts)
    {
        if (!button)
            continue;

        const MergedRiskEvent &buttonEvent = button->getMergedRiskEvent();
        if (buttonEvent.getSectorId() == mergedEvent.getSectorId() &&
            buttonEvent.getRiskEvents().size() == mergedEvent.getRiskEvents().size())
        {
            match = button;
            break;
        }
    }

    if (match)
        handleAlertAcknowledged(match);
}

void AlertPanel::handleAlertAcknowledged(AlertButton *button)
{
    if (!button)
        return;

    _activeAlerts.removeOne(button);

    button->hide();
    button->setParent(nullptr);
    button->deleteLater();
}

void AlertPanel::addMergedAlert(const MergedRiskEvent &mergedEvent)
{
    if (mergedEvent.getRiskEvents().empty())
        return;

    Q_ASSERT(_alertsContainer && _alertsContainer->layout());

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