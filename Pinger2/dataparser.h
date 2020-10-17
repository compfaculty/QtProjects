#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QCommandLineParser>
#include <QCommandLineOption>

class DataParser: public QObject
{
    QCommandLineParser parser;
    Q_OBJECT
public:
    DataParser(const QCoreApplication &app);
    ~DataParser();
    QList<QUrl> parseCmd();

signals:
   void dataReady(bool);
private:
    QList<QUrl> results;
    QStringList appArgs;
};

#endif // DATAPARSER_H
