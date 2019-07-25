#include <QDebug>
#include <QtGui/QImage>
#include <QString>

#include <iostream>
#include <vector>
#include <cmath>

#include <QFileInfo>
#include <QDir>

#include "ImageProcessing.h"

using namespace DocumentScanner;

ImageProcessing::ImageProcessing() : m_store(*DocumentStore::instance())
{
    /* empty */
    //TODO define default params
    //TODO load params from config
    auto config = new ExtractorConfig(this);
    setConfig(config);
}

void ImageProcessing::setConfig(ExtractorConfig* config)
{
  m_config = config;
  connect(m_config, &ExtractorConfig::filterModeChanged,
          this, &ImageProcessing::onConfigChanged);
  connect(m_config, &ExtractorConfig::colorModeChanged,
          this, &ImageProcessing::onConfigChanged);
  connect(m_config, &ExtractorConfig::colorThrChanged,
          this, &ImageProcessing::onConfigChanged);
  connect(m_config, &ExtractorConfig::colorGainChanged,
          this, &ImageProcessing::onConfigChanged);  
  connect(m_config, &ExtractorConfig::colorBiasChanged,
          this, &ImageProcessing::onConfigChanged);
}

ExtractorConfig * ImageProcessing::config()
{
  return m_config;
}

void ImageProcessing::onConfigChanged()
{
  qDebug() << "Fast geschafft";
  setConfig(m_config->colorMode(), m_config->filterMode(), m_config->colorThr(), m_config->colorGain(), m_config->colorBias());
}

void ImageProcessing::restoreCache()
{
    for (QString id : m_store.restoreCache())
        emit imageAdded(id);
}

void ImageProcessing::addImage(const QString &imageURL)
{
    QString id = m_store.addDocument(imageURL);
    m_store.cacheDocument(id);
    emit imageAdded(id);
}

void ImageProcessing::removeImage(const QString &id)
{
    m_store.removeDocument(id);
    emit imageRemoved(id);
}

void ImageProcessing::removeAll()
{
    for (QString id : m_store.getIDs())
        removeImage(id);
}

QString ImageProcessing::exportAsPdf(const QString &id) const
{
    QStringList ids = { id };
    return m_store.exportPdf(ids);
}

QString ImageProcessing::exportAllAsPdf() const
{
    return m_store.exportPdf(m_store.getIDs());
}

QString ImageProcessing::exportAsImage(const QString &id) const
{
    return m_store.getImageURL(id);
}

QStringList ImageProcessing::exportAllAsImages() const
{
    QStringList ids = m_store.getIDs();
    QStringList urls;
    for (QString id : ids)
    {
        urls << m_store.getImageURL(id);
    }
    return urls;
}

bool ImageProcessing::isDocument(const QString &id) const
{
    return m_store.isExtractedDoc(id);
}

void ImageProcessing::setDefaultConfig(const ExtractorConfig &conf)
{
	//TODO
	//~ qDebug() << "ImageProcessing::setDefaultConfig() conf.colorThr = " << conf.colorThr;
	qDebug() << "ImageProcessing::setDefaultConfig() conf.colorThr = ";
}

void ImageProcessing::getDefaultConfig(ExtractorConfig &conf) const
{
	//~ conf.colorThr = 20;
	qDebug() << "ImageProcessing::getDefaultConfig() conf.colorThr = 20";
	//TODO
}

void ImageProcessing::setConfig(const bool colorMode, const bool filterMode, const int colorThr, const float colorGain, const float colorBias)
{
  qDebug() << "colorMode: " << colorMode << "filterMode: " << filterMode << "colorThr: " << colorThr << "colorGain: " << colorGain << "colorBias: " << colorBias;
  //TODO Die Werte müssen jetzt gespeichert oder für die Bildverarbeitungs weiter gegeben werden
}

/*void ImageProcessing::setConfig(const QString &id, const ExtractorConfig &conf)
{
	//~ qDebug() << "ImageProcessing::setConfig() conf.colorThr = " << conf.colorThr;
	qDebug() << "ImageProcessing::setConfig() conf.colorThr = ";
	//TODO
}*/

void ImageProcessing::getConfig(const QString &id, ExtractorConfig &conf) const
{
	//~ conf.colorThr = 10;
	qDebug() << "Image Processing: conf.colorThr = 10";
	//TODO
}
