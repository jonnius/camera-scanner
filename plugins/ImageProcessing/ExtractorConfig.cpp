#include "ExtractorConfig.h"
#include <QDebug>

ExtractorConfig::ExtractorConfig(QObject *parent) : QObject(parent)
{
}

bool ExtractorConfig::colorMode() const
{
  return m_colorMode;
}

void ExtractorConfig::setColorMode(const bool colorMode)
{
  if(colorMode != m_colorMode) {
    m_colorMode = colorMode;
    emit colorModeChanged(m_colorMode);
  }
}

bool ExtractorConfig::filterMode() const
{
  return m_filterMode;
}

void ExtractorConfig::setFilterMode(const bool filterMode)
{
  if(filterMode != m_filterMode) {
    m_filterMode = filterMode;
    emit filterModeChanged(m_filterMode);
  }
}

int ExtractorConfig::colorThr() const
{
  return m_colorThr;
}

void ExtractorConfig::setColorThr(const int colorThr)
{
  if(colorThr != m_colorThr) {
    m_colorThr = colorThr;
    emit colorThrChanged(m_colorThr);
  }
}

float ExtractorConfig::colorGain() const
{
  return m_colorGain;
}

void ExtractorConfig::setColorGain(const float colorGain)
{
  if(colorGain != m_colorGain) {
    m_colorGain = colorGain;
    emit colorGainChanged(m_colorGain);
  }
}

float ExtractorConfig::colorBias() const
{
  return m_colorBias;
}

void ExtractorConfig::setColorBias(const float colorBias)
{
  if(colorBias != m_colorBias) {
    m_colorBias = colorBias;
    emit colorBiasChanged(m_colorBias);
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
