#pragma once

#include <QSystemTrayIcon>
#include <QMenu>
#include <MellowPlayer/UseCases/Interfaces/ISystemTrayIcon.hpp>
#include <MellowPlayer/Presentation/IconProvider.hpp>

PREDECLARE_MELLOWPLAYER_CLASS(UseCases, ILogger)
PREDECLARE_MELLOWPLAYER_CLASS(UseCases, IPlayer)
PREDECLARE_MELLOWPLAYER_CLASS(UseCases, IMainWindow)
PREDECLARE_MELLOWPLAYER_CLASS(UseCases, IQtApplication)
PREDECLARE_MELLOWPLAYER_CLASS(UseCases, Setting)
PREDECLARE_MELLOWPLAYER_CLASS(UseCases, Settings)

BEGIN_MELLOWPLAYER_NAMESPACE(Presentation)

class SystemTrayIcon: public QObject, public UseCases::ISystemTrayIcon {
    Q_OBJECT
public:
    SystemTrayIcon(UseCases::IPlayer& player, UseCases::IMainWindow& mainWindow,
                   UseCases::IQtApplication& qtApplication, UseCases::Settings& settings);
    void show() override;
    void hide() override;
    void showMessage(const QString &title, const QString &message) override;

public slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void togglePlayPause();
    void next();
    void previous();
    void restoreWindow();
    void quit();

private slots:
    void onShowTrayIconSettingValueChanged();

private:
    void setUpMenu();

    UseCases::ILogger& logger;
    UseCases::IPlayer& player;
    UseCases::IMainWindow& mainWindow;
    UseCases::IQtApplication& qtApplication;
    UseCases::Settings& settings;
    UseCases::Setting& showTrayIconSetting;

    QSystemTrayIcon qSystemTrayIcon;
    QMenu menu;
    QAction* playPauseAction;
    QAction* previousSongAction;
    QAction* nextSongAction;
    QAction* restoreWindowAction;
    QAction* quitApplicationAction;
};

END_MELLOWPLAYER_NAMESPACE