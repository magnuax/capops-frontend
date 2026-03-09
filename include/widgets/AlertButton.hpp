#pragma once
#include <QWidget>

class QLabel;
class QPushButton;
class QMessageBox;

class AlertButton : public QWidget
{
    Q_OBJECT

public:
    AlertButton(int sectorId, const QString &label, QWidget *parent = nullptr);

private:
    QWidget *createAlert(const QString &label);

    QLabel *createLabel(const QString &label);

    QPushButton *createPushButton();

    void applyStyling();

    QPushButton *_button;
    QLabel *_label;
    int _sectorId;

private slots:
    void acknowledgeAlert();

signals:
    void alertAcknowledged(int sectorId);
};