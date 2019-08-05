#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "ImageProcessing.h"

using namespace DocumentScanner;

void ImageProcessingPlugin::registerTypes(const char *uri)
{
    //@uri ImageProcessing
    qmlRegisterSingletonType<ImageProcessing>(uri, 1, 0, "ImageProcessing", [](QQmlEngine*, QJSEngine*) -> QObject* { return new ImageProcessing(); });
}

void ImageProcessingPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    engine->addImageProvider(QLatin1String("documents"), DocumentStore::instance());
}
