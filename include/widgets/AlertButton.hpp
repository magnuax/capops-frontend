#pragma once
#include <QWidget>

#include <domain/SectorStates.hpp>
#include <domain/data/RiskEvent.hpp>
#include <domain/data/MergedRiskEvent.hpp>

class QString;
class QLabel;
class QPushButton;
class QMessageBox;
class QDateTime;
class QMouseEvent;
class QToolButton;

Q_PROPERTY(RiskState riskState READ riskState WRITE setRiskState)

class AlertButton : public QWidget
{
    Q_OBJECT

signals:
    void alertAcknowledged(const MergedRiskEvent &mergedEvent);

private slots:
    void acknowledgeAlert();
    void toggleHistory();

public:
    AlertButton(const RiskEvent &event, QWidget *parent = nullptr);
    AlertButton(const MergedRiskEvent &mergedEvent, QWidget *parent = nullptr);

    void setRiskEvent(const RiskEvent &event);
    void setMergedRiskEvent(const MergedRiskEvent &mergedEvent);

private:
    QWidget *createLabel();
    QPushButton *createAckButton();
    void buildHistoryWidget();

    void setRiskState(const RiskState &riskState);

    std::vector<RiskEvent> getSortedEvents(const std::vector<RiskEvent> &events);

    QPushButton *_ackButton = nullptr;
    QWidget *_labelWidget = nullptr;
    QWidget *_historyWidget = nullptr;
    QPushButton *_expandButton = nullptr;

    RiskState _riskState;
    QLabel *_mainLabel = nullptr;
    QLabel *_timestampLabel = nullptr;

    std::vector<RiskEvent> _sortedEvents;

    MergedRiskEvent _mergedRiskEvent;
};