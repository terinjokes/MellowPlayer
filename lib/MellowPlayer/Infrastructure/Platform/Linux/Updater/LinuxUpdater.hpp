#pragma once

#include <MellowPlayer/Application/Updater/AbstractPlatformUpdater.hpp>
#include <MellowPlayer/Application/Updater/Asset.hpp>

namespace MellowPlayer::Application {
    class IFileDownloader;
    class Release;
}

namespace MellowPlayer::Infrastructure
{
    class LinuxUpdater : public Application::AbstractPlatformUpdater
    {
    public:
        explicit LinuxUpdater(Application::IFileDownloader& fileDownloader);

        void setRelease(const Application::Release* release) override;
        bool canInstall() const override;
        void restart() override;

    protected:
        void doInstall(const QString& assetLocalPath) override;
        QString assetUrl() const override;
        QString assetFileName() const override;
        Application::Asset asset_;
        QString destinationDir() const;
        QString destinationPath() const;
    };
}
