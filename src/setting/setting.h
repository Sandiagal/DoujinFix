#ifndef SETTING_H
#define SETTING_H

#include <QSettings>
#include "loglistmodel.h"

class Setting : public QObject
{
    Q_OBJECT
public:
    Setting(QObject *parent = nullptr);

    QString ORIGIN_PATH;
    QString TARGET_PATH;
    int PROCESS_DELAY;
    bool INDEX_FIX;
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
     void writeLog(QString content, int type);
};

#endif // SETTING_H
