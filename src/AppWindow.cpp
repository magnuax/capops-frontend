#include <QWidget>
#include <QStackedWidget>
#include <QStandardPaths>

#include "AppWindow.hpp"
#include "pages/MainPage.hpp"

#include "helpers/TileMapServiceCARTO.hpp"
#include "helpers/FlightDataServiceTest.hpp"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    _dataService = new FlightDataServiceTest();

    _mapFetcher = new TileMapServiceCARTO(this);
    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/maptiles");

    initializePages();
    showMainPage();
}


void AppWindow::initializePages()
{
    _mainPage = new MainPage(*_dataService, _mapFetcher, this);
    _pageStack = new QStackedWidget(this);
    _pageStack->addWidget(_mainPage);

    setCentralWidget(_pageStack);
}

void AppWindow::showMainPage()
{
    _pageStack->setCurrentWidget(_mainPage);
}