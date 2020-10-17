#include "dataparser.h"
#include <QCoreApplication>
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

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

DataParser::DataParser(const QCoreApplication &app)
{
    appArgs = app.arguments();

}

DataParser::~DataParser()
{

}

QList<QUrl> DataParser::parseCmd()
{
    parser.setApplicationDescription("Check hosts alive");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption scumsDB(QStringList() << "f" << "path", QCoreApplication::translate("main", "path to file with urls"),
                               QCoreApplication::translate("main", "path"));
    parser.addOption(scumsDB);
    parser.process(appArgs);
    if ( parser.isSet(scumsDB)){
        qInfo() << "The target file: " << parser.value(scumsDB);
        QString dbPath = parser.value(scumsDB);
        QDir pwd(QDir::currentPath());
//        qDebug() << pwd.absoluteFilePath(dbPath);
        QFile db(pwd.absoluteFilePath(dbPath));
        QFile db2("c:\\Users\\meliha\\scums.txt");

        if (!db2.open(QIODevice::ReadOnly)){
            fprintf(stderr, "Could not open %s for writing: %s\n", qPrintable(dbPath),  qPrintable(db.errorString()));
        }

        QTextStream qts(&db2);
        while (!qts.atEnd()) {
            QUrl url(qts.readLine().trimmed());
            if (url.isValid())
                results.append(url);
        }
    }
    //assuming all positional argumenta are host
    else {
        parser.positionalArguments().removeDuplicates();
        const QStringList hosts = parser.positionalArguments();
        for(const QString &host: hosts){

            if (host.startsWith("http")){
                QUrl url(host);
                if (url.isValid())
                    results.append(url);
            }
            else
                qWarning() << "The adsress " << host << " has wrong format" << endl;
        }
    }

    return results;
}
