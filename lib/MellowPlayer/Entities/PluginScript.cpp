#include "PluginScript.hpp"

USE_MELLOWPLAYER_NAMESPACE(Entities)

PluginScript::PluginScript(const QString &code, const QString &path):
        QObject(), code(code), path(path) {

}

bool PluginScript::isValid() const {
    return code.contains("function update") && code.contains("function play") && (code.contains("function pause") &&
           code.contains("function goNext") && code.contains("function goPrevious") &&
           code.contains("function setVolume") && code.contains("function addToFavorites")) &&
           code.contains("function removeFromFavorites") && code.contains("function seekToPosition");
}

QString PluginScript::getCode() const {
    return code;
}

void PluginScript::setCode(const QString &value) {
    if (code == value)
        return;
    code = value;
    emit codeChanged(value);
}

const QString &PluginScript::getPath() const {
    return path;
}

QString PluginScript::update() const {
    return "if (typeof update !== 'undefined') update();";
}

QString PluginScript::play() const {
    return "play();";
}

QString PluginScript::pause() const {
    return "pause();";
}

QString PluginScript::next() const {
    return "goNext();";
}

QString PluginScript::previous() const {
    return "goPrevious();";
}

QString PluginScript::setVolume(double volume) const {
    return QString("setVolume(%1);").arg(volume);
}

QString PluginScript::addToFavorites() const {
    return "addToFavorites();";
}

QString PluginScript::removeFromFavorites() const {
    return "removeFromFavorites();";
}

QString PluginScript::seekToPosition(double position) const {
    return QString("seekToPosition(%1);").arg(position);
}

QString PluginScript::getConstants() const {
    return  "mellowplayer = {\n"
            "    PlaybackStatus: {\n"
            "        STOPPED: 0,\n"
            "        PLAYING: 1,\n"
            "        PAUSED: 2,\n"
            "        BUFFERING: 3\n"
            "    }\n"
            "};\n";
}