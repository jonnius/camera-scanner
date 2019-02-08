#ifndef DOCUMENTSTORE_H
#define DOCUMENTSTORE_H

#include <QQuickImageProvider>

#include "Document.h"
#include "ONSExtractor.h"

namespace DocumentScanner
{
class DocumentStore : public QQuickImageProvider
{
public:
    DocumentStore()
        : QQuickImageProvider(QQuickImageProvider::Image)
    {
        /* empty */
    }

    QImage requestImage(const QString &id, QSize *size,
                        const QSize &requestedSize) override;

    QString addDocument(const QString &url, QString id = "");
    void cacheDocument(const QString &id);
    void removeDocument(const QString &id);
    QStringList getIDs();
    QStringList restoreCache();
    QString exportPdf(const QStringList &ids);
    bool isExtractedDoc(const QString &id);
    QString getImageURL(const QString &id);

private:
    std::map<QString,Document> m_documents;
    ONSExtractor extractor;
};
}

#endif
