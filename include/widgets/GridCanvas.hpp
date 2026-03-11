#pragma once
#include <QWidget>

class QPixmap;
class QPaintEvent;

class GridCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit GridCanvas(QWidget *parent = nullptr);

    void setMapSource(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap _mapSource;
};