#pragma once

#include <QMainWindow>

class QWidget;
class QStackedWidget;
class IMapFetcher;

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *_pageStack;
    QWidget *_mainPage;
    IMapFetcher *_mapFetcher;

    void initializePages();

public slots:
    void showMainPage();
};