#pragma once

#include <QMainWindow>

class QWidget;
class QStackedWidget;

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);

private:
    QStackedWidget* _pageStack;
    QWidget* _mainPage;

    void initializePages();

public slots:
    void showMainPage();
};