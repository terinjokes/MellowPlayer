#include "ApplicationViewModel.hpp"
#include "MellowPlayer/Presentation/IconProvider.hpp"
#include <MellowPlayer/Application/ListeningHistory/ListeningHistoryEntry.hpp>
#include <MellowPlayer/Application/Logging/LoggingManager.hpp>
#include <MellowPlayer/Application/Player/Player.hpp>
#include <MellowPlayer/Application/StreamingServices/StreamingService.hpp>
#include <MellowPlayer/Presentation/ViewModels/Settings/Types/SettingViewModel.hpp>
#include <QWebEngineProfile>
#include <QtWebEngine>

using namespace MellowPlayer::Application;
using namespace MellowPlayer::Application;
using namespace MellowPlayer::Presentation;

ApplicationViewModel::ApplicationViewModel(int& argc, char** argv, const QString& appName) : qtApp(argc, argv)
{
    qtApp.setApplicationDisplayName("MellowPlayer");
    qtApp.setApplicationName(appName);
    qtApp.setApplicationVersion(MELLOWPLAYER_VERSION);
    qtApp.setOrganizationDomain("org.mellowplayer");
    qtApp.setOrganizationName("MellowPlayer");
    qtApp.setWindowIcon(IconProvider::windowIcon());

    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Black.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Italic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Thin.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-ThinItalic.ttf");

    qRegisterMetaType<Song*>("Application::Song*");
    qRegisterMetaType<Song*>("Song*");
    qRegisterMetaType<StreamingService*>("Application::StreamingService*");
    qRegisterMetaType<StreamingService*>("StreamingService*");
    qRegisterMetaType<ListeningHistoryEntry>("Application::ListeningHistoryEntry");
    qRegisterMetaType<ListeningHistoryEntry>("ListeningHistoryEntry");
    qRegisterMetaType<Player*>("Application::Player*");
    qRegisterMetaType<Player*>("Player*");
    qRegisterMetaType<SettingViewModel*>("Presentation::SettingViewModel*");
    qRegisterMetaType<SettingViewModel*>("SettingViewModel*");

    connect(&qtApp, &QApplication::aboutToQuit, this, &ApplicationViewModel::onAboutToQuit);
}

int ApplicationViewModel::run()
{
    return qtApp.exec();
}

void ApplicationViewModel::clearCache() const
{
    QWebEngineProfile profile("Default");
    profile.clearHttpCache();
    QDir cacheDir(QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0]);
    qDebug() << "removing cache directory: " << cacheDir;
    cacheDir.removeRecursively();
}

void ApplicationViewModel::clearCookies() const
{
    QWebEngineProfile profile("Default");
    QDir storageDir(profile.persistentStoragePath());
    qDebug() << "removing persistent storage directory: " << storageDir;
    storageDir.removeRecursively();
}

void ApplicationViewModel::restart()
{
    restartRequested_ = true;
    qApp->quit();
}

void ApplicationViewModel::requestQuit()
{
    emit quitRequested();
}

void ApplicationViewModel::quit()
{
    qtApp.exit(0);
}

void ApplicationViewModel::setAutoQuitDelay(int delay)
{
    if (delay > 0)
        QTimer::singleShot(delay, qtApp.quit);
}

void ApplicationViewModel::onAboutToQuit()
{
    clearCache();
}

static QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang ") + QString::number(__clang_major__) + QLatin1Char('.') + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC ") + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER >= 1800) // 1800: MSVC 2013 (yearly release cycle)
        return QLatin1String("MSVC ") + QString::number(2008 + ((_MSC_VER / 100) - 13));
    if (_MSC_VER >= 1500) // 1500: MSVC 2008, 1600: MSVC 2010, ... (2-year release cycle)
        return QLatin1String("MSVC ") + QString::number(2008 + 2 * ((_MSC_VER / 100) - 15));
#else
    return QLatin1String("<unknown compiler>");
#endif
}

QString ApplicationViewModel::getBuildInfo() const
{

    return QObject::tr("Built on %1 at %2 (%3, %4 bit) with Qt %5")
    .arg(QString(__DATE__), QString(__TIME__), compilerString(), QString::number(QSysInfo::WordSize), QString(QT_VERSION_STR));
}

bool ApplicationViewModel::restartRequested() const
{
    return restartRequested_;
}
