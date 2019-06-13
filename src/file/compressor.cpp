#include "compressor.h"

Compressor::Compressor(Setting *setting, QObject *parent):
    BaseFile(setting, parent)
{

}

void Compressor::start()
{
    removeBack();
    originDirs.clear();
    originDirs=loadDirs(setting->ORIGIN_PATH);
    oldFilesDirs.clear();
    newFilesDirs.clear();
    int compressCnts=0;
    int compressSizes=0;
    for(int i=0;i<originDirs.length();
        i++,emit setDirBarValue(100*i/originDirs.length())){
        QThread::msleep(setting->PROCESS_DELAY);
        QVector<int> compressInfo = subimageCompression(originDirs.at(i));
        compressCnts+=compressInfo.at(0);
        compressSizes+=compressInfo.at(1);
    }
    emit writeLog(QString::fromLocal8Bit("┗━━━━━━━━━━ ( `·Д·)ゞ %1 / %2 压缩成功,节约体积 %3 MB ━━━━━━━━━━┛")
                  .arg(compressCnts).arg(originDirs.length()).arg(compressSizes/1024),
                  LogType::OKLOG);
}

void Compressor::revoke()
{
    int compressCnts=0;

    for(int i=0;i<oldFilesDirs.length();
        i++,emit setDirBarValue(100*i/oldFilesDirs.length())){
        QThread::msleep(setting->PROCESS_DELAY);
        QString dirName = QFileInfo(oldFilesDirs.at(i).at(0)).absolutePath().section("/",-1);
        QDir backupsTargeDir = backupsDir.filePath(dirName);
        int compressCnt=0;
        for(int j=0;j<oldFilesDirs.at(i).length();
            j++,emit setFileBarValue(100*j/oldFilesDirs.at(i).length())){
//            QThread::msleep(setting->PROCESS_DELAY);

            QFileInfo originFile(oldFilesDirs.at(i).at(j));
            QFileInfo targetFile(newFilesDirs.at(i).at(j));
            emit showMessage(originFile.absoluteFilePath().section("/",-2,-1));
            try {
//                QFile::rename(originFile.absoluteDir().filePath(originFile.fileName()), targetFile.absoluteFilePath());
//                QFile::rename(backupsTargeDir.filePath(originFile.fileName()), originFile.absoluteFilePath());
                qDebug() << QString::fromLocal8Bit("开始下方");
                qDebug() << originFile.absoluteDir().filePath(targetFile.fileName())<<"----->"<<targetFile.absoluteFilePath();
                qDebug() << backupsTargeDir.filePath(originFile.fileName())<<"----->"<<originFile.absoluteFilePath();
                compressCnt++;
            } catch (std::exception &e) {
                qDebug()<<e.what();
                qDebug()<<"rename filed:"<<originFile.absoluteDir().filePath(originFile.fileName());
            }
        }
        compressCnts+=compressCnt;
        emit writeLog(QString::fromLocal8Bit("%1 / %2 撤销成功\n%3")
                      .arg(compressCnt).arg(oldFilesDirs.at(i).length()).arg(dirName),
                      LogType::NORMALLOG);
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

    QStringList files=loadFiles(path);
    int compressCnt=0;
    int compressSize=0;
    for(int j=0;j<files.length();
        j++,emit setFileBarValue(100*j/files.length())){
//        QThread::msleep(setting->PROCESS_DELAY);

        QFileInfo originFile(files.at(j));
        emit showMessage(originFile.absoluteFilePath().section("/",-2,-1));
        QString targeDir = QDir(setting->TARGET_PATH).filePath(path.section("/",-1));
        QFileInfo targetFile(QDir(targeDir).filePath(originFile.completeBaseName())+".jpg");
        qDebug() << originFile.size()/1024<<"----->"<<originFile.absoluteFilePath();
        qDebug() << targetFile.size()/1024<<"----->"<<targetFile.absoluteFilePath();
        if (100*(originFile.size()-targetFile.size())/originFile.size()>setting->SIZE_THRESHOLD) {
            try {
//                QFile::rename(originFile.absoluteFilePath(), backupsTargeDir.filePath(originFile.fileName()));
//                QFile::rename(targetFile.absoluteFilePath(), originFile.absoluteDir().filePath(originFile.fileName()));
                compressCnt++;
                compressSize+=(originFile.size()-targetFile.size())/1024;
                oldFiles.append(originFile.absoluteFilePath());
                newFiles.append(targetFile.absoluteFilePath());
                qDebug() <<"V compress "<<(originFile.size()-targetFile.size())/1024;
            } catch (std::exception &e) {
                qDebug()<<e.what();
                qDebug()<<"rename filed:"<<originFile.absoluteFilePath();
            }
        }else {
            qDebug() <<"X compress ";
        }
    }
    if (oldFiles.length()>0) {
        oldFilesDirs.append(oldFiles);
        newFilesDirs.append(newFiles);
        emit writeLog(QString::fromLocal8Bit("%1 / %2 压缩成功,节约体积 %3 MB\n%4")
                      .arg(compressCnt).arg(files.length())
                      .arg(compressSize/1024).arg(QFileInfo(path).fileName()),
                      LogType::NORMALLOG);
    }else {
        emit writeLog(QString::fromLocal8Bit("无需修改\n%1")
                      .arg(QFileInfo(path).fileName()),
                      LogType::IGNORELOG);
    }
    return QVector<int>{compressCnt,compressSize};
}
