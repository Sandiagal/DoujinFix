#ifndef LOGLISTMODEL_H
#define LOGLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>

namespace LogType
{
    enum LogTypeEnum {NORMALLOG=0,WARNINGLOG,ERRORLOG,OKLOG,IGNORELOG,NAMELOG,INDEXLOG};
}

class LogListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    LogListModel(QObject *parent = nullptr, int savelog = 100);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    enum LogTypeEnum {NORMALLOG=0,WARINGLOG,ERRORLOG,OKLOG,TIMERLOG,NETLOG,DEBUGLOG};

    void FlushLogFile(bool isExiting = false);
    QStringList logList;

private:
    int m_LogQueThreshold;
    QStringList logTimeList;
    QList<int> logTypeList;

signals:
    void doneWriting();

public slots:
    void WriteLog(QString string, int type = 1);
    void DelOldLog(int delDate=7);
};

#endif // LOGLISTMODEL_H
