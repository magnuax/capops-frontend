#include <QApplication>

#include "AppWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppWindow window;
    window.show();

    return app.exec();
}