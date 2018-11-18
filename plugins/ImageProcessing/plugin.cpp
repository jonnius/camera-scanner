#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "ImageProcessing.h"

void ImageProcessingPlugin::registerTypes(const char *uri)
{
    //@uri ImageProcessing
    qmlRegisterSingletonType<ImageProcessing>(uri, 1, 0, "ImageProcessing", [](QQmlEngine*, QJSEngine*) -> QObject* { return new ImageProcessing(m_store); });
}

void ImageProcessingPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    engine->addImageProvider(QLatin1String("documents"), &m_store);
}

