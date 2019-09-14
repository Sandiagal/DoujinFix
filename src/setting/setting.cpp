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

#include "setting.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

Setting::Setting(QObject *parent):QObject(parent)
{

}

void Setting::loadSetting()
{
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    ORIGIN_PATH=settings.value("ORIGIN_PATH", "D:/path/to/origin").toString();
    TARGET_PATH=settings.value("TARGET_PATH", "D:/path/to/target").toString();
    PROCESS_DELAY=settings.value("PROCESS_DELAY", 5).toInt();
    INDEX_FIX=settings.value("INDEX_FIX", true).toBool();
    LABEL_FILTER=settings.value("LABEL_FILTER", true).toBool();
    PARODY_MAP=settings.value("PARODY_MAP", true).toBool();
    UNKNOWN_TRANSLATOR=settings.value("UNKNOWN_TRANSLATOR", true).toBool();
    LOW_QUALITY=settings.value("LOW_QUALITY", true).toBool();
    WIDTH_THRESHOLD=settings.value("WIDTH_THRESHOLD", 960).toInt();
    NAMING_STYLE=settings.value("NAMING_STYLE", 0).toInt();
    SIZE_THRESHOLD=settings.value("SIZE_THRESHOLD", 3).toInt();

    NAME_MAP = new QMap<QString, QString>();
    QFile file("parodyMap.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "No nameMap.txt";
        emit writeLog(QString::fromLocal8Bit("无原作映射表"), LogType::WARNINGLOG);
    }
    QTextStream in(&file);
    in.setCodec("UTF-8"); // change the file codec to UTF-8.
    while (!in.atEnd())
    {
        QString reciveName = in.readLine();
        if (in.atEnd()) qDebug() << "Not plural lines in nameMap.txt";
        QString standardName = in.readLine();
        NAME_MAP->insert(reciveName,standardName);
    }
    file.close();

    //    QMapIterator<QString, QString> i(*NAME_MAP);
    //    while (i.hasNext()) {
    //        i.next();
    //        qDebug() << i.key() << ": " << i.value();
    //    }

    emit writeLog(QString::fromLocal8Bit("读取配置成功"), LogType::OKLOG);
}

void Setting::saveSetting()
{
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("ORIGIN_PATH", ORIGIN_PATH);
    settings.setValue("TARGET_PATH", TARGET_PATH);
    settings.setValue("LABEL_FILTER", LABEL_FILTER);
    settings.setValue("PROCESS_DELAY", PROCESS_DELAY);
    settings.setValue("INDEX_FIX", INDEX_FIX);
    settings.setValue("PARODY_MAP", PARODY_MAP);
    settings.setValue("UNKNOWN_TRANSLATOR", UNKNOWN_TRANSLATOR);
    settings.setValue("LOW_QUALITY", LOW_QUALITY);
    settings.setValue("WIDTH_THRESHOLD", WIDTH_THRESHOLD);
    settings.setValue("NAMING_STYLE", NAMING_STYLE);
    settings.setValue("SIZE_THRESHOLD", SIZE_THRESHOLD);
    emit writeLog(QString::fromLocal8Bit("保存配置成功"), LogType::OKLOG);
}


