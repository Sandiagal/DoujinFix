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

#include "standardiser.h"

#include <QImageReader>
#include <QPixmap>


Standardiser::Standardiser(Setting *setting, QObject *parent):
    BaseFile(setting,parent)
{
    patterns = QStringList{
            "\\(.+\\)\\[.+\\(.+\\)\\].*\\(.+\\)\\[.+\\]",
            "\\[.+\\]\\(.+\\)\\[.+\\(.+\\)\\].*\\(.+\\)",
            "\\[.+\\(.+\\)\\].*\\(.+\\)\\[.+\\]",
            "\\(.+\\)\\[.+\\(.+\\)\\].*\\[.+\\]",
            "\\(.+\\)\\[.+\\].*\\(.+\\)\\[.+\\]",
            "\\[.+\\]\\[.+\\(.+\\)\\].*\\(.+\\)",
            "\\[.+\\]\\(.+\\)\\[.+\\(.+\\)\\].*",
            "\\[.+\\]\\(.+\\)\\[.+\\].*\\(.+\\)",
            "\\(.+\\)\\[.+\\(.+\\)\\].*\\(.+\\)",
            "\\[.+\\(.+\\)\\].*\\[.+\\]",
            "\\[.+\\].*\\(.+\\)\\[.+\\]",
            "\\(.+\\)\\[.+\\].*\\[.+\\]",
            "\\[.+\\]\\[.+\\(.+\\)\\].*",
            "\\[.+\\]\\(.+\\)\\[.+\\].*",
            "\\[.+\\]\\[.+\\].*\\(.+\\)",
            "\\[.+\\(.+\\)\\].*\\(.+\\)",
            "\\(.+\\)\\[.+\\(.+\\)\\].*",
            "\\(.+\\)\\[.+\\].*\\(.+\\)",
            "\\[.+\\].+\\[.+\\]",
            "\\[.+\\]\\[.+\\].*",
            "\\[.+\\(.+\\)\\].*",
            "\\(.+\\)\\[.+\\].*",
            "\\[.+\\].*\\(.+\\)",
            "\\(.+\\).*\\(.+\\)",
            "\\[.+\\].*",
            ".+\\(.+\\)",
            "\\(.+\\).*",
            ".+"};
}

void Standardiser::start()
{
    emit writeLog(QString::fromLocal8Bit("┏━━━━━━━━━━ (`·Д·)シ 开始名称标准化 ━━━━━━━━━━┓"),LogType::OKLOG);
    originDirs.clear();
    newDirs.clear();
    int successDir = 0;
    
    if (setting->INDEX_FIX) {
        oldFilesDirs.clear();
        newFilesDirs.clear();
    }
    int successFileCnt = 0;
    int successDirFileCnt = 0;
    
    QStringList dirs = loadDirs(setting->ORIGIN_PATH);
    for (int i = 0; i < dirs.length();
         i++, emit setProgressBarValue(100 * i / dirs.length())) {
        QThread::msleep(setting->PROCESS_DELAY);
        if (!setting->LABEL_FILTER) filtedLabels.clear();

        QFileInfo dirInfo(dirs.at(i));
        
        QString standardResult = dirStandard(dirInfo);
        if (standardResult != dirInfo.absoluteFilePath())
            successDir += 1;
        
        if (setting->INDEX_FIX) {
            QStringList files = loadFiles(standardResult);
            int successCnt = fileStandard(files);
            successFileCnt += successCnt;
            if(successCnt > 0)
                successDirFileCnt += 1;
        }
    }
    
    QString log = QString::fromLocal8Bit("┗━━━━━━━━━━ ( `·Д·)ゞ %1 / %2 名称标准化成功 ━━━━━━━━━━┛")
            .arg(successDir).arg(dirs.length());
    if (setting->INDEX_FIX) {
        log += QString::fromLocal8Bit("\n%3┗━━━━━  %1 / %2 序号标准化成功 ━━━━━┛")
                .arg(successFileCnt).arg(successDirFileCnt).arg(QString().fill(' '), 27);
    }
    emit writeLog(log,LogType::OKLOG);
}

