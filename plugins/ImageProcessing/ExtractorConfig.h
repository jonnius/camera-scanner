#ifndef EXTRACTORCONFIG_H
#define EXTRACTORCONFIG_H

#include <QObject>

#include "ONSExtractor.h"

class ExtractorConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool colorMode READ colorMode WRITE setColorMode NOTIFY colorModeChanged)
    Q_PROPERTY(bool filterMode READ filterMode WRITE setFilterMode NOTIFY filterModeChanged)
    Q_PROPERTY(int colorThr READ colorThr WRITE setColorThr NOTIFY colorThrChanged)
    Q_PROPERTY(float colorGain READ colorGain WRITE setColorGain NOTIFY colorGainChanged)
    Q_PROPERTY(float colorBias READ colorBias WRITE setColorBias NOTIFY colorBiasChanged)

public:
	ExtractorConfig(QObject *parent = 0);
    
    Q_INVOKABLE void setColorMode(const bool colorMode);
    bool colorMode() const;
    
    Q_INVOKABLE void setFilterMode(const bool filterMode);
    bool filterMode() const;
    
    Q_INVOKABLE void setColorThr(const int colorThr);
    int colorThr() const;
    
    Q_INVOKABLE void setColorGain(const float colorGain);
    float colorGain() const;
    
    Q_INVOKABLE void setColorBias(const float colorBias);
    float colorBias() const;
    
    Q_INVOKABLE void loadDefault();
    
signals:
    void colorModeChanged(bool colorMode);
    void filterModeChanged(bool filterMode);
    void colorThrChanged(int colorThr);
    void colorGainChanged(float colorGain);
    void colorBiasChanged(float colorBias);
    
private:
    void resetValues();
    void save();
    void load();
    void emitAll();

    bool m_colorMode;
    bool m_filterMode;
    int m_colorThr;
    float m_colorGain;
    float m_colorBias;
};


#endif
