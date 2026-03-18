#pragma once
#include <QWidget>

#include <domain/SectorStates.hpp>

class QString;
class QLabel;
class QPushButton;
class QMessageBox;
class QDateTime;
class QMouseEvent;

Q_PROPERTY(RiskState riskState READ riskState WRITE setRiskState)

class AlertButton : public QWidget
{
    Q_OBJECT

signals:
    void alertAcknowledged(int sectorId);

private slots:
    void acknowledgeAlert();

public:
    AlertButton(int sectorId, const QString &label, const QDateTime &timestamp, QWidget *parent = nullptr);

    AlertButton(int sectorId, const QString &label, QWidget *parent = nullptr);

    void setRiskState(const RiskState &riskState);

private:
    QWidget *createLabel();

    QPushButton *createPushButton();

    QPushButton *_buttonWidget;
    QWidget *_labelWidget;

    int _sectorId;
    QString _label;
    QDateTime _timestamp;
    RiskState _riskState;
};