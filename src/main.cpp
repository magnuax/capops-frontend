#include <QApplication>
#include <QFile>

#include "AppWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/stylesheet.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    app.setStyleSheet(style);

    AppWindow window;
    window.show();

    return app.exec();
}