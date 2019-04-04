#include "DocumentStore.h"

#include <chrono>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>

#include "ONSExtractor.h"

using namespace DocumentScanner;
using namespace cv;


inline
QImage convertMat2QImage(const Mat &img)
{
    if (!img.data)
    {
        qDebug() << "Error: Tried to convert an empty Mat to QImage!";
        return QImage();
        //TODO throw exception
    }
    if (img.type() != CV_8UC3)
    {
        qDebug() << "Error: Tried to convert an Mat other than CV_8UC3 to QImage!";
        return QImage();
        //TODO throw exception
    }
    Mat rgb(img.size(), CV_8UC3);
    cvtColor(img, rgb, COLOR_BGR2RGB);
    return QImage((uchar*) rgb.data, rgb.cols, rgb.rows,
                  rgb.step, QImage::Format_RGB888).copy();
}

inline
QString getTimeStampNow()
{
    return QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch()).count());
}

inline
QString joinPath(QString d1, QString d2)
{
    return QFileInfo(QDir(d1), d2).absoluteFilePath();
}

inline
QString getDocumentBaseDir()
{
    QString path = joinPath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation), "docs");
    QDir doc(path);
    if (!doc.exists() && !doc.mkpath("."))
    {
        qDebug() << "Failed to create document base directory " << path;
    }
    return path;
}

inline
QString getDocumentDir(const QString &id)
{
    QString path = joinPath(getDocumentBaseDir(), id);
    QDir doc(path);
    if (!doc.exists() && !doc.mkpath("."))
    {
        qDebug() << "Failed to create document directory " << path;
    }
    return path;
}

inline
QStringList getIDsFromCache()
{
    QDir base(getDocumentBaseDir());
    base.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    return base.entryList();
}

inline
QString getRawImagePath(const QString &id)
{
    return joinPath(getDocumentDir(id), "raw.jpg");
}

inline
QString getDocImagePath(const QString &id)
{
    return joinPath(getDocumentDir(id), "doc.jpg");
}

inline
QString URL2Path(const QString &URL)
{
    if (URL.startsWith("file://"))
    {
        return QUrl(URL).toLocalFile();
    }
    return URL;
}

inline
QString path2URL(const QString &path)
{
    return QUrl::fromLocalFile(path).toString();
}

inline
bool loadImage(const QString &imageURL, Mat &img)
{
    QString imagePath = URL2Path(imageURL);
    img = imread(imagePath.toStdString());
    return img.data;
}

inline
Document loadDocument(const QString &imageURL, const QString &docURL)
{
    Mat img, doc;
    bool imgFound = loadImage(imageURL, img);
    bool docFound = loadImage(docURL, doc);

    if (!imgFound)
    {
        qDebug() << "raw image does not exist or is invalid: " << imageURL;
        // TODO throw exception
    }
    if (!docFound)
    {
        //~ qDebug() << "No cached extracted document " << docURL;
    }

    return Document(img, doc, docFound);
}

inline
Document createDocument(const QString &imageURL, const AbstractDocumentExtractor &extractor)
{
    Mat img;
    bool imgFound = loadImage(imageURL, img);

    if (!imgFound)
    {
        qDebug() << "raw image does not exist or is invalid: " << imageURL;
        // TODO throw exception
    }

    return Document(img, extractor);
}

QString DocumentStore::addDocument(const QString &url, QString id)
{
    bool newDoc = id.isEmpty();
    if (newDoc)
        id = getTimeStampNow();

    if (!m_documents.count(id))
    {
        m_documents.insert(std::pair<QString,Document>(id,
                           newDoc ? createDocument(url, extractor)
                           : loadDocument(url, getDocImagePath(id))));
        return id;
    }
    else
    {
        qDebug() << "Incredible! A document with the exact same timestamp already existed! We met a document from the future with id " << id;
        return QString();
    }
}

