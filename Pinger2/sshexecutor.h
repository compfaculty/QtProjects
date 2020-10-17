#ifndef SSHEXECUTOR_H
#define SSHEXECUTOR_H

#include <QObject>

class SSHExecutor : public QObject
{
    Q_OBJECT
public:
    explicit SSHExecutor(QObject *parent = nullptr);
    QString RunCmd(const QString &host, const QString &passwd, const QString &login, const QString &cmd);

signals:
    void answerReady(QStringList *);

};

#endif // SSHEXECUTOR_H
