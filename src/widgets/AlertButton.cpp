#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMouseEvent>
#include <QStyle>

#include "widgets/AlertButton.hpp"

AlertButton::AlertButton(int sectorId, const QString &label, QWidget *parent)
    : AlertButton(sectorId, label, QDateTime::currentDateTime(), parent)
{
}

AlertButton::AlertButton(int sectorId, const QString &label, const QDateTime &timestamp, QWidget *parent)
    : QWidget(parent), _sectorId(sectorId), _label(label), _timestamp(timestamp)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(60);

    _labelWidget = createLabel();
    _buttonWidget = createPushButton();

    QHBoxLayout *wrapper = new QHBoxLayout(this);

    wrapper->addWidget(_labelWidget, 0, Qt::AlignLeft);
    wrapper->addStretch();
    wrapper->addWidget(_buttonWidget, 0, Qt::AlignRight);

    wrapper->setContentsMargins(8, 8, 8, 8);
    setLayout(wrapper);

    connect(_buttonWidget, &QPushButton::clicked, this, &AlertButton::acknowledgeAlert);
}

QWidget *AlertButton::createLabel()
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLabel *mainLabel = new QLabel(_label, container);
    mainLabel->setObjectName("alertLabel");

    QLabel *timestampLabel = new QLabel(_timestamp.toString("yyyy-MM-dd hh:mm:ss"), container);
    timestampLabel->setObjectName("alertTimestamp");

    layout->addWidget(mainLabel);
    layout->addWidget(timestampLabel);

    return container;
}

QPushButton *AlertButton::createPushButton()
{
    _buttonWidget = new QPushButton("Acknowledge", this);
    _buttonWidget->setCursor(Qt::PointingHandCursor);
    _buttonWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    _buttonWidget->setObjectName("acknowledgeButton");

    return _buttonWidget;
}

void AlertButton::setRiskState(const RiskState &riskState)
{
    _riskState = riskState;
    
    setProperty("riskState", toString(riskState));
    style()->unpolish(this);
    style()->polish(this);
    
    _buttonWidget->setProperty("riskState", toString(riskState));
    _buttonWidget->style()->unpolish(_buttonWidget);
    _buttonWidget->style()->polish(_buttonWidget);
}

void AlertButton::acknowledgeAlert()
{
    emit alertAcknowledged(_sectorId);
}