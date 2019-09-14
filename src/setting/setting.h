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

#ifndef SETTING_H
#define SETTING_H

#include <QSettings>
#include "custom/loglistmodel.h"

class Setting : public QObject
{
    Q_OBJECT
public:
    Setting(QObject *parent = nullptr);

    QString ORIGIN_PATH;
    QString TARGET_PATH;
    int PROCESS_DELAY;
    bool INDEX_FIX;
    bool LABEL_FILTER;
    bool PARODY_MAP;
    bool UNKNOWN_TRANSLATOR;
    bool LOW_QUALITY;
    int WIDTH_THRESHOLD;
    int NAMING_STYLE;
    int SIZE_THRESHOLD;
    QMap<QString, QString> *NAME_MAP;

    void loadSetting();
    void saveSetting();

signals:
     void writeLog(QString content, int type = 1, const QString &copy = "");
};

#endif // SETTING_H
