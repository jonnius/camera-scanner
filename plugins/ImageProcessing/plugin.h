#ifndef IMAGEPROCESSINGPLUGIN_H
#define IMAGEPROCESSINGPLUGIN_H

#include <QQmlExtensionPlugin>
#include "DocumentStore.h"

/**
  * QML Plugin. Auto-generated by clickable.
  */
class ImageProcessingPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
    void initializeEngine(QQmlEngine *engine, const char *uri) override;

private:
	static DocumentScanner::DocumentStore m_store;
};

#endif
