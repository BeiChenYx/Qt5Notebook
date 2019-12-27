#include "gaugearcplugin.h"
#include "gaugecarplugin.h"
#include "gaugecircleplugin.h"
#include "gaugeclockplugin.h"
#include "gaugecompassplugin.h"
#include "gaugedialplugin.h"
#include "gaugeminiplugin.h"
#include "gaugeplaneplugin.h"
#include "gaugeprogressbarplugin.h"
#include "gaugerangeplugin.h"
#include "gaugeroundplugin.h"
#include "gaugesimpleplugin.h"
#include "gaugewatchplugin.h"
#include "gaugenetplugin.h"
#include "gaugespeedplugin.h"
#include "gaugelpmplugin.h"
#include "gaugecolorplugin.h"

#include "progressbarcolorplugin.h"
#include "progressbarpercentplugin.h"
#include "progressbarroundplugin.h"
#include "progressbarwaitplugin.h"
#include "progressbarwaterplugin.h"

#include "navbarplugin.h"
#include "navbuttonplugin.h"
#include "navlabelplugin.h"
#include "navlistviewplugin.h"
#include "navprogressbarplugin.h"

#include "wavebarplugin.h"
#include "wavechartplugin.h"
#include "wavedataplugin.h"
#include "wavedoubleplugin.h"
#include "wavelineplugin.h"
#include "waveplotplugin.h"

#include "rulerbarplugin.h"
#include "rulerlineplugin.h"
#include "rulersliderplugin.h"
#include "rulerdoubleplugin.h"
#include "rulertempplugin.h"
#include "rulerscaleplugin.h"

#include "sliderselectplugin.h"
#include "sliderrangeplugin.h"
#include "slidertipplugin.h"

#include "barvolumeplugin.h"
#include "barvumeterplugin.h"

#include "colorcomboboxplugin.h"
#include "colorpanelbarplugin.h"
#include "colorpanelfaderplugin.h"
#include "colorpanelhsbplugin.h"
#include "colorpanelbtnplugin.h"
#include "colorbuttonplugin.h"
#include "colorstyleplugin.h"

#include "adswidgetplugin.h"
#include "animationbutton1plugin.h"
#include "animationbutton2plugin.h"
#include "buttondefenceplugin.h"
#include "imagecalendarplugin.h"
#include "imageclockplugin.h"
#include "imagepilotplugin.h"
#include "imagepowerplugin.h"
#include "imageswitchplugin.h"
#include "imageviewplugin.h"

#include "batteryplugin.h"
#include "lightbuttonplugin.h"
#include "lightpointplugin.h"
#include "magicfishplugin.h"
#include "magicmouseplugin.h"
#include "magicpoolplugin.h"
#include "roundcircleplugin.h"
#include "roundwidgetplugin.h"
#include "switchbuttonplugin.h"
#include "oventimerplugin.h"
#include "tumblerplugin.h"
#include "tumblerdatetimeplugin.h"
#include "lunarcalendaritemplugin.h"
#include "lunarcalendarwidgetplugin.h"
#include "tiledbgplugin.h"

#include "comboboxplugin.h"
#include "lcddatetimeplugin.h"
#include "lcdnumberplugin.h"
#include "lednumberplugin.h"
#include "ledtextplugin.h"
#include "devicesizetableplugin.h"
#include "cpumemorylabelplugin.h"
#include "textmovewidgetplugin.h"
#include "panelitemplugin.h"
#include "panelwidgetplugin.h"
#include "verifiedcodeplugin.h"
#include "iconeditorplugin.h"
#include "ipaddressplugin.h"

#include "allcontrol.h"

