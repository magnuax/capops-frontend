#pragma once

#include <QMainWindow>

class QWidget;
class QStackedWidget;
class IMapFetcher;
class IFlightDataService;

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *_pageStack;
    QWidget *_mainPage;
    IMapFetcher *_mapFetcher;
    IFlightDataService *_dataService;

    void initializePages();

public slots:
    void showMainPage();
};