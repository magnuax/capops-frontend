#pragma once

#include <QWidget>
#include <QStringList>

class QButtonGroup;
class QHBoxLayout;
class QToolButton;

class SegmentedControl : public QWidget
{
    Q_OBJECT

public:
    explicit SegmentedControl(QWidget *parent = nullptr);

    explicit SegmentedControl(const QStringList &segments, QWidget *parent = nullptr);

    void setSegmentData(int index, const QVariant &data);

    QVariant getSegmentData(int index) const;

    void addSegment(const QString &text);

    void setSegments(const QStringList &segments);

    void setSelectedIndex(int index);

    int getSelectedIndex() const;

signals:
    void segmentSelected(int index);

private:
    QToolButton *createButton(const QString &text, int index);

    void applyStyling();

    int _selectedIndex = -1;

    QHBoxLayout *_layout = nullptr;
    QButtonGroup *_group = nullptr;
    QList<QToolButton *> _buttons;
};
