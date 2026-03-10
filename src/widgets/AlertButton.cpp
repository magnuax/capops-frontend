#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

#include "widgets/AlertButton.hpp"

AlertButton::AlertButton(int sectorId, const QString &label, QWidget *parent)
    : QWidget(parent), _sectorId(sectorId)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(60);

    _label = createLabel(label);
    _button = createPushButton();

    QHBoxLayout *wrapper = new QHBoxLayout(this);

    wrapper->addWidget(_label, 0, Qt::AlignLeft);
    wrapper->addStretch();
    wrapper->addWidget(_button, 0, Qt::AlignRight);

    wrapper->setContentsMargins(8, 8, 8, 8);
    setLayout(wrapper);

    connect(_button, &QPushButton::clicked, this, &AlertButton::acknowledgeAlert);
}

QWidget *AlertButton::createAlert(const QString &label)
{
    return createLabel(label);
}

QLabel *AlertButton::createLabel(const QString &label)
{
    _label = new QLabel(label, this);
    _label->setObjectName("alertLabel");

    return _label;
}

QPushButton *AlertButton::createPushButton()
{
    _button = new QPushButton("Acknowledge", this);
    _button->setCursor(Qt::PointingHandCursor);
    _button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    _button->setObjectName("acknowledgeButton");

    return _button;
}

void AlertButton::acknowledgeAlert()
{
    emit alertAcknowledged(_sectorId);
}