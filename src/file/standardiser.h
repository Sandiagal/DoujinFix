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

#ifndef STANDARDISER_H
#define STANDARDISER_H

#include <QFileInfo>
#include <QString>
#include <QStringList>
#include "custom/loglistmodel.h"
#include "file/baseFile.h"

class Standardiser : public BaseFile
{
    Q_OBJECT
public:
    explicit Standardiser(Setting *setting, QObject *parent = nullptr);

    void start() override;
    void revoke() override;

    QStringList newDirs;

private:
    QStringList patterns;
    QString filtedLabels;

    QString dirStandard(QFileInfo &dirInfo);
    int fileStandard(const QStringList &files);
    QString &nameAnalysis(QString &name);
    QString &stringReplacement(QString &name);
    bool ifvalid(QString &name);
    QString haveTranslators(QString &name);
    QString ifLowQuality(QString &name, QString path);
    QStringList readInfo(const QString &name);
    double stringSimilar(const QString &strA,const QString &strB);
    void parodyMap(QString &parody);
    QString join(QStringList &component);
    QString &stringReplacement2(QString &name);
    QString &labelStorage(QString &name, QString label);
    QString &labelStorage(QString &name, QRegExp rx);

signals:
    void setProgressBarValue(int value);

public slots:
};

#endif // STANDARDISER_H
