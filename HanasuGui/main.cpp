
#include <QGuiApplication>
#include <QQuickView>
#include <QQuickStyle>
#include <QQmlContext>
#include <QQmlEngine>
#include <QFontDatabase>

#include "controler.h"
#include "conversations.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QFontDatabase::addApplicationFont("fonts/NotoColorEmoji.ttf");
    QFontDatabase::addApplicationFont("fonts/icon-font.ttf");

    QQuickView view;

    Controler c;

    view.engine()->rootContext()->setContextProperty("c", &c);
    view.engine()->rootContext()->setContextProperty("cs", c.getConv());
    view.engine()->rootContext()->setContextProperty("msgs", c.getMsgList());
    view.engine()->rootContext()->setContextProperty("fmodel", c.getFModel());

    view.setSource(QUrl("qrc:/main.qml"));

    return app.exec();
}
