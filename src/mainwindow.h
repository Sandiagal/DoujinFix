#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "setting/setting.h"
#include "file/standardiser.h"
#include "file/compressor.h"
#include "file/baseFile.h"
#include "loglistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Setting *setting = new Setting();
    LogListModel *logModel =new LogListModel(this, 500);
    Standardiser *standardiser;
    Compressor *compressor;
    QThread standardiserThread, compressorThread;

    void standardiserGroupEnable(bool status);
    void compressorGroupEnable(bool status);

signals:
    void startStandardiser(int SR);
    void startCompressor(int SR);
    void writeLog(QString content, int type);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void standardiserDone(int SR);
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void compressorDone(int SR);
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_actionasd_triggered();
    void on_action_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();
    void on_action_2_triggered();
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_horizontalSlider_valueChanged(int value);
    void on_checkBox_2_stateChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_3_stateChanged(int arg1);
    void on_checkBox_4_stateChanged(int arg1);
    void on_comboBox_currentIndexChanged(int index);
    void on_spinBox_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
