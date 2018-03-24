#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>

#include "CDocument.h"

class ImageProcessing: public QObject
{
    Q_OBJECT

public:
    ImageProcessing();
    ~ImageProcessing() = default;

    Q_INVOKABLE void speak();

public slots:
    void processImage(const QImage & image);

private:
  DocumentScanner::CDocument m_document;
};

#endif
