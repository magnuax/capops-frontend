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
    createAckButton();

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

    connect(_expandButton, &QPushButton::toggled, this, [](bool checked)
            { qDebug() << "expand toggled ->" << checked; });
}

QWidget *AlertButton::createLabel()
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    _mainLabel = new QLabel("", container);
    _mainLabel->setObjectName("alertLabel");
    _mainLabel->setWordWrap(true);

    _timestampLabel = new QLabel("", container);
    _timestampLabel->setObjectName("alertTimestamp");

    layout->addWidget(_mainLabel);
    layout->addWidget(_timestampLabel);

    return container;
}

void *AlertButton::createAckButton()
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
    if (!history.empty())
        history.erase(history.begin());

    if (history.empty())
    {
        _expandButton->setVisible(false);
        if (_historyWidget)
            _historyWidget->setVisible(false);
        return;
    }

    _expandButton->setVisible(true);

    if (!_historyWidget)
    {
        _historyWidget = new QWidget(this);
        _historyWidget->setObjectName("historyWidget");
        _historyWidget->setVisible(false);
        static_cast<QVBoxLayout *>(this->layout())->addWidget(_historyWidget);
    }

    if (!history.empty())
    {
        const QDateTime cutoff = _mergedRiskEvent.getLastEvent().getCreatedTimestamp().addSecs(-3600);
        history.erase(
            std::remove_if(history.begin(), history.end(),
                           [&cutoff](const RiskEvent &event)
                           {
                               return event.getCreatedTimestamp() < cutoff;
                           }),
            history.end());
    }

    const bool wasExpanded = _historyWidget->isVisible();

    // Block signals while rebuilding to prevent layout changes
    // from spuriously unchecking _expandButton
    QSignalBlocker blocker(_expandButton);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(_historyWidget->layout());
    if (!layout)
    {
        layout = new QVBoxLayout(_historyWidget);
        layout->setContentsMargins(8, 4, 8, 4);
    }
    else
    {
        while (QLayoutItem *item = layout->takeAt(0))
        {
            delete item->widget();
            delete item;
        }
    }

    for (const RiskEvent &event : history)
    {
        QLabel *entry = new QLabel(
            event.getCreatedTimestamp().toString("hh:mm:ss") + "  " + event.getMessage(),
            _historyWidget);
        layout->addWidget(entry);
    }

    // Restore state explicitly after rebuild
    _historyWidget->setVisible(wasExpanded);
    _expandButton->setChecked(wasExpanded);
    _expandButton->setText(wasExpanded ? "︿" : "﹀");
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

void AlertButton::toggleHistory(bool expanded)
{
    if (_historyWidget)
    {
        _historyWidget->setVisible(expanded);
        _expandButton->setText(expanded ? "︿" : "﹀");
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

    if (mergedEvent.getRiskEvents().size() != _lastKnownEventCount)
    {
        _lastKnownEventCount = mergedEvent.getRiskEvents().size();
        buildHistoryWidget();
    }
}

MergedRiskEvent AlertButton::getMergedRiskEvent() const
{
    return _mergedRiskEvent;
}

bool AlertButton::isHistoryExpanded() const
{
    return _historyWidget && _historyWidget->isVisible();
}

void AlertButton::setHistoryExpanded(bool expanded)
{
    if (!_expandButton || !_expandButton->isVisible())
        return;

    _expandButton->setChecked(expanded);
}

void AlertButton::acknowledgeAlert()
{
    qDebug() << "[AlertButton]" << (void *)this << "Acknowledge clicked for sector" << _mergedRiskEvent.getSectorId();
    emit alertAcknowledged(_mergedRiskEvent);
}