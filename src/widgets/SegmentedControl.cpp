#include <QHBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QStyle>

#include "widgets/SegmentedControl.hpp"

SegmentedControl::SegmentedControl(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _layout = new QHBoxLayout(this);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);

    _group = new QButtonGroup(this);
    _group->setExclusive(true);

    connect(_group, &QButtonGroup::idClicked, this, [this](int id)
            {
        if (_selectedIndex == id) return;
        _selectedIndex = id;
        emit segmentSelected(id); });
}

SegmentedControl::SegmentedControl(const QStringList &segments, QWidget *parent) : SegmentedControl(parent)
{
    setSegments(segments);
}

void SegmentedControl::setSegmentData(int index, const QVariant &data)
{
    if (index < 0 || index >= _buttons.size())
        return;

    _buttons[index]->setProperty("segmentData", data);
}

QVariant SegmentedControl::getSegmentData(int index) const
{
    if (index < 0 || index >= _buttons.size())
        return QVariant();

    return _buttons[index]->property("segmentData");
}

QToolButton *SegmentedControl::createButton(const QString &text, int index)
{
    auto *button = new QToolButton(this);
    button->setText(text);
    button->setCheckable(true);
    button->setAutoExclusive(true);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setCursor(Qt::PointingHandCursor);

    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    _group->addButton(button, index);
    _layout->addWidget(button, 1);

    return button;
}

void SegmentedControl::addSegment(const QString &text)
{
    const int index = _buttons.size();
    auto *button = createButton(text, index);
    _buttons.push_back(button);

    if (_selectedIndex < 0 && !_buttons.isEmpty())
    {
        setSelectedIndex(0);
    }
}

void SegmentedControl::setSegments(const QStringList &segments)
{
    for (auto *button : _buttons)
    {
        _group->removeButton(button);
        _layout->removeWidget(button);
        button->deleteLater();
    }
    _buttons.clear();
    _selectedIndex = -1;

    for (const auto &segment : segments)
    {
        addSegment(segment);
    }
}

void SegmentedControl::setSelectedIndex(int index)
{
    if (index < 0 || index >= _buttons.size())
        return;

    _selectedIndex = index;
    _buttons[index]->setChecked(true);
    emit segmentSelected(index);
}
