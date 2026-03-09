#pragma once
#include <QFrame>

class QWidget;
class QTabWidget;

class AlertPanel : public QFrame
{
    Q_OBJECT

public:
    explicit AlertPanel(QWidget *parent = nullptr);

private:
    QTabWidget *createAlertPanel();
    QWidget *createLogTab();
    QWidget *createAlertsTab();

    QWidget *_logTab;
    QWidget *_alertsTab;

signals:
    void alertAcknowledged(const QString &sectorId);
};