AllControl::AllControl(QObject *parent) : QObject(parent)
{
    widgets.append(new GaugeArcPlugin(this));
    widgets.append(new GaugeCarPlugin(this));
    widgets.append(new GaugeCirclePlugin(this));
    widgets.append(new GaugeClockPlugin(this));
    widgets.append(new GaugeCompassPlugin(this));
    widgets.append(new GaugeDialPlugin(this));
    widgets.append(new GaugeMiniPlugin(this));
    widgets.append(new GaugePlanePlugin(this));
    widgets.append(new GaugeProgressBarPlugin(this));
    widgets.append(new GaugeRoundPlugin(this));
    widgets.append(new GaugeRangePlugin(this));
    widgets.append(new GaugeSimplePlugin(this));
    widgets.append(new GaugeWatchPlugin(this));
    widgets.append(new GaugeNetPlugin(this));
    widgets.append(new GaugeSpeedPlugin(this));
    widgets.append(new GaugeLpmPlugin(this));
    widgets.append(new GaugeColorPlugin(this));

    widgets.append(new ProgressBarColorPlugin(this));
    widgets.append(new ProgressBarPercentPlugin(this));
    widgets.append(new ProgressBarRoundPlugin(this));
    widgets.append(new ProgressBarWaitPlugin(this));
    widgets.append(new ProgressBarWaterPlugin(this));

    widgets.append(new NavBarPlugin(this));
    widgets.append(new NavButtonPlugin(this));
    widgets.append(new NavLabelPlugin(this));    
    widgets.append(new NavListViewPlugin(this));
    widgets.append(new NavProgressBarPlugin(this));

    widgets.append(new WaveBarPlugin(this));
    widgets.append(new WaveChartPlugin(this));
    widgets.append(new WaveDataPlugin(this));
    widgets.append(new WaveDoublePlugin(this));
    widgets.append(new WaveLinePlugin(this));
    widgets.append(new WavePlotPlugin(this));

    widgets.append(new RulerBarPlugin(this));
    widgets.append(new RulerLinePlugin(this));
    widgets.append(new RulerSliderPlugin(this));
    widgets.append(new RulerDoublePlugin(this));
    widgets.append(new RulerTempPlugin(this));
    widgets.append(new RulerScalePlugin(this));

    widgets.append(new SliderSelectPlugin(this));
    widgets.append(new SliderRangePlugin(this));    
    widgets.append(new SliderTipPlugin(this));

    widgets.append(new BarVolumePlugin(this));
    widgets.append(new BarVUMeterPlugin(this));

    widgets.append(new ColorComboBoxPlugin(this));
    widgets.append(new ColorPanelBarPlugin(this));
    widgets.append(new ColorPanelFaderPlugin(this));
    widgets.append(new ColorPanelHSBPlugin(this));
    widgets.append(new ColorPanelBtnPlugin(this));
    widgets.append(new ColorButtonPlugin(this));
    widgets.append(new ColorStylePlugin(this));

    widgets.append(new AdsWidgetPlugin(this));
    widgets.append(new AnimationButton1Plugin(this));
    widgets.append(new AnimationButton2Plugin(this));    
    widgets.append(new ButtonDefencePlugin(this));
    widgets.append(new ImageCalendarPlugin(this));
    widgets.append(new ImageClockPlugin(this));
    widgets.append(new ImagePilotPlugin(this));
    widgets.append(new ImagePowerPlugin(this));
    widgets.append(new ImageSwitchPlugin(this));
    widgets.append(new ImageViewPlugin(this));

    widgets.append(new BatteryPlugin(this));
    widgets.append(new LightButtonPlugin(this));
    widgets.append(new LightPointPlugin(this));
    widgets.append(new MagicFishPlugin(this));
    widgets.append(new MagicMousePlugin(this));
    widgets.append(new MagicPoolPlugin(this));
    widgets.append(new RoundCirclePlugin(this));
    widgets.append(new RoundWidgetPlugin(this));
    widgets.append(new SwitchButtonPlugin(this));
    widgets.append(new OvenTimerPlugin(this));
    widgets.append(new TumblerPlugin(this));
    widgets.append(new TumblerDateTimePlugin(this));
    widgets.append(new LunarCalendarItemPlugin(this));
    widgets.append(new LunarCalendarWidgetPlugin(this));
    widgets.append(new TiledBgPlugin(this));

    widgets.append(new ComboBoxPlugin(this));
    widgets.append(new LcdDateTimePlugin(this));
    widgets.append(new LcdNumberPlugin(this));
    widgets.append(new LedNumberPlugin(this));
    widgets.append(new LedTextPlugin(this));
    widgets.append(new DeviceSizeTablePlugin(this));
    widgets.append(new CpuMemoryLabelPlugin(this));
    widgets.append(new TextMoveWidgetPlugin(this));
    widgets.append(new PanelItemPlugin(this));
    widgets.append(new PanelWidgetPlugin(this));
    widgets.append(new VerifiedCodePlugin(this));
    widgets.append(new IconEditorPlugin(this));
    widgets.append(new IPAddressPlugin(this));   
}

QList<QDesignerCustomWidgetInterface *> AllControl::customWidgets() const
{
    return widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(allcontrolplugin, AllControl)
#endif // QT_VERSION < 0x050000
