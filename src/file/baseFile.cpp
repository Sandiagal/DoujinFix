#include "baseFile.h"



BaseFile::BaseFile(Setting *setting, QObject *parent) : QObject(parent),setting(setting)
{
    removeBack();
}

const QStringList BaseFile::loadDirs(const QString & path)
{
    QStringList dirs;
    QDirIterator it(path, QDir::Dirs|QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        dirs.append(it.next());
        //qDebug() << dirs.back();
    }
    return dirs;
}

const QStringList BaseFile::loadFiles(const QString &path)
{
    QStringList files;
    QDirIterator it(path, QStringList() << "*.bmp"<< "*.jpg"<< "*.fpeg"<< "*.png",
                    QDir::Files, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        files.append(it.next());
        //        qDebug() << files.back();
    }
    return files;
}

void BaseFile::removeBack()
{
    backupsDir = QDir(QDir::currentPath()).filePath("backups");
    backupsDir.removeRecursively();
    backupsDir.mkdir(".");
}

void BaseFile::run(int SR)
{
    if(!algLock.tryLock()) return;
    if (SR==0) {
        start();
        emit workDone(SR);
    }else {
        revoke();
        emit workDone(SR);
    }
    algLock.unlock();
}


void BaseFile::start()
{

}

void BaseFile::revoke()
{

}
