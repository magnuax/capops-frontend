#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>

#include "widgets/AlertButton.hpp"

AlertButton::AlertButton(const RiskEvent &event, QWidget *parent)
    : AlertButton(MergedRiskEvent(event.getSectorId(), {event}), parent)
{
}

AlertButton::AlertButton(const MergedRiskEvent &mergedEvent, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _labelWidget = createLabel();
    _ackButton = createAckButton();

    _expandButton = new QPushButton("﹀", this);
    _expandButton->setCheckable(true);
    _expandButton->setCursor(Qt::PointingHandCursor);
    _expandButton->setObjectName("expandButton");
    _expandButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _expandButton->setFlat(true);
    _expandButton->setVisible(false);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(_labelWidget, 0, Qt::AlignLeft);
    headerLayout->addStretch();
    headerLayout->addWidget(_ackButton, 0, Qt::AlignRight);
    headerLayout->setContentsMargins(8, 8, 8, 4);

    QHBoxLayout *expandLayout = new QHBoxLayout();
    expandLayout->setContentsMargins(0, 0, 0, 0);
    expandLayout->setSpacing(0);
    expandLayout->addWidget(_expandButton);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);
    outerLayout->addLayout(headerLayout);
    outerLayout->addLayout(expandLayout);

    setLayout(outerLayout);

    connect(_expandButton, &QPushButton::toggled, this, &AlertButton::toggleHistory);
    connect(_ackButton, &QPushButton::clicked, this, &AlertButton::acknowledgeAlert);

    setMergedRiskEvent(mergedEvent);
}

QWidget *AlertButton::createLabel()
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    _mainLabel = new QLabel("", container);
    _mainLabel->setObjectName("alertLabel");

    _timestampLabel = new QLabel("", container);
    _timestampLabel->setObjectName("alertTimestamp");

    layout->addWidget(_mainLabel);
    layout->addWidget(_timestampLabel);

    return container;
}

QPushButton *AlertButton::createAckButton()
{
    _ackButton = new QPushButton("Acknowledge", this);
    _ackButton->setCursor(Qt::PointingHandCursor);
    _ackButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    _ackButton->setObjectName("acknowledgeButton");

    return _ackButton;
}

std::vector<RiskEvent> AlertButton::getSortedEvents(const std::vector<RiskEvent> &events)
{
    std::vector<RiskEvent> sorted = events;
    std::sort(sorted.begin(), sorted.end(),
              [](const RiskEvent &a, const RiskEvent &b)
              {
                  return a.getCreatedTimestamp() > b.getCreatedTimestamp();
              });

    return sorted;
}

void AlertButton::buildHistoryWidget()
{
    std::vector<RiskEvent> history = getSortedEvents(_mergedRiskEvent.getRiskEvents());
    history.erase(history.begin());

    if (history.empty())
    {
        _expandButton->setVisible(false);
        return;
    }

    _expandButton->setVisible(true);

    delete _historyWidget;
    _historyWidget = new QWidget(this);
    _historyWidget->setObjectName("historyWidget");

    QVBoxLayout *layout = new QVBoxLayout(_historyWidget);
    layout->setContentsMargins(8, 4, 8, 4);

    for (const RiskEvent &event : history)
    {
        QLabel *entry = new QLabel(
            event.getCreatedTimestamp().toString("hh:mm:ss") + "  " + event.getMessage(),
            _historyWidget);
        layout->addWidget(entry);
    }

    _historyWidget->setVisible(false);
    static_cast<QVBoxLayout *>(this->layout())->addWidget(_historyWidget);
}

void AlertButton::setRiskState(const RiskState &riskState)
{
    _riskState = riskState;

    auto repolish = [](QWidget *widget, const QString &state)
    {
        widget->setProperty("riskState", state);
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    };

    repolish(this, toString(riskState));
    repolish(_ackButton, toString(riskState));
}

void AlertButton::toggleHistory()
{
    if (_historyWidget)
    {
        _historyWidget->setVisible(!_historyWidget->isVisible());
        _expandButton->setText(_historyWidget->isVisible() ? "︿" : "﹀");
    }
}

void AlertButton::setRiskEvent(const RiskEvent &event)
{
    setMergedRiskEvent(MergedRiskEvent(event.getSectorId(), {event}));
}
void AlertButton::setMergedRiskEvent(const MergedRiskEvent &mergedEvent)
{
    _mergedRiskEvent = mergedEvent;
    const RiskEvent &latest = mergedEvent.getLastEvent();

    _mainLabel->setText(latest.getMessage());
    _timestampLabel->setText(latest.getCreatedTimestamp().toString("yyyy-MM-dd hh:mm:ss"));

    setRiskState(mergedEvent.getLastRiskState());
    buildHistoryWidget();
}

void AlertButton::acknowledgeAlert()
{
    emit alertAcknowledged(_mergedRiskEvent);
}