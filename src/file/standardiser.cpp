#include "standardiser.h"

#include <QImageReader>


Standardiser::Standardiser(Setting *setting, QObject *parent):
    BaseFile(setting,parent)
{
    patterns= new QStringList{
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
    originDirs.clear();
    newDirs.clear();
    int successDir=0;

    if (setting->INDEX_FIX) {
        oldFilesDirs.clear();
        newFilesDirs.clear();
    }
    int successFileCnt=0;
    int successDirFileCnt=0;

    QStringList dirs=loadDirs(setting->ORIGIN_PATH);
    for(int i=0;i<dirs.length();
        i++,emit setProgressBarValue(100*i/dirs.length())){
        QThread::msleep(setting->PROCESS_DELAY);

        QFileInfo dirInfo(dirs.at(i));

        QString standardResult = dirStandard(dirInfo);
        if(standardResult!=dirInfo.absoluteFilePath())
            successDir+=1;

        if (setting->INDEX_FIX) {
            QStringList files=loadFiles(dirInfo.absoluteFilePath());
            int successCnt=fileStandard(files);
            successFileCnt+=successCnt;
            if(successCnt>0)
                successDirFileCnt+=1;
        }
    }

    QString log=QString::fromLocal8Bit("┗━━━━━━━━━━ ( `·Д·)ゞ %1 / %2 命名标准化成功 ━━━━━━━━━━┛")
            .arg(successDir).arg(dirs.length());
    if (setting->INDEX_FIX) {
        log+=QString::fromLocal8Bit("\n%3 %1 / %2 序号标准化成功")
                .arg(successFileCnt).arg(successDirFileCnt).arg(QString().fill(' '),45);
    }
    emit writeLog(log,LogType::OKLOG);
}

void Standardiser::revoke()
{
    int dirCnt=0;
    int successDirCnt=0;
    int fileCnt=0;
    QString filelog;
    int successFileCnts=0;
    int successDirFileCnt=0;
    for (int i=0;i<originDirs.length();
         i++,emit setProgressBarValue(100*i/originDirs.length())){
        QThread::msleep(setting->PROCESS_DELAY);

        if (setting->INDEX_FIX){

            int successFileCnt=0;
            filelog="";
            for (int j=0;j<oldFilesDirs.at(i).length();j++) {

                if(newFilesDirs.at(i).at(j).size()>0){
                    fileCnt++;
                    QString originFile(oldFilesDirs.at(i).at(j));
                    QString targetFile(newFilesDirs.at(i).at(j));
                    emit showMessage(targetFile.section("/",-2,-1));
                    try {
                        qDebug()<< targetFile << "----->" << originFile;
                        //                    QFile::rename(targetFile, originFile);
                        successFileCnt++;
                    } catch (std::exception &e) {
                        qDebug()<<e.what();
                        qDebug()<<"index stand filed:" << targetFile;
                    }
                }
            }
            if(successFileCnt>0){
                successDirFileCnt+=1;
                successFileCnts+=successFileCnt;
                filelog = QString::fromLocal8Bit("%3\n           ┗━%1 / %2 序号撤销成功")
                        .arg(successFileCnt).arg(fileCnt).arg(QFileInfo(originDirs[i]).fileName());
            }
        }

        if(newDirs[i].size()>0){
            dirCnt++;
            emit showMessage(newDirs[i].section("/",-1));
            try {
                //            QFile::rename(newDirs[i], originDirs[i]);
                successDirCnt++;
                emit writeLog(QString::fromLocal8Bit("撤销成功\n%1     ---------->\n%2")
                              .arg(QFileInfo(newDirs[i]).fileName())
                              .arg(QFileInfo(originDirs[i]).fileName()),
                              LogType::NAMELOG);
            } catch (std::exception &e) {
                qDebug()<<e.what();
                qDebug()<<"rename filed:"<<QFileInfo(newDirs[i]).fileName();
                emit writeLog(QString::fromLocal8Bit("撤销失败\n%1")
                              .arg(QFileInfo(newDirs[i]).fileName()),
                              LogType::ERRORLOG);
            }
        }

        if (setting->INDEX_FIX&&filelog.size()>0){
            emit writeLog(filelog,LogType::INDEXLOG);
        }

    }
    QString log=QString::fromLocal8Bit("┗━━━━━━━━━━ (‘_ゝ`) %1 / %2  命名撤销成功 ━━━━━━━━━━┛")
            .arg(successDirCnt).arg(dirCnt);
    if (setting->INDEX_FIX)
        log+=QString::fromLocal8Bit("\n%3 %1 / %2 序号撤销成功")
                .arg(successFileCnts).arg(successDirFileCnt).arg(QString().fill(' '),45);
    emit writeLog(log,LogType::OKLOG);
}

QString Standardiser::dirStandard(QFileInfo &dirInfo)
{
    emit showMessage(dirInfo.fileName());

    originDirs.append(dirInfo.absoluteFilePath());
    QString name=dirInfo.fileName();
    stringReplacement(name);

    QString special;

    if(setting->UNKNOWN_TRANSLATOR)
        special.append(haveTranslators(name));

    if(setting->LOW_QUALITY)
        special.append(ifLowQuality(name, dirInfo.absoluteFilePath()));

    nameAnalysis(name);

    QStringList component;
    component.append(special);
    component.append(readInfo(name));
    if (component.length()==0) {
        newDirs.append("");
        emit writeLog(QString::fromLocal8Bit("命名真奇葩\n%1")
                      .arg(dirInfo.fileName()),
                      LogType::ERRORLOG);
        return dirInfo.absoluteFilePath();
    }

    QString result=join(component);
    qDebug() << "Special :"<<component[0];
    qDebug() << "Convention :"<<component[1];
    qDebug() << "Circle :"<<component[2];
    qDebug() << "Artist :"<<component[3];
    qDebug() << "Title :"<<component[4];
    qDebug() << "Parody :"<<component[5];
    qDebug() << "Translators :"<<component[6];

    stringReplacement2(result);

    qDebug() <<"Standardization result:";
    qDebug() << result;

    QString resultPath = QDir(dirInfo.absolutePath()).filePath(result);

    if (resultPath==dirInfo.absoluteFilePath()) {
        newDirs.append("");
        emit writeLog(QString::fromLocal8Bit("无需修改\n%1")
                      .arg(result),
                      LogType::IGNORELOG);
        return dirInfo.absoluteFilePath();
    }else {
        try {
            //            QFile::rename(dirInfo.absoluteFilePath(), resultPath);
            newDirs.append(resultPath);
            qDebug()<<dirInfo.absoluteFilePath()<<"----->"<<resultPath;
            emit writeLog(QString::fromLocal8Bit("标准化成功\n%1     ---------->\n%2")
                          .arg(dirInfo.fileName())
                          .arg(result), LogType::NAMELOG);
            return resultPath;
        } catch (std::exception &e) {
            newDirs.append("");
            qDebug()<<e.what();
            qDebug()<<"rename filed";
            emit writeLog(QString::fromLocal8Bit("重命名失败\n")
                          .arg(dirInfo.fileName()),
                          LogType::ERRORLOG);
            return dirInfo.absoluteFilePath();
        }
    }
    return dirInfo.absoluteFilePath();
}

int Standardiser::fileStandard(const QStringList &files)
{
    int compressCnt=0;
    QStringList oldFiles;
    QStringList newFiles;
    for(int i=0;i<files.length();i++){

        QFileInfo originFile(files.at(i));
        emit showMessage(originFile.absoluteFilePath().section("/",-2,-1));

        QString targetBaseName(originFile.baseName());
        targetBaseName.replace(QRegExp("^(\\d)$"), "00\\1");
        targetBaseName.replace(QRegExp("^(\\d{2})$"), "0\\1");
        targetBaseName.replace(QRegExp("0{3,}(\\d)$"), "00\\1");
        targetBaseName.replace(QRegExp("0{2,}(\\d{2})$"), "0\\1");
        targetBaseName.replace(QRegExp("^\\d*_CE"), "CE");
        targetBaseName.replace(QRegExp("^\\d*_MJK"), "MJK");
        targetBaseName+="."+originFile.completeSuffix();
        QString targetFilePath(originFile.absoluteDir().filePath(targetBaseName));

        if (targetFilePath!=originFile.absoluteFilePath()) {
            qDebug() << originFile.absoluteFilePath()<<"----->"<<targetFilePath;
            try {
                //                QFile::rename(originFile.absoluteFilePath(), targetFilePath);
                compressCnt++;
                newFiles.append(targetFilePath);
            } catch (std::exception &e) {
                newFiles.append("");
                qDebug()<<e.what();
                qDebug()<<"rename filed";
            }
            oldFiles.append(originFile.absoluteFilePath());
        }
    }
    if (oldFiles.length()>0) {
        emit writeLog(QString::fromLocal8Bit("┗━%1 / %2 序号标准化成功")
                      .arg(compressCnt).arg(files.length()),
                      LogType::INDEXLOG);
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
        result.append(rx.cap(1).remove(0,1).chopped(1));
        result.append(rx.cap(2).remove(0,1));
        result.append(rx.cap(3).remove(0,1).chopped(2));
        result.append(rx.cap(4));
        result.append(rx.cap(5).remove(0,1).chopped(1));
        result.append(rx.cap(6).remove(0,1).chopped(1));

        if (setting->PARODY_MAP) {
            parodyMap(result[4]);
        }

    }
    return result;
}

float Standardiser::stringSimilar(const QString &strA, const QString &strB)
{
    int lenA = strA.length();
    int lenB = strB.length();
    QVector<QVector<int>> d(lenA+1,QVector<int>(lenB+1,0));
    for(int i=0;i<=lenA;++i)
        d[i][0] = i;
    for(int i=0;i<=lenB;++i)
        d[0][i] = i;
    for(int i=1;i<=lenA;++i)
        for(int j=1;j<=lenB;++j)
            d[i][j] = qMin(qMin(d[i-1][j-1] + (strA[i-1] == strB[j-1] ? 0:1), d[i-1][j]+1), d[i][j-1]+1);
    return 1-d[lenA][lenB]*1.0/qMax(lenA,lenB);
}

void Standardiser::parodyMap(QString &parody)
{
    if (parody.length()==0) {
        return;
    }

    QVector<float> similarities;
    QMapIterator<QString, QString> i(*(setting->NAME_MAP));
    while (i.hasNext()) {
        i.next();
        similarities.append(stringSimilar(parody,i.key()));
        //            qDebug() << similarities.back() << " : " << result[4] << " and "<<i.key();
    }

    int maxIndex = std::max_element(similarities.begin(),similarities.end())-similarities.begin();
    float maxsimilarity = similarities.at(maxIndex);
    qDebug()<<QString("%1 O: %2 S: %3 T: %4")
              .arg(maxsimilarity).arg(parody).
              arg((setting->NAME_MAP->begin()+maxIndex).key()).
              arg((setting->NAME_MAP->begin()+maxIndex).value());
    if (maxsimilarity>0.5) {
        parody = (setting->NAME_MAP->begin()+maxIndex).value();
    }
}

QString &Standardiser::nameAnalysis(QString &name)
{
    for(int i=0;i<patterns->length();i++){
        if (QRegExp(patterns->at(i)).exactMatch(name)) {

            int format=0;
            // 根据匹配的格式，添加缺失的格式符号
            switch (i) {
            case 0:
                format = 1;
                break;
            case 1:
                format = 2;
                break;
            case 2:
                name.insert(name.indexOf("["),"()");
                format = 1;
                break;
            case 3:
                name.insert(name.lastIndexOf("["),"()");
                format = 1;
                break;
            case 4:
                name.insert(name.indexOf("]"),"()");
                format = 1;
                break;
            case 5:
                name.insert(name.indexOf("]")+1,"()");
                format = 2;
                break;
            case 6:
                name.append("()");
                format = 2;
                break;
            case 7:
                name.insert(name.indexOf("]",name.indexOf("]")+1),"()");
                format = 2;
                break;
            case 8:
                format = 3;
                break;
            case 9:
                name.insert(name.indexOf("["),"()");
                name.insert(name.lastIndexOf("["),"()");
                format = 1;
                break;
            case 10:
                name.insert(name.indexOf("["),"()");
                name.insert(name.indexOf("]"),"()");
                format = 1;
                break;
            case 11:
                name.insert(name.indexOf("]"),"()");
                name.insert(name.lastIndexOf("["),"()");
                format = 1;
                break;
            case 12:
                name.insert(name.indexOf("]")+1,"()");
                name.append("()");
                format = 2;
                break;
            case 13:
                name.insert(name.indexOf("]",name.indexOf("]")+1),"()");
                name.append("()");
                format = 2;
                break;
            case 14:
                name.insert(name.indexOf("]")+1,"()");
                name.insert(name.indexOf("]",name.indexOf("]")+1),"()");
                format = 2;
                break;
            case 15:
                name.insert(name.lastIndexOf("["),"()");
                format = 3;
                break;
            case 16:
                name.append("()");
                format = 3;
                break;
            case 17:
                name.insert(name.indexOf("]"),"()");
                format = 3;
                break;
            case 18:
                name.insert(name.lastIndexOf("["),"()");
                name.insert(name.indexOf("]"),"()");
                name.insert(name.lastIndexOf("["),"()");
                format = 1;
                break;
            case 19:
                name.insert(name.indexOf("]")+1,"()");
                name.insert(name.indexOf("]",name.indexOf("]")+1),"()");
                name.append("()");
                format = 2;
                break;
            case 20:
                name.insert(name.lastIndexOf("["),"()");
                name.append("()");
                format = 3;
                break;
            case 21:
                name.insert(name.indexOf("]"),"()");
                name.append("()");
                format = 3;
                break;
            case 22:
                name.insert(name.lastIndexOf("["),"()");
                name.insert(name.indexOf("]"),"()");
                format = 3;
                break;
            case 23:
                format = 4;
                break;
            case 24:
                name.insert(name.lastIndexOf("["),"()");
                name.insert(name.indexOf("]"),"()");
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
                name.insert(name.indexOf(")")+1,"[()]");
                format = 3;
            }
            if (format == 3) {
                name.append("[]");
                format = 1;
            }
            if (format == 2) {
                QString temp = name.left(name.indexOf("]")+1);
                name = name.mid(name.indexOf("]")+1)+temp;
                format = 1;
            }
            qDebug() << "Regular expression 2:";
            qDebug() << name;
            break;
        }
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
    name.replace("～", "~");
    name.replace("] ", "]");
    name.replace(" [", "[");
    name.replace(" (", "(");
    name.replace(") ", ")");
    name.replace(QString::fromLocal8Bit("[DL版]"), "");
    name.replace(QString::fromLocal8Bit("[Digital]"), "");
    name.replace(QString::fromLocal8Bit("[無修正]"), "");
    name.replace(QString::fromLocal8Bit("(無修正)"), "");
    name.replace(QString::fromLocal8Bit("[中国語]"), "");
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
    name.replace("  ", " ");
    name = name.trimmed();
    qDebug() << "stringReplacement 1:";
    qDebug() << name;
    return name;
}

QString Standardiser::haveTranslators(QString &name)
{
    QString special;
    bool noTranslators=false;
    if (name.contains(QString::fromLocal8Bit("[中国翻訳]"))) {
        name.replace(QString::fromLocal8Bit("[中国翻訳]"), "");
        noTranslators = true;
    }
    if (name.contains("[Chinese]")) {
        name.replace("[Chinese]", "");
        noTranslators = true;
    }
    int idx=name.section("[",0,0).indexOf(QString::fromLocal8Bit("★"));
    if (idx>-1){
        name.remove(idx,1);
        special=QString::fromLocal8Bit("★");
        return special;
    }

    if (noTranslators == true){
        special=QString::fromLocal8Bit("★");
    } else {
        special="";
    }
    return special;
}

QString Standardiser::ifLowQuality(QString &name, QString path)
{
    QString special;
    int idx=name.section("[",0,0).indexOf(QString::fromLocal8Bit("▼"));
    if (idx>-1){
        name.remove(idx,1);
        special=QString::fromLocal8Bit("▼");
        return special;
    }

    QStringList files=loadFiles(path);
    int fileCnt=1;
    int lowCnt=0;
    for(int i=0;i<files.length() && i<10;i++){
        fileCnt++;
        QImageReader reader(files.at(i));
        QSize sizeOfImage = reader.size();

        qDebug() << sizeOfImage.width()<<"----->"<<path;

        if (sizeOfImage.width()<setting->WIDTH_THRESHOLD) {
            lowCnt++;
        }
    }
    if(lowCnt*10/fileCnt>5)
        special = QString::fromLocal8Bit("▼");
    return special;
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
