#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "ImageProcessing.h"

void ImageProcessingPlugin::registerTypes(const char *uri) {
    //@uri ImageProcessing
    qmlRegisterSingletonType<ImageProcessing>(uri, 1, 0, "ImageProcessing", [](QQmlEngine*, QJSEngine*) -> QObject* { return new ImageProcessing; });
}
