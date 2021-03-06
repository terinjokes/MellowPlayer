import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import MellowPlayer 3.0

StackLayout {
    id: root

    signal newViewRequested(var request, var profile)
    signal fullScreenRequested(var request)
    property bool linkHovered: false

    function currentWebView() {
        return root.itemAt(root.currentIndex);
    }
    function goBack() {
        currentWebView().goBack();
    }
    function goHome() {
        var webView = currentWebView();
        webView.url = webView.urlToLoad
        webView.reload();
        weView.start();
    }
    function goForward() {
        currentWebView().goForward();
    }
    function reload() {
        currentWebView().reload();
        currentWebView().start();
    }
    function toList() {
        var list = []
        for(var i = 0; i < root.count; i++)
            list.push(root.itemAt(i));
        return list;
    }
    function updatePreviewImage(callback) {
        if (currentWebView() != null) {
            d.callback = callback
            currentWebView().updateImageFinished.connect(d.onUpdatePreviewImageFinished);
            currentWebView().updateImage();
        }
        else
            callback()
    }
    function exitFullScreen() {
        currentWebView().exitFullScreen();
    }

    function zoomIn() {
        currentWebView().zoomIn()
    }

    function zoomOut() {
         currentWebView().zoomOut()
    }

    currentIndex: _streamingServices.currentIndex

    Shortcut {
        sequence: "Ctrl++"
        onActivated: root.zoomIn()
    }

    Shortcut {
        sequence: "Ctrl+-"
        onActivated: root.zoomOut()
    }

    Shortcut {
        sequence: "Ctrl+0"
        onActivated: currentWebView().resetZoom()
    }

    QtObject {
        id: d

        property var callback

        function onUpdatePreviewImageFinished() {
            currentWebView().updateImageFinished.disconnect(d.onUpdatePreviewImageFinished);
            callback()
        }
    }

    Repeater {
        id: repeater

        model: _streamingServices.allServices
        delegate: WebView {
            id: webView

            anchors.fill: parent
            visible: root.currentIndex === _streamingServices.webViewIndex(model.name);
            enabled: visible
            player: model.player
            urlToLoad: model.url
            service: model.qtObject

            onCustomUrlSet: {
                model.url = customUrl
                start()
            }
            onFullScreenRequested: root.fullScreenRequested(request);
            onNewViewRequested: root.newViewRequested(request, webView.profile)

            onLinkHovered: root.linkHovered = hoveredUrl != ""
        }
    }

    Shortcut {
        property var setting: _settings.get(SettingKey.SHORTCUTS_SELECT_NEXT_SERVICE)

        sequence: setting.value
        onActivated: _streamingServices.next()
    }

    Shortcut {
        property var setting: _settings.get(SettingKey.SHORTCUTS_SELECT_PREVIOUS_SERVICE)

        sequence: setting.value
        onActivated: _streamingServices.previous()
    }

    Behavior on x {
        NumberAnimation { duration: 100 }
    }
}