void Standardiser::revoke()
{
    emit writeLog(QString::fromLocal8Bit("┏━━━━━━━━━━ (‘_ゝ`) 撤销名称标准化 ━━━━━━━━━━┓"),LogType::OKLOG);
    int dirCnt = 0;
    int successDirCnt = 0;
    int fileCnt = 0;
    QString filelog;
    int successFileCnts = 0;
    int successDirFileCnt = 0;
    for (int i = 0; i < originDirs.length();
         i++, emit setProgressBarValue(100 * i / originDirs.length())){
        QThread::msleep(setting->PROCESS_DELAY);
        
        if (setting->INDEX_FIX){
            int successFileCnt = 0;
            filelog = "";
            for (int j = 0; j < oldFilesDirs.at(i).length(); j++) {
                if(newFilesDirs.at(i).at(j).size() > 0){
                    fileCnt++;
                    QString originFile(oldFilesDirs.at(i).at(j));
                    QString targetFile(newFilesDirs.at(i).at(j));
                    emit showMessage(targetFile.section("/", -2, -1));
                    try {
                        qDebug() << targetFile << "----->" << originFile;
                        QFile::rename(targetFile, originFile);
                        successFileCnt++;
                    } catch (std::exception &e) {
                        qDebug() << e.what();
                        qDebug() << "index stand filed:" << targetFile;
                    }
                }
            }
            if (successFileCnt > 0){
                successDirFileCnt += 1;
                successFileCnts += successFileCnt;
                filelog = QString::fromLocal8Bit("%3\n           ┗━ %1 / %2 序号撤销成功")
                        .arg(successFileCnt).arg(fileCnt).arg(QFileInfo(originDirs[i]).fileName());
            }
        }
        
        if(newDirs[i].size() > 0){
            dirCnt++;
            emit showMessage(newDirs[i].section("/", -1));
            try {
                QFile::rename(newDirs[i], originDirs[i]);
                successDirCnt++;
                emit writeLog(QString::fromLocal8Bit("撤销成功\n%1     ---------->\n%2")
                              .arg(QFileInfo(newDirs[i]).fileName())
                              .arg(QFileInfo(originDirs[i]).fileName()),
                              LogType::NAMELOG,
                              QFileInfo(originDirs[i]).absoluteFilePath());
            } catch (std::exception &e) {
                qDebug() << e.what();
                qDebug() << "rename filed:"<<QFileInfo(newDirs[i]).fileName();
                emit writeLog(QString::fromLocal8Bit("撤销失败\n%1")
                              .arg(QFileInfo(newDirs[i]).fileName()),
                              LogType::ERRORLOG,
                              QFileInfo(originDirs[i]).absoluteFilePath());
            }
        }
        
        if (setting->INDEX_FIX && filelog.size() > 0){
            emit writeLog(filelog, LogType::INDEXLOG, QFileInfo(originDirs[i]).absoluteFilePath());
        }
        
    }
    QString log = QString::fromLocal8Bit("┗━━━━━━━━━━ (‘_ゝ`) %1 / %2  命名标准化撤销成功 ━━━━━━━━━━┛")
            .arg(successDirCnt).arg(dirCnt);
    if (setting->INDEX_FIX)
        log += QString::fromLocal8Bit("\n%3┗━━━━━ %1 / %2 序号撤销成功 ━━━━━┛")
                .arg(successFileCnts).arg(successDirFileCnt).arg(QString().fill(' '), 33);
    emit writeLog(log,LogType::OKLOG);
}

