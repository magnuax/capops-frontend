#pragma once

#include <QMainWindow>

class QWidget;
class QStackedWidget;
class FlightDataServiceJSON;
class ITileMapService;

class AppWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void showMainPage();

public:
    explicit AppWindow(QWidget *parent = nullptr);

private:
    void initializePages();
    void initializeServices();

    QStackedWidget *_pageStack;
    QWidget *_mainPage;

    FlightDataServiceJSON *_dataService;
    FlightDataServiceJSON *_dataEvents;
    ITileMapService *_mapFetcher;
};