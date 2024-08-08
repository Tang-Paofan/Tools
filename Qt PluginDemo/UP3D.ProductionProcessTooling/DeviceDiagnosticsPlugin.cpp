#include "DeviceDiagnostics.h"
#include "DeviceDiagnosticsPlugin.h"

#include <QtCore/QtPlugin>

DeviceDiagnosticsPlugin::DeviceDiagnosticsPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void DeviceDiagnosticsPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
    if (initialized)
        return;

    initialized = true;
}

bool DeviceDiagnosticsPlugin::isInitialized() const
{
    return initialized;
}

QWidget *DeviceDiagnosticsPlugin::createWidget(QWidget *parent)
{
    return new DeviceDiagnostics(parent);
}

QString DeviceDiagnosticsPlugin::name() const
{
    return "DeviceDiagnostics";
}

QString DeviceDiagnosticsPlugin::group() const
{
    return "My Plugins";
}

QIcon DeviceDiagnosticsPlugin::icon() const
{
    return QIcon();
}

QString DeviceDiagnosticsPlugin::toolTip() const
{
    return QString();
}

QString DeviceDiagnosticsPlugin::whatsThis() const
{
    return QString();
}

bool DeviceDiagnosticsPlugin::isContainer() const
{
    return false;
}

QString DeviceDiagnosticsPlugin::domXml() const
{
    return "<widget class=\"DeviceDiagnostics\" name=\"DeviceDiagnostics\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>100</width>\n"
        "   <height>100</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QString DeviceDiagnosticsPlugin::includeFile() const
{
    return "DeviceDiagnostics.h";
}