QString Standardiser::dirStandard(QFileInfo &dirInfo)
{
    emit showMessage(dirInfo.fileName());
    
    originDirs.append(dirInfo.absoluteFilePath());
    QString name = dirInfo.fileName();
    stringReplacement(name);
    
    QString special;
    if(setting->UNKNOWN_TRANSLATOR) special.append(haveTranslators(name));
    if(setting->LOW_QUALITY) special.append(ifLowQuality(name, dirInfo.absoluteFilePath()));
    
    nameAnalysis(name);
    if(!ifvalid(name)){
        newDirs.append("");
        emit writeLog(QString::fromLocal8Bit("命名不合法\n%1")
                      .arg(dirInfo.fileName()),
                      LogType::ERRORLOG,
                      dirInfo.absoluteFilePath());
        return dirInfo.absoluteFilePath();
    }
    
    QStringList component(readInfo(name));
    if (component.length() == 0 || component.back().contains("]")) {
        newDirs.append("");
        emit writeLog(QString::fromLocal8Bit("命名真奇葩\n%1")
                      .arg(dirInfo.fileName()),
                      LogType::ERRORLOG,
                      dirInfo.absoluteFilePath());
        return dirInfo.absoluteFilePath();
    }
    if (!component.back().isEmpty() && special.contains(QString::fromLocal8Bit("★"))) {
        special.replace(QString::fromLocal8Bit("★"),"");
    }
    component.push_front(special);
    
    QString result = join(component);
    qDebug() << "Special :" << component[0];
    qDebug() << "Convention :" << component[1];
    qDebug() << "Circle :" << component[2];
    qDebug() << "Artist :" << component[3];
    qDebug() << "Title :" << component[4];
    qDebug() << "Parody :" << component[5];
    qDebug() << "Translators :" << component[6];
    qDebug() << "Labels :" << filtedLabels;

    stringReplacement2(result);
    
    qDebug() << "Standardization result:";
    qDebug() << result;
    
    QString resultPath = QDir(dirInfo.absolutePath()).filePath(result);
    
    if (resultPath == dirInfo.absoluteFilePath()) {
        newDirs.append("");
        emit writeLog(QString::fromLocal8Bit("无需修改\n%1")
                      .arg(result),
                      LogType::IGNORELOG,
                      dirInfo.absoluteFilePath());
        return dirInfo.absoluteFilePath();
    } else {
        try {
            QFile::rename(dirInfo.absoluteFilePath(), resultPath);
            newDirs.append(resultPath);
            qDebug() << dirInfo.absoluteFilePath() << "----->" << resultPath;
            emit writeLog(QString::fromLocal8Bit("标准化成功\n%1     ---------->\n%2")
                          .arg(dirInfo.fileName()).arg(result),
                          LogType::NAMELOG,
                          resultPath);
            return resultPath;
        } catch (std::exception &e) {
            newDirs.append("");
            qDebug() << e.what();
            qDebug() << "rename filed";
            emit writeLog(QString::fromLocal8Bit("重命名失败\n")
                          .arg(dirInfo.fileName()),
                          LogType::ERRORLOG,
                          dirInfo.absoluteFilePath());
            return dirInfo.absoluteFilePath();
        }
    }
}

