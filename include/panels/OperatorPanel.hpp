#pragma once
#include <QFrame>

class QWidget;
class QTabWidget;

class OperatorPanel : public QFrame
{
    Q_OBJECT

public:
    explicit OperatorPanel(QWidget *parent = nullptr);

private:
    QTabWidget *createOperatorPanel();
    QWidget *createLogTab();
    QWidget *createAlertsTab();

    QWidget *_logTab;
    QWidget *_alertsTab;

signals:
    void alertAcknowledged(const QString &sectorId);
};