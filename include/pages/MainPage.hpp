#pragma once

#include <QWidget>

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);

private:
    QWidget* createStateGrid();
    QWidget* operatorPanel();

};