int Standardiser::fileStandard(const QStringList &files)
{
    int compressCnt = 0;
    int warningCnt = 0;
    QStringList oldFiles;
    QStringList newFiles;
    QString lastName;
    for(int i = 0; i < files.length(); i++){
        
        QFileInfo originFile(files[i]);
        emit showMessage(originFile.absoluteFilePath().section("/", -2, -1));
        
        QString targetBaseName(originFile.baseName());
        if (targetBaseName.contains("__ver")) {
            int lastIndex = lastName.section("_", -1).toInt();
            targetBaseName = lastName.section("_", 0, 2) + "_" + QString::number(lastIndex + 1) ;
        }
        targetBaseName.replace("(", "");
        targetBaseName.replace(")", "");
        targetBaseName.replace(QRegExp("^[pP](age)?"), "");
        targetBaseName.replace(QRegExp("^[iI]mage"), "");
        targetBaseName.replace(QRegExp("^(img|IMG)"), "");
        targetBaseName.replace(QRegExp("^[sS]can"), "");
        targetBaseName.replace(QRegExp("^JPEG"), "");
        targetBaseName.replace(QRegExp("^Xeros"), "");
        targetBaseName.replace(QRegExp("^Girls"), "");
        targetBaseName.replace(QRegExp("^_+"), "");
        targetBaseName.replace(QRegExp("^0_+"), "");
        targetBaseName.replace(QString::fromLocal8Bit("副本"), "");
        targetBaseName.replace(QString::fromLocal8Bit("拷贝"), "");
        targetBaseName.replace(QRegExp("[\\sv_z]+$"), "");
        
        targetBaseName.replace(QRegExp("^(\\d+)(_|-)\\d+$"), "\\1");
        targetBaseName.replace(QRegExp("^(\\d+)(_|-)?(img|IMG)(_|-)?\\d+$"), "\\1");
        
        targetBaseName.replace(QRegExp("^(\\df?)$"), "00\\1");
        targetBaseName.replace(QRegExp("^(\\d{2}f?)$"), "0\\1");
        targetBaseName.replace(QRegExp("0{3,}(\\df?)$"), "00\\1");
        targetBaseName.replace(QRegExp("0{2,}(\\d{2}f?)$"), "0\\1");
        targetBaseName.replace(QRegExp("0{1,}(\\d{3}f?)$"), "\\1");
        targetBaseName.replace(QRegExp("^\\d*_CE"), "CE");
        targetBaseName.replace(QRegExp("^\\d*_MJK"), "MJK");
        
        if (!targetBaseName.contains(QRegExp("^\\d{3}f?$")) &&
                !targetBaseName.contains("CE") &&
                !targetBaseName.contains("MJK") &&
                !targetBaseName.contains("staff") &&
                !targetBaseName.contains("zCREDIT") &&
                !targetBaseName.contains(QString::fromLocal8Bit("脸肿汉化组招募")) &&
                !targetBaseName.contains(QString::fromLocal8Bit("脸肿字幕组招募")) &&
                !targetBaseName.contains(QString::fromLocal8Bit("臉腫粉絲群二维码")) &&
                !targetBaseName.contains(QString::fromLocal8Bit("微博图"))) {
            warningCnt++;
        }
        
        lastName = targetBaseName;
        targetBaseName += "."+originFile.suffix().toLower();
        QString targetFilePath(originFile.absoluteDir().filePath(targetBaseName));
        
        if (targetFilePath != originFile.absoluteFilePath()) {
            qDebug() << originFile.absoluteFilePath() << "----->" << targetFilePath;
            try {
                QFile::rename(originFile.absoluteFilePath(), targetFilePath);
                compressCnt++;
                newFiles.append(targetFilePath);
            } catch (std::exception &e) {
                newFiles.append("");
                qDebug() << e.what();
                qDebug() << "rename filed";
            }
            oldFiles.append(originFile.absoluteFilePath());
        }
    }
    
    if (oldFiles.length() > 0 || warningCnt > 0) {
        QString log;
        int logType = 4;
        if (oldFiles.length() > 0){
            log += QString::fromLocal8Bit("┗━ %1 / %2 序号修改成功")
                    .arg(compressCnt).arg(files.length());
            logType = LogType::INDEXLOG;
        }
        if (warningCnt > 0) {
            if (!log.isEmpty()){
                log.replace(QString::fromLocal8Bit("┗━"),QString::fromLocal8Bit("┣━"));
                log+="\n           ";
            }
            log += QString::fromLocal8Bit("┗━ %1 / %2 序号非标准")
                    .arg(warningCnt).arg(files.length());
            logType = LogType::WARNINGLOG;
        }
        emit writeLog(log, logType,QFileInfo(files[0]).absolutePath());
    }
    
    oldFilesDirs.append(oldFiles);
    newFilesDirs.append(newFiles);
    return compressCnt;
}

QStringList Standardiser::readInfo(const QString &name)
{
    QStringList result;
    QRegExp rx("^(\\(.*\\))(\\[.*)(\\(.*\\)\\])(.*)(\\(.*\\))(\\[.*\\])$");
    if (rx.exactMatch(name)) {
        result.append(rx.cap(1).remove(0, 1).chopped(1));
        result.append(rx.cap(2).remove(0, 1));
        result.append(rx.cap(3).remove(0, 1).chopped(2));
        result.append(rx.cap(4));
        result.append(rx.cap(5).remove(0, 1).chopped(1));
        result.append(rx.cap(6).remove(0, 1).chopped(1));
        
        if (setting->PARODY_MAP && !setting->NAME_MAP->empty()) {
            parodyMap(result[4]);
        }
        
        result[1].replace(QString::fromLocal8Bit("、"), ", ");
        result[2].replace(QString::fromLocal8Bit("、"), ", ");
    }
    
    return result;
}

double Standardiser::stringSimilar(const QString &strA, const QString &strB)
{
    int lenA = strA.length();
    int lenB = strB.length();
    QVector<QVector<int>> d(lenA+1, QVector<int>(lenB + 1, 0));
    for (int i = 0; i <= lenA; ++i) d[i][0] = i;
    for (int i = 0; i <= lenB; ++i) d[0][i] = i;
    for (int i = 1; i <= lenA; ++i)
        for (int j = 1; j <= lenB; ++j)
            d[i][j] = qMin(qMin(d[i - 1][j - 1] + (strA[i - 1] == strB[j - 1] ? 0 : 1),
                    d[i - 1][j] + 1), d[i][j - 1] + 1);
    return 1 - d[lenA][lenB] * 1.0 / qMax(lenA, lenB);
}

