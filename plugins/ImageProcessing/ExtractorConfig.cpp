#include "ExtractorConfig.h"
#include <QDebug>

ExtractorConfig::ExtractorConfig(QObject *parent) : QObject(parent)
{
}

bool ExtractorConfig::colorMode() const
{
  return m_colorMode;
}

void ExtractorConfig::setColorMode(bool colorMode)
{
  if(colorMode != m_colorMode) {
    m_colorMode = colorMode;
    emit colorModeChanged(m_colorMode);
    qDebug() << "Selected color mode:" << m_colorMode;
  }
}

bool ExtractorConfig::filterMode() const
{
  return m_filterMode;
}

void ExtractorConfig::setFilterMode(bool filterMode)
{
  if(filterMode != m_filterMode) {
    m_filterMode = filterMode;
    emit filterModeChanged(m_filterMode);
    qDebug() << "Selected filter mode:" << m_filterMode;
  }
}

int ExtractorConfig::colorThr() const
{
  return m_colorThr;
}

void ExtractorConfig::setColorThr(int colorThr)
{
  if(colorThr != m_colorThr) {
    m_colorThr = colorThr;
    emit colorThrChanged(m_colorThr);
    qDebug() << "ColorThr value:" << m_colorThr;
  }
}

float ExtractorConfig::colorGain() const
{
  return m_colorGain;
}

void ExtractorConfig::setColorGain(float colorGain)
{
  if(colorGain != m_colorGain) {
    m_colorGain = colorGain;
    emit colorGainChanged(m_colorGain);
    qDebug() << "Color gain value:" << m_colorGain;
  }
}

float ExtractorConfig::colorBias() const
{
  return m_colorBias;
}

void ExtractorConfig::setColorBias(float colorBias)
{
  if(colorBias != m_colorBias) {
    m_colorBias = colorBias;
    emit colorBiasChanged(m_colorBias);
    qDebug() << "Color bias value:" << m_colorBias;
  }
}

void ExtractorConfig::loadDefault()
{
    qDebug() << "Load Default Settings";
    //Default Settings
    //~ bool colorMode = true;
    setColorMode(true);
    //~ bool filterMode = true;
    setFilterMode(true);
    //~ int colorThr = 110;
    setColorThr(110);
    //~ float colorGain = 1.5; // contrast
    setColorGain(1.5);
    //~ float colorBias = 0.0;   // bright
    setColorBias(0.0);
}
