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

#include "compressor.h"

Compressor::Compressor(Setting *setting, QObject *parent):
    BaseFile(setting, parent)
{

}

void Compressor::start()
{
    emit writeLog(QString::fromLocal8Bit("┏━━━━━━━━━━ (`·Д·)シ 开始压缩图像替换 ━━━━━━━━━━┓"),LogType::OKLOG);
    removeBack();
    originDirs.clear();
    originDirs=loadDirs(setting->ORIGIN_PATH);
    oldFilesDirs.clear();
    newFilesDirs.clear();
    int compressCnts = 0;
    int compressSizes = 0;
    for (int i = 0; i < originDirs.length();
         i++, emit setDirBarValue(100 * i / originDirs.length())){
        QVector<int> compressInfo = subimageCompression(originDirs.at(i));
        compressCnts += compressInfo.at(0);
        compressSizes += compressInfo.at(1);
    }
    emit writeLog(QString::fromLocal8Bit("┗━━━━━━━━━━ ( `·Д·)ゞ %1 / %2 压缩成功，减少 %3 MB ━━━━━━━━━━┛")
                  .arg(compressCnts).arg(originDirs.length()).arg(compressSizes / 1024),
                  LogType::OKLOG);
}

void Compressor::revoke()
{
    emit writeLog(QString::fromLocal8Bit("┏━━━━━━━━━━ (‘_ゝ`) 撤销压缩图像替换 ━━━━━━━━━━┓"),LogType::OKLOG);
    int compressCnts = 0;

    for (int i = 0; i < oldFilesDirs.length();
         i++, emit setDirBarValue(100 * i / oldFilesDirs.length())){
        QString dirName = QFileInfo(oldFilesDirs.at(i).at(0)).absolutePath().section("/",-1);
        QDir backupsTargeDir = backupsDir.filePath(dirName);
        int compressCnt = 0;
        for (int j = 0 ; j < oldFilesDirs.at(i).length();
             j++, emit setFileBarValue(100 * j / oldFilesDirs.at(i).length())){
            QThread::msleep(setting->PROCESS_DELAY / 10);

            QFileInfo originFile(oldFilesDirs.at(i).at(j));
            QFileInfo targetFile(newFilesDirs.at(i).at(j));
            emit showMessage(originFile.absoluteFilePath().section("/", -2, -1));
            try {
                QFile::rename(originFile.absoluteDir().filePath(targetFile.fileName()), targetFile.absoluteFilePath());
                QFile::rename(backupsTargeDir.filePath(originFile.fileName()), originFile.absoluteFilePath());
                qDebug() << QString::fromLocal8Bit("开始下方");
                qDebug() << originFile.absoluteDir().filePath(targetFile.fileName())
                         << "----->" << targetFile.absoluteFilePath();
                qDebug() << backupsTargeDir.filePath(originFile.fileName())
                         << "----->" << originFile.absoluteFilePath();
                compressCnt++;
            } catch (std::exception &e) {
                qDebug() << e.what();
                qDebug() << "rename filed:" << originFile.absoluteDir().filePath(originFile.fileName());
            }
        }
        compressCnts += compressCnt;
        emit writeLog(QString::fromLocal8Bit("%3\n           ┗━ %1 / %2 撤销成功")
                      .arg(compressCnt).arg(oldFilesDirs.at(i).length()).arg(dirName),
                      LogType::REPLACELOG,
                      QFileInfo(oldFilesDirs.at(i).at(0)).absolutePath());
    }
    emit writeLog(QString::fromLocal8Bit("┗━━━━━━━━━━ (‘_ゝ`) %1 / %2 撤销成功 ━━━━━━━━━━┛")
                  .arg(compressCnts).arg(oldFilesDirs.length()),
                  LogType::OKLOG);
    removeBack();
}

QVector<int> Compressor::subimageCompression(const QString &path)
{
    QStringList oldFiles;
    QStringList newFiles;
    QStringList targetNameMods;

    QString dirName = path.section("/", -1);
    QDir backupsTargeDir = backupsDir.filePath(dirName);
    backupsTargeDir.mkdir(".");

    QStringList files = loadFiles(path);
    int compressCnt = 0;
    int warningCnt = 0;
    int compressSize = 0;
    for (int j = 0 ; j < files.length();
         j++, emit setFileBarValue(100 * j / files.length())){
        QThread::msleep(setting->PROCESS_DELAY / 10);

        QFileInfo originFile(files.at(j));
        emit showMessage(originFile.absoluteFilePath().section("/", -2, -1));
        QString targeDir = QDir(setting->TARGET_PATH).filePath(path.section("/", -1));
        QFileInfo targetFile(QDir(targeDir).filePath(originFile.completeBaseName()) + ".jpg");

        if (!targetFile.exists()) {
            warningCnt++;
            qDebug() << "Do not exists:" << targetFile.absoluteFilePath();
            continue;
        }

        qDebug() << originFile.size() / 1024 << "----->" << originFile.absoluteFilePath();
        qDebug() << targetFile.size() / 1024 << "----->" << targetFile.absoluteFilePath();
        if (100 * (originFile.size() - targetFile.size()) / originFile.size() > setting->SIZE_THRESHOLD) {
            try {
                QFile::rename(originFile.absoluteFilePath(), backupsTargeDir.filePath(originFile.fileName()));
                QFile::rename(targetFile.absoluteFilePath(), originFile.absoluteDir().filePath(targetFile.fileName()));
                compressCnt++;
                compressSize += (originFile.size() - targetFile.size()) / 1024;
                oldFiles.append(originFile.absoluteFilePath());
                newFiles.append(targetFile.absoluteFilePath());
                qDebug() << "V compress " << (originFile.size()-targetFile.size()) / 1024;
            } catch (std::exception &e) {
                qDebug() << e.what();
                qDebug() << "rename filed:" << originFile.absoluteFilePath();
                emit writeLog(QString("%1\n%2")
                              .arg(originFile.absoluteFilePath()).arg(e.what()),
                              LogType::IGNORELOG,
                              originFile.absoluteFilePath());
            }
        } else {
            qDebug() <<"X compress ";
        }
    }
    if (compressCnt > 0 || warningCnt > 0) {
        QString log(QFileInfo(path).fileName());
        int logType = 5;
        if (compressCnt > 0){
            oldFilesDirs.append(oldFiles);
            newFilesDirs.append(newFiles);
            log += QString::fromLocal8Bit("\n           ┗━ %1 / %2 压缩成功，减少 %3 MB")
                    .arg(compressCnt).arg(files.length())
                    .arg(compressSize / 1024);
            logType = LogType::REPLACELOG;
        }
        if (warningCnt > 0) {
            log.replace(QString::fromLocal8Bit("┗━"),QString::fromLocal8Bit("┣━"));
            log += QString::fromLocal8Bit("\n           ┗━ %1 / %2 文件不存在")
                    .arg(warningCnt).arg(files.length());
            logType = LogType::WARNINGLOG;
        }
        emit writeLog(log,logType,QFileInfo(path).absoluteFilePath());
    } else {
        emit writeLog(QString::fromLocal8Bit("无需修改\n%1")
                      .arg(QFileInfo(path).fileName()),
                      LogType::IGNORELOG,
                      QFileInfo(path).absoluteFilePath());
    }
    return QVector<int>{compressCnt,compressSize};
}