void Standardiser::parodyMap(QString &parody)
{
    if (parody.length() == 0) return;

    QVector<double> similarities;
    QMapIterator<QString, QString> i(*(setting->NAME_MAP));
    while (i.hasNext()) {
        i.next();
        similarities.append(stringSimilar(parody,i.key()));
        //            qDebug() << similarities.back() << " : " << result[4] << " and "<<i.key();
    }
    
    int maxIndex = std::max_element(similarities.begin(), similarities.end()) - similarities.begin();
    double maxsimilarity = similarities.at(maxIndex);
    qDebug() << QString("%1 O: %2 S: %3 T: %4")
                .arg(maxsimilarity).arg(parody)
                .arg((setting->NAME_MAP->begin() + maxIndex).key())
                .arg((setting->NAME_MAP->begin() + maxIndex).value());
    if (maxsimilarity > 0.5) {
        QString aim = (setting->NAME_MAP->begin() + maxIndex).value();
        if(aim == "Final Fantasy" || aim == "Dragon Quest") {
            parody.replace((setting->NAME_MAP->begin() + maxIndex).key(), aim + " ");
        } else {
            parody = aim;
        }
    }
}

QString &Standardiser::nameAnalysis(QString &name)
{
    for (int i = 0; i < patterns.length(); i++){
        if (QRegExp(patterns[i]).exactMatch(name)) {
            int format = 0;
            // 根据匹配的格式，添加缺失的格式符号
            switch (i) {
            case 0:
                format = 1;
                break;
            case 1:
                format = 2;
                break;
            case 2:
                name.insert(name.indexOf("["), "()");
                format = 1;
                break;
            case 3:
                name.insert(name.lastIndexOf("["), "()");
                format = 1;
                break;
            case 4:
                name.insert(name.indexOf("]"), "()");
                format = 1;
                break;
            case 5:
                name.insert(name.indexOf("]") + 1, "()");
                format = 2;
                break;
            case 6:
                name.append("()");
                format = 2;
                break;
            case 7:
                name.insert(name.indexOf("]", name.indexOf("]") + 1), "()");
                format = 2;
                break;
            case 8:
                format = 3;
                break;
            case 9:
                name.insert(name.indexOf("["), "()");
                name.insert(name.lastIndexOf("["), "()");
                format = 1;
                break;
            case 10:
                name.insert(name.indexOf("["), "()");
                name.insert(name.indexOf("]"), "()");
                format = 1;
                break;
            case 11:
                name.insert(name.indexOf("]"), "()");
                name.insert(name.lastIndexOf("["), "()");
                format = 1;
                break;
            case 12:
                name.insert(name.indexOf("]") + 1, "()");
                name.append("()");
                format = 2;
                break;
            case 13:
                name.insert(name.indexOf("]", name.indexOf("]") + 1), "()");
                name.append("()");
                format = 2;
                break;
            case 14:
                name.insert(name.indexOf("]") + 1, "()");
                name.insert(name.indexOf("]",name.indexOf("]") + 1), "()");
                format = 2;
                break;
            case 15:
                name.insert(name.lastIndexOf("["), "()");
                format = 3;
                break;
            case 16:
                name.append("()");
                format = 3;
                break;
            case 17:
                name.insert(name.indexOf("]"), "()");
                format = 3;
                break;
            case 18:
                name.insert(name.indexOf("["), "()");
                name.insert(name.indexOf("]"), "()");
                name.insert(name.lastIndexOf("["), "()");
                format = 1;
                break;
            case 19:
                name.insert(name.indexOf("]") + 1, "()");
                name.insert(name.indexOf("]", name.indexOf("]") + 1), "()");
                name.append("()");
                format = 2;
                break;
            case 20:
                name.insert(name.lastIndexOf("["), "()");
                name.append("()");
                format = 3;
                break;
            case 21:
                name.insert(name.indexOf("]"), "()");
                name.append("()");
                format = 3;
                break;
            case 22:
                name.insert(name.lastIndexOf("["), "()");
                name.insert(name.indexOf("]"), "()");
                format = 3;
                break;
            case 23:
                format = 4;
                break;
            case 24:
                name.insert(name.lastIndexOf("["), "()");
                name.insert(name.indexOf("]"), "()");
                name.append("()");
                format = 3;
                break;
            case 25:
                name.push_front("()");
                format = 4;
                break;
            case 26:
                name.append("()");
                format = 4;
                break;
            case 27:
                name.push_front("()");
                name.append("()");
                format = 4;
                break;
            default:
                break;
            }
            qDebug() << "Regular expression 1:";
            qDebug() << QString("PatternArray %1 format: %2 : %3")
                        .arg(i).arg(format).arg(name);
            // 进一步修改符号调整为标准格式
            if (format == 4) {
                name.insert(name.indexOf(")") + 1, "[()]");
                format = 3;
            }
            if (format == 3) {
                name.append("[]");
                format = 1;
            }
            if (format == 2) {
                QString temp = name.left(name.indexOf("]") + 1);
                name = name.mid(name.indexOf("]") + 1) + temp;
                format = 1;
            }
            qDebug() << "Regular expression 2:";
            qDebug() << name;
            break;
        }
    }
    return name;
}

