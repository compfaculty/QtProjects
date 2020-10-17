#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QtCore>
#include <QtNetwork>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include <cstdio>
#include "httpclient.h"
QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationVersion("0.1");
    QList<QUrl> urls;
    QCommandLineParser parser;
    parser.setApplicationDescription("My APP");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption scumdb(QStringList() << "f" << "path", QCoreApplication::translate("main", "path to file with urls"),
                              QCoreApplication::translate("main", "path"));
    parser.addOption(scumdb);
    parser.process(a.arguments());
    if(parser.isSet(scumdb)){
        QString path = parser.value(scumdb);
        QDir pwd(QDir::currentPath());
        QFile db(pwd.absoluteFilePath(path));
        if(!db.open(QIODevice::ReadOnly)){
             qCritical() << "Could not open" << qPrintable(db.fileName()) << qPrintable(db.errorString());
            a.exit(1);
        }
        QTextStream qts(&db);
        while (!qts.atEnd()) {
            QString tmpurl = qts.readLine().trimmed();
            if (!tmpurl.startsWith("http"))
                tmpurl = "https://" + tmpurl;
            QUrl url(tmpurl);
            if(url.isValid()){
                urls.append(url);
            }
            else{
                qDebug() << "Wrong format" << url;
            }
        }
    }else{
        parser.positionalArguments().removeDuplicates();
        for(const QString &host: parser.positionalArguments()){
            QUrl url("http://" + host);
                urls.append(url);
                }
            }
    HTTPClient client(urls);
    client.run();
    return a.exec();
}
