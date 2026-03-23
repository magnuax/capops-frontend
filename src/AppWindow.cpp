#include <QWidget>
#include <QStackedWidget>
#include <QStandardPaths>

#include <QKeyEvent>
#include <QApplication>
#include <QFont>

#include <QFile>
#include <QRegularExpression>

#include "AppWindow.hpp"
#include "pages/MainPage.hpp"

#include "services/FlightDataServiceJSON.hpp"
#include "services/FlightDataServiceWS.hpp"
#include "services/TileMapServiceCARTO.hpp"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initializeServices();
    initializePages();
    showMainPage();
}

void AppWindow::initializeServices()
{

    /**
     *
     * _dataService = new FlightDataServiceJSON("./resources/test_data.json");
     */

    _dataService = new FlightDataServiceWS(
        "ws://jacobgomezhansen.net:8081",
        "http://jacobgomezhansen.net:8080",
        this);

    _dataService->connectToServer();
    
    _mapFetcher = new TileMapServiceCARTO(this);

    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/maptiles");
}

void AppWindow::initializePages()
{
    _mainPage = new MainPage(
        _dataService,
        _mapFetcher,
        this);

    _pageStack = new QStackedWidget(this);
    _pageStack->addWidget(_mainPage);

    setCentralWidget(_pageStack);
}

void AppWindow::showMainPage()
{
    _pageStack->setCurrentWidget(_mainPage);
}

void AppWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
    {
        isFullScreen() ? showNormal() : showFullScreen();
        return;
    }

    QMainWindow::keyPressEvent(event);
}