#include <QtCore>

#ifdef Q_OS_LINUX

#include "MellowPlayer/Infrastructure/Platform/Linux/Mpris/Mpris2Player.hpp"
#include "MellowPlayer/Infrastructure/Platform/Linux/Mpris/Mpris2Root.hpp"
#include "MprisController.hpp"
#include <MellowPlayer/Application/Logging/ILogger.hpp>
#include <MellowPlayer/Application/Logging/LoggingManager.hpp>
#include <MellowPlayer/Application/Logging/LoggingMacros.hpp>


using namespace MellowPlayer::Application;
using namespace MellowPlayer::Infrastructure;
using namespace std;

QString MprisController::SERVICE_NAME = "org.mpris.MediaPlayer2.";

QString MprisController::OBJECT_NAME = "/org/mpris/MediaPlayer2";

MprisController::MprisController(IPlayer& player, ILocalAlbumArt& localAlbumArt, IMainWindow& window, IQtApplication& application)
        : logger_(LoggingManager::logger("Mpris")),
          parent_(make_unique<QObject>()),
          mpris2Root_(new Mpris2Root(window, application, parent_.get())),
          mpris2Player_(new Mpris2Player(player, localAlbumArt, parent_.get())),
          serviceName_(SERVICE_NAME + qApp->applicationName())
{
}

MprisController::~MprisController()
{
    QDBusConnection::sessionBus().unregisterObject(OBJECT_NAME);
    QDBusConnection::sessionBus().unregisterObject(serviceName_);
}

bool MprisController::start()
{
    if (!QDBusConnection::sessionBus().registerService(serviceName_) || !QDBusConnection::sessionBus().registerObject(OBJECT_NAME, parent_.get())) {
        LOG_WARN(logger_, "failed to register service on the session bus: " + serviceName_);
        LOG_WARN(logger_, "failed to register object on the session bus: " + OBJECT_NAME);
        return false;
    }
    LOG_DEBUG(logger_, "service started: " + serviceName_);
    return true;
}

#endif