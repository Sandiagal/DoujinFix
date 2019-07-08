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

#ifndef LOGLISTMODEL_H
#define LOGLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>

namespace LogType
{
    enum LogTypeEnum {OKLOG,ERRORLOG,IGNORELOG,NAMELOG,INDEXLOG,REPLACELOG,WARNINGLOG};
}

class LogListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    LogListModel(QObject *parent = nullptr, int savelog = 100);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void FlushLogFile(bool isExiting = false);
    QStringList logList;
    QStringList logCopy;

private:
    int m_LogQueThreshold;
    QStringList logTimeList;
    QList<int> logTypeList;

signals:
    void doneWriting();

public slots:
    void WriteLog(QString string, int type = 1, QString copy = "");
    void DelOldLog(int delDate=7);
};

#endif // LOGLISTMODEL_H
