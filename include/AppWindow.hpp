#pragma once

#include <QMainWindow>

class QWidget;

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);
};