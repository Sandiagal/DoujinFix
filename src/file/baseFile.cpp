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

#include "baseFile.h"

BaseFile::BaseFile(Setting *setting, QObject *parent) : QObject(parent), setting(setting)
{
    removeBack();
}

const QStringList BaseFile::loadDirs(const QString & path)
{
    QStringList dirs;
    QDirIterator it(path, QDir::Dirs|QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) dirs.append(it.next());
    return dirs;
}

const QStringList BaseFile::loadFiles(const QString &path)
{
    QStringList files;
    QDirIterator it(path, QStringList() << "*.bmp"<< "*.jpg"<< "*.fpeg"<< "*.png",
                    QDir::Files, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) files.append(it.next());
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
    if (!algLock.tryLock()) return;
    if (SR == 0) {
        start();
        emit workDone(SR);
    } else {
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
