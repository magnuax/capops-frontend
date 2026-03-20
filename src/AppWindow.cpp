#include <QWidget>
#include <QStackedWidget>
#include <QStandardPaths>

#include "AppWindow.hpp"
#include "pages/MainPage.hpp"

#include "services/FlightDataServiceJSON.hpp"
#include "services/TileMapServiceCARTO.hpp"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initializeServices();
    initializePages();
    showMainPage();
}

void AppWindow::initializeServices()
{
    _dataService = new FlightDataServiceJSON("./resources/test_data.json");
    _dataEvents = _dataService;
    _mapFetcher = new TileMapServiceCARTO(this);

    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/maptiles");
}

void AppWindow::initializePages()
{
    _mainPage = new MainPage(
        *_dataService,
        _dataEvents,
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