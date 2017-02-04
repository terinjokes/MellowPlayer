#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <MellowPlayer/UseCases.hpp>
#include <MellowPlayer/Infrastructure.hpp>

USE_MELLOWPLAYER_NAMESPACE(UseCases)
USE_MELLOWPLAYER_NAMESPACE(Infrastructure)

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(presentation);

    // Init Qt Application
    QGuiApplication qtApp(argc, argv);
    qtApp.setApplicationDisplayName("MellowPlayer");
    qtApp.setApplicationName("MellowPlayer3");
    qtApp.setApplicationVersion(MELLOWPLAYER_VERSION);
    qtApp.setOrganizationDomain("org.mellowplayer");
    qtApp.setOrganizationName("MellowPlayer");
    QtWebEngine::initialize();
    QQmlApplicationEngine qmlApplicationEngine;

    // Init logging system
    SpdLoggerFactory loggerFactory;
    LoggingManager& loggingManager = LoggingManager::initialize(loggerFactory);
    loggingManager.setDefaultLogLevel(LogLevel::Info);
    ILogger& logger = loggingManager.getLogger("main");
    LOG_INFO(logger, "-------------------------------------------------------------------------------");
    LOG_INFO(logger, "MellowPlayer v" << MELLOWPLAYER_VERSION << " started");
    LOG_INFO(logger, "Log directory: " + FileHelper::logDirectory());

    // Init systems
    PluginLoader pluginLoader(loggingManager);
    PluginManager pluginManager(pluginLoader);

    // Load qml application
    LOG_TRACE(logger, "Loading qml application");
    qmlApplicationEngine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    LOG_TRACE(logger, "Qml application loaded");

    LOG_TRACE(logger, "Running")
    return qtApp.exec();
}