#include <QApplication>
#include <QDir>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QPalette>
#include <QSurfaceFormat>
#include <QTimer>
#include <QDebug>
#include <QFont>

#include "domostyle.h"
#include "powermanager.h"
#include "resource/audioaccessor.h"
#include "resource/audiostate.h"
#include "resource/configurationmanager.h"
#include "resource/dailyhotperiods.h"
#include "resource/fireaccessor.h"
#include "resource/firemode.h"
#include "resource/forecastdata.h"
#include "resource/gpiocontroller.h"
#include "resource/hotperiod.h"
#include "resource/resourceaccessor.h"
#include "resource/shutter.h"
#include "resource/shutteraccessor.h"
#include "resource/shuttercommand.h"
#include "resource/temperatureaccessor.h"
#include "resource/temperatureconfiguration.h"
#include "resource/temperaturethresholds.h"
#include "resource/timeaccessor.h"
#include "resource/weatheraccessor.h"
#include "resource/wifiaccessor.h"
#include "resource/wifistate.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setFont(QFont("Ubuntu"));
    app.setApplicationName("Domobox");
    app.setOrganizationName("EMA");
    #ifndef ENV_SIMULATOR
    app.setOverrideCursor(Qt::BlankCursor);
    #endif

    QDir::setCurrent(app.applicationDirPath());

    DomoStyle *style = new DomoStyle(&app);
    ConfigurationManager::init(&app);
    GpioController::init(&app);
    ResourceAccessor::init(&app);
    app.installEventFilter(new PowerManager(&app));

    QObject::connect(ResourceAccessor::get()->getWeather(), &WeatherAccessor::dayModeChanged,
                     style,                                 &DomoStyle::onDayModeChanged);

    qmlRegisterType<TimeAccessor>("Domobox", 1, 0, "TimeAccessor");
    qmlRegisterType<TemperatureAccessor>("Domobox", 1, 0, "TemperatureAccessor");
    qmlRegisterType<WeatherAccessor>("Domobox", 1, 0, "WeatherAccessor");
    qmlRegisterType<ForecastData>("Domobox", 1, 0, "ForecastData");
    //qmlRegisterType<WifiAccessor>("Domobox", 1, 0, "WifiAccessor");
    //qmlRegisterType<WifiState>("Domobox", 1, 0, "WifiState");
    qmlRegisterType<AudioAccessor>("Domobox", 1, 0, "AudioAccessor");
    qmlRegisterType<AudioState>("Domobox", 1, 0, "AudioState");
    qmlRegisterType<FireAccessor>("Domobox", 1, 0, "FireAccessor");
    qmlRegisterType<FireMode>("Domobox", 1, 0, "FireMode");
    qmlRegisterType<ShutterAccessor>("Domobox", 1, 0, "ShutterAccessor");
    qmlRegisterType<TemperatureConfiguration>("Domobox", 1, 0, "TemperatureConfiguration");
    qmlRegisterType<TemperatureThresholds>("Domobox", 1, 0, "TemperatureThresholds");
    qmlRegisterType<DailyHotPeriods>("Domobox", 1, 0, "DailyHotPeriods");
    qmlRegisterType<HotPeriod>("Domobox", 1, 0, "HotPeriod");
    qmlRegisterType<ShutterCommand>("Domobox", 1, 0, "ShutterCommand");
    qmlRegisterType<Shutter>("Domobox", 1, 0, "Shutter");

    std::srand(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());

    QSurfaceFormat format;
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("domoStyle", style);
    engine.rootContext()->setContextProperty("resource", ResourceAccessor::get());
    #ifdef ENV_SIMULATOR
    engine.rootContext()->setContextProperty("ENV_SIMULATOR", QVariant(true));
    #else
    engine.rootContext()->setContextProperty("ENV_SIMULATOR", QVariant(false));
    #endif
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl)
                           {
                                if(url == objUrl)
                                {
                                    if(!obj)
                                    {
                                        QCoreApplication::exit(-1);
                                    }
                                }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
