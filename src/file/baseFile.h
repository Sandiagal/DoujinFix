#ifndef BASEFILE_H
#define BASEFILE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QMutex>
#include "setting/setting.h"

class BaseFile : public QObject
{
    Q_OBJECT
public:
    explicit BaseFile(Setting *setting, QObject *parent = nullptr);
    Setting *setting;

    void run(int SR);
    virtual void start();
    virtual void revoke();

protected:
    QDir backupsDir;
    QStringList originDirs;
    QVector<QStringList> oldFilesDirs;
    QVector<QStringList> newFilesDirs;

    const QStringList loadDirs(const QString &path);
    const QStringList loadFiles(const QString &path);
    void removeBack();

private:
    QMutex algLock;

signals:
    void writeLog(const QString &content, int type);
    void showMessage(const QString &message, int timeout=3000);
    void workDone(int SR);

};

#endif // BASEFILE_H
