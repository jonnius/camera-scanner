#include "DocumentStore.h"

#include <chrono>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>

using namespace DocumentScanner;
using namespace cv;

QImage convertMat2QImage(Mat img)
{
	Mat rgb(img.size(), CV_8UC3);
	cvtColor(img, rgb, COLOR_BGR2RGB);
    return QImage((uchar*) rgb.data, rgb.cols, rgb.rows,
                  rgb.step, QImage::Format_RGB888).copy();
}

QString getTimeStampNow()
{
    return QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch()).count());
}

QString joinPath(QString d1, QString d2)
{
    return QFileInfo(QDir(d1), d2).absoluteFilePath();
}

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

QStringList getIDsFromCache()
{
    QDir base(getDocumentBaseDir());
    base.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    return base.entryList();
}

QString getRawImagePath(const QString &id)
{
    return joinPath(getDocumentDir(id), "raw.jpg");
}

QString getDocImagePath(const QString &id)
{
    return joinPath(getDocumentDir(id), "doc.jpg");
}

QString URL2Path(const QString &URL)
{
    if (URL.startsWith("file://"))
    {
        return URL.right(URL.size()-7);
    }
    return URL;
}

Document createDocument(const QString &imageURL, const QString &docURL = "")
{
    QString imagePath = URL2Path(imageURL);

    // Image is converted to 8bit RGB by default
    Mat img = imread(imagePath.toStdString());
    if (!img.data)
    {
        qDebug() << "raw image does not exist or is invalid: " << imagePath;
    }
    
    Document d(img);
    
    if (!docURL.isEmpty())
    {
		// Restore processed document
		// TODO check if exists
		QString docPath = URL2Path(docURL);
		
		Mat doc = imread(docPath.toStdString());
		if (!doc.data)
		{
			qDebug() << "doc image does not exist or is invalid: " << docPath;
		}
		else
		{
			d.restoreDocument(doc);
		}
	}
    
    return d;
}

QString DocumentStore::addDocument(const QString &url, QString id)
{
	QString docPath = "";
	
    if (id.isEmpty())
        id = getTimeStampNow();
	else
		docPath = getDocImagePath(id);

	Document d = createDocument(url, docPath);

    if (!m_documents.count(id))
    {
        m_documents.insert(std::pair<QString,Document>(id,d));
        return id;
    }
    else
    {
        qDebug() << "Incredible! A document with the exact same timestamp already existed! We met a document from the future with id " << id;
        return QString();
    }
}

void DocumentStore::cacheDocument(const QString &id)
{
    if (m_documents.count(id))
    {
        Document d = m_documents.at(id);
        QString rawPath = getRawImagePath(id);
        QString docPath = getDocImagePath(id);
        QFile(docPath).remove();

        try
        {
            if (!QFile(rawPath).exists())
                imwrite(rawPath.toStdString(), d.getRawImage());
            if (d.docDetected())
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

Document& DocumentStore::accessDocument(const QString &id)
{
    if (m_documents.count(id))
    {
        return m_documents.at(id);
    }
    else
    {
        qDebug() << "Tried to access document with invalid id " << id;
        return m_dummy;
    }
}

QStringList DocumentStore::getIDs()
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
    qDebug() << "image requested with id " << id;

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

QString DocumentStore::exportPdf(const QStringList &ids)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/test.pdf";
    qDebug() << "Writing pdf to " << path;
    
    QPdfWriter pdfwriter(path);
    QPainter painter(&pdfwriter);
    pdfwriter.setPageSize(QPagedPaintDevice::A4);

	bool firstPage = true;
    for (QString id : ids)
    {
		Document d = m_documents.at(id);
        if (!d.docDetected())
			continue;
		
		if (firstPage)
			firstPage = false;
		else
            pdfwriter.newPage();

        //~ painter.drawPixmap(QRect(0,0,pdfwriter.logicalDpiX()*8.3,pdfwriter.logicalDpiY()*11.7),
                           //~ QPixmap((imgurl.at(var)).toString()));
        painter.drawImage(0, 0, convertMat2QImage(d.getDocImage()));
    }
    
    if (firstPage)
		qDebug() << "Saved an empty PDF";

    painter.end();
    return path;
}
