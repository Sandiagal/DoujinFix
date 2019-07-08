/*
 * The Doujinfix – a Qt-based batch arrangement software for Doujinshi that
 * includes Doujinshi file name standardization and compressed image replacement.
 * Copyright (C) 2019 Sandiagal
 *
 * This program is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * You can contact us at sandiagal2525@gmail.com
*/

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

    QStringList originDirs;

protected:
    QDir backupsDir;
    QVector<QStringList> oldFilesDirs;
    QVector<QStringList> newFilesDirs;

    const QStringList loadDirs(const QString &path);
    const QStringList loadFiles(const QString &path);
    void removeBack();

private:
    QMutex algLock;

signals:
    void writeLog(const QString &content, int type=1, const QString &copy="");
    void showMessage(const QString &message, int timeout=3000);
    void workDone(int SR);

};

#endif // BASEFILE_H
