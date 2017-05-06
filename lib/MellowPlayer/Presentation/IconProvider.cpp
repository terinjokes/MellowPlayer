#include <MellowPlayer/UseCases/Interfaces/IApplicationSettings.hpp>
#include "IconProvider.hpp"

USE_MELLOWPLAYER_NAMESPACE(UseCases)
USE_MELLOWPLAYER_NAMESPACE(Presentation)

IconProvider::IconProvider(IApplicationSettings& applicationSettings) : applicationSettings(applicationSettings) {

}

QIcon IconProvider::trayIcon() {
    QString iconString = applicationSettings.getTrayIcon();
    if (iconString.trimmed().isEmpty())
        return windowIcon();
    else if (QFile::exists(iconString))
        return QIcon(iconString);
    else
        return QIcon::fromTheme(iconString);
}

QIcon IconProvider::windowIcon() {
    return QIcon::fromTheme("mellowplayer", QIcon("://MellowPlayer/Presentation/Resources/icons/mellowplayer.png"));
}

QIcon IconProvider::restoreWindow() {
    return QIcon::fromTheme("view-restore", QIcon("://MellowPlayer/Presentation/Resources/icons/view-restore.png"));
}

QIcon IconProvider::play() {
    return QIcon::fromTheme("media-playback-start",
                            QIcon("://MellowPlayer/Presentation/Resources/icons/media-playback-start.png"));
}

QIcon IconProvider::pause() {
    return QIcon::fromTheme("media-playback-start",
                            QIcon("://MellowPlayer/Presentation/Resources/icons/media-playback-pause.png"));
}

QIcon IconProvider::next() {
    return QIcon::fromTheme("media-seek-forward",
                            QIcon("://MellowPlayer/Presentation/Resources/icons/media-seek-forward.png"));
}

QIcon IconProvider::previous() {
    return QIcon::fromTheme("media-seek-backward",
                            QIcon("://MellowPlayer/Presentation/Resources/icons/media-seek-backward.png"));
}

QIcon IconProvider::quit() {
    return QIcon::fromTheme("application-exit",
                            QIcon("://MellowPlayer/Presentation/Resources/icons/application-exit.png"));
}