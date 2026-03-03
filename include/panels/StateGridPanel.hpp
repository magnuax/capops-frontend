#pragma once

#include <QFrame>

class QWidget;

class StateGridPanel : public QFrame
{
    Q_OBJECT

public:
    explicit StateGridPanel(QWidget *parent = nullptr);

private:
    QWidget* createGrid();
    QWidget* _grid;
};