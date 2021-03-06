#include "LatestGithubReleaseQuerier.hpp"
#include <MellowPlayer/Application/Logging/ILogger.hpp>
#include <MellowPlayer/Application/Logging/LoggingManager.hpp>
#include <MellowPlayer/Application/Logging/LoggingMacros.hpp>
#include <MellowPlayer/Application/Updater/IHttpClient.hpp>
#include <MellowPlayer/Application/Updater/Release.hpp>


#define CONTINUOUS_RELEASE_NAME "Continuous"

using namespace MellowPlayer::Application;

LatestGithubReleaseQuerier::LatestGithubReleaseQuerier(IHttpClient& httpClient)
        : logger_(LoggingManager::logger("LatestGithubReleaseQuerier")), httpClient_(httpClient)
{
    connect(&httpClient, &IHttpClient::replyReceived, this, &LatestGithubReleaseQuerier::onReplyReceived);
    connect(&replyParser_, &GithubReleasesReplyParser::ready, this, &LatestGithubReleaseQuerier::onReleaseReady);
}

void LatestGithubReleaseQuerier::setChannel(UpdateChannel channel)
{
    channel_ = channel;
}

void LatestGithubReleaseQuerier::query()
{
    LOG_DEBUG(logger_, "getting releases");
    found_ = false;
    httpClient_.get("https://api.github.com/repos/ColinDuquesnoy/MellowPlayer/releases");
}

void LatestGithubReleaseQuerier::onReplyReceived(const QByteArray& replyData)
{
    LOG_DEBUG(logger_, "Reply recevied");
    LOG_TRACE(logger_, "reply data: " << replyData.toStdString());
    replyParser_.parse(replyData);
    if (!found_) {
        LOG_DEBUG(logger_, "no release found");
        emit latestReceived(nullptr);
    }
}

void LatestGithubReleaseQuerier::onReleaseReady(const Release* release)
{
    if (release != nullptr && accept(release)) {
        LOG_DEBUG(logger_, "Latest release found: " << release->name());
        emit latestReceived(release);
        replyParser_.stop();
        found_ = true;
    }
}

bool LatestGithubReleaseQuerier::accept(const Release* release)
{
    bool accepted = false;

    switch (channel_) {
        case UpdateChannel::Continuous:
            accepted = release->name() == CONTINUOUS_RELEASE_NAME;
            break;
        case UpdateChannel::Beta:
            accepted = release->isPreRelease() && release->name() != CONTINUOUS_RELEASE_NAME;
            break;
        case UpdateChannel::Stable:
            accepted = !release->isPreRelease() && release->name() != CONTINUOUS_RELEASE_NAME;
            break;
    }

    return accepted;
}
