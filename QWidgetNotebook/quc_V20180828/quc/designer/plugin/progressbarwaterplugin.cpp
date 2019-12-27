#include "../include/progressbarwater.h"
#include "progressbarwaterplugin.h"

#include <QtPlugin>

ProgressBarWaterPlugin::ProgressBarWaterPlugin(QObject *parent) : QObject(parent)
{
    m_initialized = false;
}

void ProgressBarWaterPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ProgressBarWaterPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ProgressBarWaterPlugin::createWidget(QWidget *parent)
{
    return new ProgressBarWater(parent);
}

QString ProgressBarWaterPlugin::name() const
{
    return QLatin1String("ProgressBarWater");
}

QString ProgressBarWaterPlugin::group() const
{
    return QLatin1String("Quc Widgets");
}

QIcon ProgressBarWaterPlugin::icon() const
{
    return QIcon(QLatin1String(":/ico/progressbarwater.png"));
}

QString ProgressBarWaterPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ProgressBarWaterPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ProgressBarWaterPlugin::isContainer() const
{
    return false;
}

QString ProgressBarWaterPlugin::domXml() const
{
    return QLatin1String("<widget class=\"ProgressBarWater\" name=\"progressBarWater\">\n</widget>\n");
}

QString ProgressBarWaterPlugin::includeFile() const
{
    return QLatin1String("progressbarwater.h");
}

