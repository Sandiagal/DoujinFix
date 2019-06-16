#include "loglistmodel.h"

#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QDateTime>

#include <QPixmap>
#include <QVariant>
#include <QColor>

#include <QDebug>

LogListModel::LogListModel(QObject *parent, int savelog)
    : QAbstractListModel(parent), m_LogQueThreshold(savelog)
{

}

int LogListModel::rowCount(const QModelIndex &parent) const
{
    return logList.count();
}

QVariant LogListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= logList.size())
        return QVariant();

    if(role == Qt::DisplayRole)
        return logList.at(index.row());

    if(role == Qt::DecorationRole)
    {
        QPixmap icon(10,10);
        switch (logTypeList.at(index.row()))
        {
        case LogType::OKLOG:
            icon.fill(QColor(0,0,255));
            break;
        case LogType::ERRORLOG:
            icon.fill(QColor(255,0,0));
            break;
        case LogType::IGNORELOG:
            icon.fill(QColor(192,192,192));
            break;
        case LogType::NAMELOG:
            icon.fill(QColor(0,255,128));
            break;
        case LogType::INDEXLOG:
            icon.fill(QColor(192,0,255));
            break;
        case LogType::REPLACELOG:
            icon.fill(QColor(0,192,128));
            break;
        case LogType::WARNINGLOG:
        default:
            icon.fill(QColor(255,128,0));
            break;
        }

        QVariant vTmp = icon;
        return vTmp;
    }

        return QVariant();
}

void LogListModel::WriteLog(QString string, int type)
{
    int tmpLen = logList.count();

    beginInsertRows(QModelIndex(), tmpLen, tmpLen);
    switch (type)
    {

    case LogType::OKLOG:
        string.prepend(QString::fromLocal8Bit("[完成] "));
        break;
    case LogType::ERRORLOG:
        string.prepend(QString::fromLocal8Bit("[错误] "));
        break;
    case LogType::IGNORELOG:
        string.prepend(QString::fromLocal8Bit("[忽略] "));
        break;
    case LogType::NAMELOG:
        string.prepend(QString::fromLocal8Bit("[名称] "));
        break;
    case LogType::INDEXLOG:
        string.prepend(QString::fromLocal8Bit("[序号] "));
        break;
    case LogType::REPLACELOG:
        string.prepend(QString::fromLocal8Bit("[替换] "));
        break;
    case LogType::WARNINGLOG:
    default:
        string.prepend(QString::fromLocal8Bit("[警告] "));
        break;
    }
    logList.push_back(string);
    logTypeList.push_back(type);
    logTimeList.push_back(QDateTime::currentDateTime().toString());
    QString tmpstr = QDateTime::currentDateTime().toString();
    endInsertRows();

    if(logList.count()>=m_LogQueThreshold)
        FlushLogFile();

    emit doneWriting();
}

void LogListModel::DelOldLog(int delDate)
{
    QDate st = QDate::currentDate();
    if(st.day() > delDate)
        st.setDate(st.year(), st.month(), st.day() - delDate);
    QString logFile = QString::asprintf("%04d%02d%02d.log", st.year(), st.month(), st.day());

    QDir dir("syslog");
    QFile fileCtrl;
    QStringList logFileEntries = dir.entryList(QStringList("*.log"),
                                         QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for(auto itr = logFileEntries.begin(); itr!=logFileEntries.end(); ++itr)
    {
        if((*itr)<logFile)
        {
            fileCtrl.remove(QDir::cleanPath("syslog/"+(*itr)));
        }
    }
}

void LogListModel::FlushLogFile(bool isExiting)
{
    QDir dir;
    if(!dir.exists("syslog"))
        dir.mkdir("syslog");

    QDate st = QDate::currentDate();
    QString logFile = QString::asprintf("syslog/%04d%02d%02d.log", st.year(), st.month(), st.day());

    QFile file(QDir::cleanPath(logFile));
    if(file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(true);

        beginRemoveRows(QModelIndex(), 0, logList.count() - 1);
        while (!logList.isEmpty())
        {
            out << logTimeList.first() << " " << logList.first() << endl;
            logList.pop_front();
            logTypeList.pop_front();
            logTimeList.pop_front();
        }
        endRemoveRows();

        if(isExiting) out << QString::fromLocal8Bit("----- 程序退出 -----") <<endl<<endl;

        file.close();
    }
}
