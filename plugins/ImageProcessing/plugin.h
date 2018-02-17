#ifndef IMAGEPROCESSINGPLUGIN_H
#define IMAGEPROCESSINGPLUGIN_H

#include <QQmlExtensionPlugin>

class ImageProcessingPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif
