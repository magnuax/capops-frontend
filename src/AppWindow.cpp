#include <QWidget>
#include <QStackedWidget>
#include <QStandardPaths>

#include "AppWindow.hpp"
#include "pages/MainPage.hpp"

#include "services/WebSocketClient.hpp"
#include "services/FlightDataServiceJSON.hpp"
#include "services/TileMapServiceCARTO.hpp"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initializeServices();
    wireServices();

    initializePages();
    showMainPage();
}

void AppWindow::initializeServices()
{
    _webSocket = new WebSocketClient(this);
    _dataService = new FlightDataServiceJSON("./resources/test_data.json");
    _dataEvents = _dataService;
    _mapFetcher = new TileMapServiceCARTO(this);

    _mapFetcher->enableDiskCache(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/maptiles");
}

void AppWindow::wireServices()
{
    // --- Wire WebSocket updates to service ---
    /*

    connect(_webSocket, &WebSocketClient::sectorRiskReceived,
            _dataService, &FlightDataServiceWS::updateSectorRisk);

    connect(_webSocket, &WebSocketClient::sectorWeatherReceived,
            _dataService, &FlightDataServiceWS::updateSectorWeather);

    connect(_webSocket, &WebSocketClient::sectorTrafficReceived,
            _dataService, &FlightDataServiceWS::updateSectorTraffic);

    connect(_webSocket, &WebSocketClient::sectorFlightsReceived,
            _dataService, &FlightDataServiceWS::updateSectorFlights);

    // etc...

    */
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