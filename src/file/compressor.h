#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "file/baseFile.h"

class Compressor : public BaseFile
{
    Q_OBJECT
public:
    explicit Compressor(Setting *setting, QObject *parent = nullptr);

    void start() override;
    void revoke() override;

private:
    QVector<int> subimageCompression(const QString &path);

signals:
    void setDirBarValue(int value);
    void setFileBarValue(int value);

};

#endif // COMPRESSOR_H
