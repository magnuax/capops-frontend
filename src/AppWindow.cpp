#include <QWidget>
#include <QStackedWidget>

#include "AppWindow.hpp"
#include "pages/MainPage.hpp"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initializePages();
    showMainPage();
}

void AppWindow::initializePages()
{
    _mainPage = new MainPage(this);
    _pageStack = new QStackedWidget(this);
    _pageStack->addWidget(_mainPage);
}

void AppWindow::showMainPage()
{
    _pageStack->setCurrentWidget(_mainPage);
}