QString &Standardiser::labelStorage(QString &name, QString label)
{
    if (name.contains(label)) {
        if (!setting->LABEL_FILTER) filtedLabels += " " + label;
        name.replace(label, "");
    }
    return name;
}

QString &Standardiser::labelStorage(QString &name, QRegExp rx)
{
    if (name.contains(rx)) {
        if (!setting->LABEL_FILTER) {
            rx.indexIn(name);
            filtedLabels.push_front(" " + rx.cap(0));
        }
        name.replace(rx, "");
    }
    return name;
}

QString &Standardiser::stringReplacement(QString &name)
{
    name.replace(QString::fromLocal8Bit("【"), "[");
    name.replace(QString::fromLocal8Bit("】"), "]");
    name.replace(QString::fromLocal8Bit("［"), "[");
    name.replace(QString::fromLocal8Bit("］"), "]");
    name.replace(QString::fromLocal8Bit("（"), "(");
    name.replace(QString::fromLocal8Bit("）"), ")");
    name.replace(QString::fromLocal8Bit("～"), "~");
    name.replace("  ", " ");
    name.replace("] ", "]");
    name.replace(" [", "[");
    name.replace(" (", "(");
    name.replace(") ", ")");
    name.replace(QString::fromLocal8Bit("！"), "!");
    name.replace(QString::fromLocal8Bit("(無修正)"), "");
    name.replace(QString::fromLocal8Bit("(オリジナル)"), "");
    name.replace(QString::fromLocal8Bit("(Original)"), "");
    name.replace("(Various)", QString::fromLocal8Bit("(よろず)"));
    name.replace(QString::fromLocal8Bit("無毒漢化組"), QString::fromLocal8Bit("无毒汉化组"));
    name.replace(QString::fromLocal8Bit("CE漢化組"), QString::fromLocal8Bit("CE家族社"));
    name.replace(QString::fromLocal8Bit("CE汉化组"), QString::fromLocal8Bit("CE家族社"));
    name.replace(QString::fromLocal8Bit("CE家族社汉化"), QString::fromLocal8Bit("CE家族社"));
    name.replace(QString::fromLocal8Bit("CE家族社漢化"), QString::fromLocal8Bit("CE家族社"));
    name.replace(QString::fromLocal8Bit("空気系★漢化"), QString::fromLocal8Bit("空気系☆漢化"));
    name.replace(QString::fromLocal8Bit("空気系☆汉化"), QString::fromLocal8Bit("空気系☆漢化"));
    name.replace(QString::fromLocal8Bit("空気系★汉化"), QString::fromLocal8Bit("空気系☆漢化"));
    name.replace(QString::fromLocal8Bit("空気系汉化"), QString::fromLocal8Bit("空気系☆漢化"));
    name.replace(QString::fromLocal8Bit("空気系漢化"), QString::fromLocal8Bit("空気系☆漢化"));

    labelStorage(name, QString::fromLocal8Bit("[中文]"));
    labelStorage(name, QString::fromLocal8Bit("[中国語]"));
    labelStorage(name, QString::fromLocal8Bit("[中國語]"));
    labelStorage(name, QString::fromLocal8Bit("[無修正]"));
    labelStorage(name, QString::fromLocal8Bit("[无修正]"));
    labelStorage(name, QString::fromLocal8Bit("[DL版]"));
    labelStorage(name, QString::fromLocal8Bit("[Digital]"));
    labelStorage(name, QString::fromLocal8Bit("[全彩]"));
    labelStorage(name, QRegExp("\\[MJK-\\d+-[A-Z]?\\d+\\]"));
    labelStorage(name, QRegExp("\\[CE_\\d+\\]"));

    name.replace("()", "");
    name.replace("[]", "");
    name = name.trimmed();
    qDebug() << "stringReplacement 1:";
    qDebug() << name;
    return name;
}

