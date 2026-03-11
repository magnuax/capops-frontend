#pragma once

#include <QMainWindow>

class QWidget;
class QStackedWidget;
class WebSocketClient;
class FlightDataServiceTest;
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
    void wireServices();

    QStackedWidget *_pageStack;
    QWidget *_mainPage;

    FlightDataServiceTest *_dataService;
    FlightDataServiceTest *_dataEvents;
    WebSocketClient *_webSocket;
    ITileMapService *_mapFetcher;
};