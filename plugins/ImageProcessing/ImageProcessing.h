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
  /** Process the specified image */
  Q_INVOKABLE void processImage(const QString &id);
  /** Remove the specified image and delete it from cache */
  Q_INVOKABLE void removeImage(const QString &id);
  /** Remove all images and clear the cache */
  Q_INVOKABLE void removeAll();

  /** Return true if a document has been found in the specified image */
  Q_INVOKABLE bool isDocument(const QString &id);

  /** Set an image processing parameter */  
  Q_INVOKABLE void setParam(const QString &key, const QString &value);
  /** Get an image processing parameter */  
  Q_INVOKABLE QString getParam(const QString &key);  

signals:
  void imageAdded(const QString &id);
  void imageProcessed(const QString &id, const bool &success);
  void imageRemoved(const QString &id);

private:
  DocumentScanner::DocumentStore &m_store;
  std::map<QString,QVariant> m_params;
};

#endif
