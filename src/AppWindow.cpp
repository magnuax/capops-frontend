#include <QWidget>
#include <QStackedWidget>
#include <QStandardPaths>

#include "AppWindow.hpp"
#include "pages/MainPage.hpp"
#include "helpers/MapFetcherCARTO.hpp"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initializePages();
    showMainPage();
}


void AppWindow::initializePages()
{
    _mapFetcher = new MapFetcherCARTO(this);
    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/maptiles");

    _mainPage = new MainPage(_mapFetcher, this);
    _pageStack = new QStackedWidget(this);
    _pageStack->addWidget(_mainPage);

    setCentralWidget(_pageStack);
}

void AppWindow::showMainPage()
{
    _pageStack->setCurrentWidget(_mainPage);
}