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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "setting/setting.h"
#include "file/standardiser.h"
#include "file/compressor.h"
#include "file/baseFile.h"
#include "custom/loglistmodel.h"

namespace Ui {
class MainWindow;
}

class QSimpleUpdater;

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


    QVector<int> logCnt;
    int logRow;
    QString logString;

    QSimpleUpdater* m_updater;

    void standardiserGroupEnable(bool status);
    void compressorGroupEnable(bool status);

signals:
    void startStandardiser(int SR);
    void startCompressor(int SR);
    void writeLog(QString content, int type=1, const QString &copy="");

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
    void on_action_5_triggered();
    void context_menu_on_listView(const QPoint &);

    void haveUpdate();
    void updateQuit();
    void on_action_6_triggered();
    void on_action_7_triggered();
    void on_action_8_triggered();
    void on_action_9_triggered();
    void on_action_10_triggered();
    void on_action_11_triggered();

};

#endif // MAINWINDOW_H