void DocumentStore::cacheDocument(const QString &id) const
{
    if (m_documents.count(id))
    {
        const Document &d = m_documents.at(id);
        QString rawPath = getRawImagePath(id);
        QString docPath = getDocImagePath(id);
        QFile(docPath).remove();

        try
        {
            if (!QFile(rawPath).exists())
                imwrite(rawPath.toStdString(), d.getRawImage());
            if (d.docExtracted())
                imwrite(docPath.toStdString(), d.getDocImage());
        }
        catch (cv::Exception& e)
        {
            qDebug() << "Exception caching image with id " << id << "\n" << e.what();
        }
    }
    else
    {
        qDebug() << "Tried to access document with invalid id " << id;
    }
}


void DocumentStore::removeDocument(const QString &id)
{
    if (m_documents.count(id))
    {
        if (!QDir(getDocumentDir(id)).removeRecursively())
        {
            qDebug() << "Zombie alert! Failed to remove document dir from cache: " << id;
        }
        m_documents.erase(id);
    }
    else
    {
        qDebug() << "Tried to access document with invalid id " << id;
    }
}

QStringList DocumentStore::getIDs() const
{
    QStringList ids;
    ids.reserve(m_documents.size());
    for (auto const& idmap : m_documents)
        ids.push_back(idmap.first);
    return ids;
}

QStringList DocumentStore::restoreCache()
{
    qDebug() << "Restoring cache";
    QStringList ids = getIDsFromCache();
    for (QString id:ids)
        addDocument(getRawImagePath(id), id);
    return ids;
}


QImage DocumentStore::requestImage(const QString &id, QSize *size,
                                   const QSize &requestedSize)
{
    QImage img;
    //~ qDebug() << "image requested with id " << id;

    if (m_documents.count(id))
    {
        img = convertMat2QImage(m_documents.at(id).getDocImage());
        QSize sizeOrig = img.size();

        if (size)
            *size = sizeOrig;

        QSize newSize(requestedSize.width() > 0 ? requestedSize.width() : sizeOrig.width(),
                      requestedSize.height() > 0 ? requestedSize.height() : sizeOrig.height());
        img = img.scaled(newSize, Qt::KeepAspectRatio);
    }
    else
    {
        qDebug() << "Tried to access document with invalid id " << id;
        if (size)
            *size = QSize(0, 0);
    }
    return img;
}

QString DocumentStore::getImageURL(const QString &id) const
{
    if (m_documents.count(id))
    {
        const Document &d = m_documents.at(id);
        QString imagePath = d.docExtracted() ? getDocImagePath(id)
                            : getRawImagePath(id);
        return path2URL(imagePath);
    }
    else
    {
        qDebug() << "Tried to access document with invalid id " << id;
        return "";
    }
}

QString DocumentStore::exportPdf(const QStringList &ids) const
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/export.pdf";
    qDebug() << "Writing pdf to " << path;

    // TODO make divider configurable
    // reduces the default resolution of 1200 DPI by the divider.
    double resDivider = 4.0;

    QPdfWriter pdfwriter(path);
    QPainter painter(&pdfwriter);
    pdfwriter.setPageSize(QPagedPaintDevice::A4);
    painter.scale(resDivider, resDivider);

    bool firstPage = true;
    for (QString id : ids)
    {
        const Document &d = m_documents.at(id);
        if (!d.docExtracted())
            continue;

        if (firstPage)
            firstPage = false;
        else
            pdfwriter.newPage();

        QSize sizeA4 = pdfwriter.pageLayout().pageSize().sizePixels(
                           pdfwriter.resolution()/resDivider);
        QImage imageA4 = convertMat2QImage(d.getDocImage());
        imageA4 = imageA4.scaled(sizeA4);
        painter.drawImage(0, 0, imageA4);
    }

    if (firstPage)
        qDebug() << "Saved an empty PDF";

    painter.end();
    return path;
}

bool DocumentStore::isExtractedDoc(const QString &id) const
{
    if (m_documents.count(id))
    {
        return m_documents.at(id).docExtracted();
    }
    else
    {
        qDebug() << "Tried to access document with invalid id " << id;
    }
}
