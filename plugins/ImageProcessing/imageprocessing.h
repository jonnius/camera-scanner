#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>

class ImageProcessing: public QObject 
{
    Q_OBJECT

public:
    ImageProcessing();
    ~ImageProcessing() = default;

    Q_INVOKABLE void speak();
    
    
};

#endif
