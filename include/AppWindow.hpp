#pragma once

#include <QMainWindow>

class QWidget;
class QStackedWidget;
class FlightDataServiceJSON;
class FlightDataServiceWS;
class ITileMapService;

class AppWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void showMainPage();

public:
    explicit AppWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initializePages();
    void initializeServices();

    QStackedWidget *_pageStack;
    QWidget *_mainPage;

    // FlightDataServiceJSON *_dataService;
    FlightDataServiceWS *_dataService;
    ITileMapService *_mapFetcher;
};