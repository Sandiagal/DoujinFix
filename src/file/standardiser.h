#ifndef STANDARDISER_H
#define STANDARDISER_H

#include <QFileInfo>
#include <QString>
#include <QStringList>
#include "loglistmodel.h"
#include "file/baseFile.h"

class Standardiser : public BaseFile
{
    Q_OBJECT
public:
    explicit Standardiser(Setting *setting, QObject *parent=nullptr);

    void start() override;
    void revoke() override;

private:
    QStringList *patterns;
    QStringList newDirs;

    QString dirStandard(QFileInfo &dirInfo);
    int fileStandard(const QStringList &files);
    QString &nameAnalysis(QString &name);
    QString &stringReplacement(QString &name);
    QString haveTranslators(QString &name);
    QString ifLowQuality(QString &name, QString path);
    QStringList readInfo(const QString &name);

    float stringSimilar(const QString &strA,const QString &strB);
    void parodyMap(QString &parody);

    QString join(QStringList &component);
    QString &stringReplacement2(QString &name);

signals:
    void setProgressBarValue(int value);

public slots:
};

#endif // STANDARDISER_H
