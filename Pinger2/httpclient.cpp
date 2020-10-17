#include "httpclient.h"
#include <QtCore>
#include <QtNetwork>
#include <QNetworkSession>
#include <cstdio>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

using namespace std;

HTTPClient::HTTPClient(bool save, QObject *parent) :
    QObject(parent)
{
    saveFlag = save;
    connect(&session, SIGNAL(finished(QNetworkReply*)), SLOT(onRequestFinished(QNetworkReply*)));
//#ifndef QT_NO_SSL
//    connect(&session, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
//#endif

}

HTTPClient::~HTTPClient()
{

}

void HTTPClient::makeRequest(const QUrl &url)
{
    QNetworkRequest req(url);
    QNetworkReply *reply = session.get(req);
    qInfo() << "Checking: " << url.url() << endl;
#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif
    currentReplies.append(reply);

}

void HTTPClient::onRequestFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    QString urlName = url.url();
    if (reply->error()) {
//        fprintf(stderr, "%s failed: %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
        qWarning() <<  urlName << "ERROR" << reply->errorString() << endl;

    } else {
        if (isHttpRedirect(reply)) {
//            fputs("Request was redirected.\n", stderr);
            qInfo() << urlName << ": REDIRECTED: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << endl;
        } else {
            if (saveFlag && saveReplyToDisk(url, reply)){
                qDebug() << "Content was saved" << urlName << endl;
            }
            qInfo() << urlName << ":" << "OK" << endl;
        }
    }

    currentReplies.removeAll(reply);
    reply->deleteLater();
}

void HTTPClient::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

bool HTTPClient::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
            || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

bool HTTPClient::saveReplyToDisk(const QUrl &url, QIODevice *data)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();
    if (basename.isEmpty())
        basename = "download";
    if (QFile::exists(basename)){
        basename = QString("%s.%s").arg(basename, QString::number(QDateTime::currentSecsSinceEpoch()));
    }
    QFile fd(basename);
    if(!fd.open(QIODevice::WriteOnly)){
        fprintf(stderr, "Could not open %s for writing: %s\n", qPrintable(basename), qPrintable(fd.errorString()));
        return false;
    }
    fd.write(data->readAll());
    fd.close();
    return true;
}
