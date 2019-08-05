#include "ExtractorConfig.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

ExtractorConfig::ExtractorConfig(QObject *parent) : QObject(parent)
{
    resetValues();
    load();
}

bool ExtractorConfig::colorMode() const
{
    return m_colorMode;
}

void ExtractorConfig::setColorMode(const bool colorMode)
{
    if(colorMode != m_colorMode) {
        m_colorMode = colorMode;
        save();
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
        save();
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
        save();
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
        save();
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
        save();
        emit colorBiasChanged(m_colorBias);
    }
}

inline
QString joinPath(QString d1, QString d2)
{
    return QFileInfo(QDir(d1), d2).absoluteFilePath();
}

inline
QString getConfigPath()
{
    return joinPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation), "config.json");
}

void ExtractorConfig::save()
{
    qDebug() << "Saving settings to config file...";

    QJsonObject config;

    config.insert(QString("colorMode"), QJsonValue(m_colorMode));
    config.insert(QString("filterMode"), QJsonValue(m_filterMode));
    config.insert(QString("colorThr"), QJsonValue(m_colorThr));
    config.insert(QString("colorGain"), QJsonValue(m_colorGain));
    config.insert(QString("colorBias"), QJsonValue(m_colorBias));

    QJsonDocument d(config);

    QFile file;
    file.setFileName(getConfigPath());
    if (!file.open(QIODevice::WriteOnly)
            || file.write(d.toJson(QJsonDocument::Indented)) < 0)
    {
        qDebug() << "Failed to write to config file.";
        return;
    }
}

void ExtractorConfig::load()
{
    qDebug() << "Loading settings from config file...";
    QFile file;
    file.setFileName(getConfigPath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "No config file found. Setting default values...";
        return;
    }
    QString json = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());

    if (!d.isObject())
    {
        qDebug() << "ERROR: Invalid config file. Setting default values...";
        return;
    }

    QJsonObject config = d.object();

    QJsonValue colorMode = config.value(QString("colorMode"));
    QJsonValue filterMode = config.value(QString("filterMode"));
    QJsonValue colorThr = config.value(QString("colorThr"));
    QJsonValue colorGain = config.value(QString("colorGain"));
    QJsonValue colorBias = config.value(QString("colorBias"));

    setColorMode(colorMode.toBool(m_colorMode));
    setFilterMode(filterMode.toBool(m_filterMode));
    setColorThr(colorThr.toInt(m_colorThr));
    setColorGain(colorGain.toDouble(m_colorGain));
    setColorBias(colorBias.toDouble(m_colorBias));
}

void ExtractorConfig::loadDefault()
{
    resetValues();
    save();
    emitAll();
}

void ExtractorConfig::emitAll()
{
    emit colorModeChanged(m_colorBias);
    emit filterModeChanged(m_filterMode);
    emit colorThrChanged(m_colorThr);
    emit colorGainChanged(m_colorGain);
    emit colorBiasChanged(m_colorBias);
}

void ExtractorConfig::resetValues()
{
    qDebug() << "Reset Default Settings";
    m_colorMode = true;
    m_filterMode = true;
    m_colorThr = 110;
    m_colorGain = 1.5;
    m_colorBias = 0.0;
}