bool Standardiser::ifvalid(QString &name)
{
    int smallBlock = 0;
    int middleBlock = 0;
    for (QChar c : name) {
        if (c == '(') smallBlock++;
        if (c == ')') smallBlock--;
        if (c == '[') middleBlock++;
        if (c == ']') middleBlock--;
        if (smallBlock < 0 || 1 < smallBlock || middleBlock < 0 || 1 < middleBlock) return false;
    }
    return true;
}

QString Standardiser::haveTranslators(QString &name)
{
    bool noTranslators = false;
    if (name.contains(QString::fromLocal8Bit("[中国翻訳]"))) {
        if (!setting->LABEL_FILTER)
            filtedLabels.push_front(" " + QString::fromLocal8Bit("[中国翻訳]"));
        name.replace(QString::fromLocal8Bit("[中国翻訳]"), "");
        noTranslators = true;
    }
    if (name.contains("[Chinese]")) {
        if (!setting->LABEL_FILTER) filtedLabels.push_front(" [Chinese]");
        name.replace("[Chinese]", "");
        noTranslators = true;
    }
    int idx=name.section("[", 0, 0).indexOf(QString::fromLocal8Bit("★"));
    if (idx > -1){
        name.remove(idx, 1);
        return QString::fromLocal8Bit("★");
    }
    
    if (noTranslators == true) return QString::fromLocal8Bit("★");
    return "";
}

QString Standardiser::ifLowQuality(QString &name, QString path)
{
    int idx=name.section("[", 0, 0).indexOf(QString::fromLocal8Bit("▼"));
    if (idx > -1){
        name.remove(idx, 1);
        return QString::fromLocal8Bit("▼");
    }
    
    QStringList files = loadFiles(path);
    int fileCnt = 0;
    int lowCnt = 0;
    for (int i = 2; i < files.length() && i < 7; i++){
        emit showMessage(files.at(i).section("/", -2, -1));
        
        fileCnt++;
        QImageReader reader(files.at(i));
        QSize sizeOfImage = reader.size();
        if (sizeOfImage.width() < 1) return "";
        
        qDebug() << sizeOfImage.width() << "----->" << path;
        
        if (sizeOfImage.width() < setting->WIDTH_THRESHOLD) lowCnt++;
        if (fileCnt > 2 && fileCnt - lowCnt > 2) return "";
        if (fileCnt > 2 && lowCnt > 2)  return QString::fromLocal8Bit("▼");
    }
    return "";
}

QString Standardiser::join(QStringList &component)
{
    QString result;
    result += component[0] + " ";
    
    switch (setting->NAMING_STYLE) {
    case 0:
        result += "[" + component[2] + " (" + component[3] + ")] " + component[4] + " ("
                + component[5] + ") [" + component[6] + "]";
        break;
    case 1:
        result += "(" + component[1] + ") [" + component[2] + " (" + component[3] + ")] "
                + component[4] + " (" + component[5] + ") [" + component[6] + "]";
        break;
    case 2:
        result += "[" + component[6] + "] (" + component[1] + ") [" + component[2] + " ("
                + component[3] + ")] " + component[4] + " (" + component[5] + ")";
        break;
    default:
        break;
    }
    if (!setting->LABEL_FILTER) result += filtedLabels;
    return result;
}

QString &Standardiser::stringReplacement2(QString &name)
{
    name.replace("[ ()]", "");
    name.replace("[]", "");
    name.replace("()", "");
    name.replace(" ]", "]");
    name.replace("  ", " ");
    name = name.trimmed();
    return name;
}
