#include <QCoreApplication>
#include <QObject>
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
#include <QProcess>
#include <cstdio>
#include "dataparser.h"
#include "httpclient.h"
#include "sshexecutor.h"
QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

using namespace std;

// Smart pointer to log file
QScopedPointer<QFile>   m_logFile;
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // SETUP APP
    QCoreApplication::setApplicationName("Pinger");
    QCoreApplication::setApplicationVersion("0.1");
//    SSHExecutor session;
//    QString ret = session.RunCmd("vd-alexm","Welcome3#","alexm", "whoami");
//    fprintf(stdout, "%s\n", qPrintable(ret));

    DataParser parser(a);
    HTTPClient client;

    //START LOGGING
    // Set the logging file, check which a path to file you use
    m_logFile.reset(new QFile(QString("%1.log").arg(a.applicationName())));
    // Open the file logging
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Set handler
    qInstallMessageHandler(messageHandler);
    qInfo() << "START CHEKING...";
    for(const QUrl &url : parser.parseCmd()){
       client.makeRequest(url);
    }
    return a.exec();
}


// The implementation of the handler
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Open stream file writes
//    QTextStream out(m_logFile.data());
    QTextStream out(stdout);
    // Write the date of recording
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // By type determine to what level belongs message
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Write to the output category of the message and the message itself
//    out << context.category << ": "  << msg << endl;
    out << msg << endl;
    out.flush();    // Clear the buffered data
}
