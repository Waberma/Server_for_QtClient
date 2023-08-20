#include "plugin.h"

Plugin::Plugin()
{
}

Plugin::~Plugin()
{

}

QString Plugin::getNamePlugin()
{
    return "Сервер";
}

QObject *Plugin::getPluginWidget()
{
    MyServer *widg = new MyServer();
    return qobject_cast<QObject *>(widg);
}

QMenu *Plugin::getMenu()
{

}

