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

void ImageProcessing::setParam(const QString &key, const QString &value)
{
    qDebug() << "ImageProcessing::setParam() not yet implemented";
//TODO
}

QString ImageProcessing::getParam(const QString &key) const
{
    qDebug() << "ImageProcessing::getParam() not yet implemented";
//TODO
}

QStringList ImageProcessing::getAvailableParams() const
{
    qDebug() << "ImageProcessing::getAvailableParams() not yet implemented";
//TODO
}
