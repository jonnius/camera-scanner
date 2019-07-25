#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include <QImage>

#include "DocumentStore.h"
#include "ExtractorConfig.h"

/**
  * Qt Object that acts as an interface to the Image Processing.
  */
class ImageProcessing: public QObject
{
    Q_OBJECT
    Q_PROPERTY(ExtractorConfig *config READ config WRITE setConfig NOTIFY configChanged)

public:
    ImageProcessing();
    ~ImageProcessing() = default;
    
    ExtractorConfig *config();
    void setConfig(ExtractorConfig *config);

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
    Q_INVOKABLE void setDefaultConfig(const ExtractorConfig &conf);
    /** Get an image processing parameter */
    Q_INVOKABLE void getDefaultConfig(ExtractorConfig &conf) const;
    /** Set an image processing parameter */
    //Q_INVOKABLE void setConfig(const QString &id, const ExtractorConfig &conf);
    void setConfig(const bool colorMode, const bool filterMode, const int colorThr, const float colorGain, const float colorBias);
    /** Get an image processing parameter */
    Q_INVOKABLE void getConfig(const QString &id, ExtractorConfig &conf) const;

signals:
    void imageAdded(const QString &id);
    void imageRemoved(const QString &id);
    void configChanged(ExtractorConfig *config);

private slots:
  void onConfigChanged();
    
private:
    DocumentScanner::DocumentStore &m_store;
    std::map<QString,QVariant> m_params;
    ExtractorConfig *m_config = nullptr;
};

#endif
