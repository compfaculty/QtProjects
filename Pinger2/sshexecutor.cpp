#include "sshexecutor.h"
#include <QObject>
#include <QEventLoop>
#include <QProcess>
#include <QScopedPointer>
#include <cstdio>
using namespace std;

SSHExecutor::SSHExecutor(QObject *parent) : QObject(parent)
{

}

QString SSHExecutor::RunCmd(const QString& host, const QString& passwd, const QString& login, const QString& cmd)
{
    QString result;
    QEventLoop looper;
    QProcess* p = new QProcess(&looper);
    p->setProcessChannelMode(QProcess::MergedChannels);
//    QObject::connect(p, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), &looper, &QEventLoop::quit);
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),  &looper, &QEventLoop::quit);
    connect(p, &QProcess::errorOccurred,[&result]()->void{qDebug("Error in Process"); result.clear();});
    connect(p, &QProcess::errorOccurred,&looper,&QEventLoop::quit);
//  connect(p, &QProcess::started,[p,&passwd]()->void{qDebug("started");p->write((passwd + '\n').toLatin1());p->write((passwd + '\n').toLatin1());p->write((passwd + '\n').toLatin1());});
    connect(p, &QProcess::readyReadStandardOutput,[p,&result]()->void{
        qDebug("readyReadStandardOutput");
        result+=p->readAllStandardOutput();
    });
//    const QString c = QString("sh -c \"" + cmd + "\" ");

    QString conStr = QString("ssh %1@%2 %3").arg(login, host, cmd);
    p->start(conStr);
    looper.exec();
    p->deleteLater();
    return result;
}
