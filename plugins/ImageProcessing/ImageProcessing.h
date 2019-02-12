#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include <QImage>

#include "DocumentStore.h"

/**
  * Qt Object that acts as an interface to the Image Processing.
  */
class ImageProcessing: public QObject
{
    Q_OBJECT

public:
    ImageProcessing(DocumentScanner::DocumentStore &d);
    ~ImageProcessing() = default;

    /** Load cached images from disk and add them */
    Q_INVOKABLE void restoreCache();
    /** Add an image and cache it on disk */
    Q_INVOKABLE void addImage(const QString &imageURL);
    /** Remove the specified image and delete it from cache */
    Q_INVOKABLE void removeImage(const QString &id);
    /** Remove all images and clear the cache */
    Q_INVOKABLE void removeAll();
    /** Export one image as PDF and return URL to PDF file */
    Q_INVOKABLE QString exportAsPdf(const QString &id) const;
    /** Export all images as PDF and return URL to PDF file */
    Q_INVOKABLE QString exportAllAsPdf() const;
    /** Export one image and return URL to image file */
    Q_INVOKABLE QString exportAsImage(const QString &id) const;
    /** Export all images and return list of URLs to image files */
    Q_INVOKABLE QStringList exportAllAsImages() const;

    /** Return true if a document has been found in the specified image */
    Q_INVOKABLE bool isDocument(const QString &id) const;

    /** Set an image processing parameter */
    Q_INVOKABLE void setParam(const QString &key, const QString &value);
    /** Get an image processing parameter */
    Q_INVOKABLE QString getParam(const QString &key) const;
    /** Get all available image processing parameters */
    Q_INVOKABLE QStringList getAvailableParams() const;

signals:
    void imageAdded(const QString &id);
    void imageRemoved(const QString &id);

private:
    DocumentScanner::DocumentStore &m_store;
    std::map<QString,QVariant> m_params;
};

#endif
