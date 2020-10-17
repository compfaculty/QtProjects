#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class HTTPClient: public QObject
{
    QNetworkAccessManager session;
    QVector<QNetworkReply *> currentReplies;
    Q_OBJECT
public:
    HTTPClient(QList<QUrl> urls, bool save = false, QObject *parent = nullptr);
    ~HTTPClient();
    void run();
    void makeRequest(const QUrl &url);
    static bool isHttpRedirect(QNetworkReply *reply);

public slots:
    void onRequestFinished(QNetworkReply *);
    void sslErrors(const QList<QSslError> &errors);
private:
    QList<QUrl> urls;
    bool  saveFlag;
    bool saveReplyToDisk(const QUrl &url, QIODevice *data);
    void errorHandler(const QNetworkReply *);
    void successHandler(const QNetworkReply *);
    void redirectHandler(const QNetworkReply *);
};

#endif // HTTPCLIENT_H
