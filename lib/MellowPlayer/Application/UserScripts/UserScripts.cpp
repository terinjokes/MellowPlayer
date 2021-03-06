#include "IUserScript.hpp"
#include "IUserScriptFactory.hpp"
#include "UserScripts.hpp"
#include <MellowPlayer/Application/Settings/ISettingsProvider.hpp>

using namespace MellowPlayer::Application;

UserScripts::UserScripts(const QString& serviceName,
                         IUserScriptFactory& userScriptFactory,
                         ISettingsProvider& settingsProvider):
    serviceName_(serviceName),
    userScriptFactory_(userScriptFactory),
    settingsProvider_(settingsProvider)
{
    auto scriptPaths = settingsProvider.value(pathsKey(), QStringList()).toStringList();
    auto scriptNames = settingsProvider.value(namesKey(), QStringList()).toStringList();

    for (int i = 0; i < scriptPaths.count(); ++i) {
        auto path = scriptPaths.at(i);
        auto name = scriptNames.at(i);
        auto* userScript = userScriptFactory_.create();
        userScript->setName(name);
        if (userScript->load(path))
            _scripts.append(userScript);
    }
}

UserScripts::~UserScripts()
{
    qDeleteAll(_scripts);
}

int UserScripts::count() const
{
    return _scripts.count();
}

IUserScript* UserScripts::add(const QString& userScriptName, const QString& sourceScriptPath)
{
    auto* userScript = userScriptFactory_.create();
    userScript->setName(userScriptName);
    if (userScript->import(sourceScriptPath)) {
        _scripts.append(userScript);
        save(userScriptName, userScript);
        return userScript;
    }
    else {
        delete userScript;
        return nullptr;
    }

}

void UserScripts::save(const QString& userScriptName, const IUserScript* userScript) const
{
    auto scriptPaths = settingsProvider_.value(pathsKey(), QStringList()).toStringList();
    scriptPaths.append(userScript->path());
    auto scriptNames = settingsProvider_.value(namesKey(), QStringList()).toStringList();
    scriptNames.append(userScriptName);
    settingsProvider_.setValue(pathsKey(), scriptPaths);
    settingsProvider_.setValue(namesKey(), scriptNames);
}

void UserScripts::remove(const QString& scriptName)
{
    int index = 0;
    for (index = 0; index < _scripts.count(); ++index) {
        IUserScript* script = _scripts.at(index);
        if (script->name() == scriptName) {
            auto scriptPaths = settingsProvider_.value(pathsKey(), QStringList()).toStringList();
            auto scriptNames = settingsProvider_.value(namesKey(), QStringList()).toStringList();
            scriptNames.removeOne(_scripts.at(index)->name());
            scriptPaths.removeOne(_scripts.at(index)->path());
            settingsProvider_.setValue(pathsKey(), scriptPaths);
            settingsProvider_.setValue(namesKey(), scriptNames);
            script->removeFile();
            _scripts.removeAt(index);
            delete script;
            break;
        }
    }

}

UserScripts::const_iterator UserScripts::begin(void) const
{
    return _scripts.begin();
}

UserScripts::const_iterator UserScripts::end(void) const
{
    return _scripts.end();
}

QString UserScripts::pathsKey() const
{
    return serviceName_ + "/userScriptPaths";
}

QString UserScripts::namesKey() const
{
    return serviceName_ + "/userScriptNames";
}
