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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QTimer>
#include <QStyleFactory>
#include <QMessageBox>
#include <QSimpleUpdater.h>
#include <QClipboard>
#include <QProcess>
#include <QMimeData>

//Define the URL of the Update Definitions file
static const QString DEFS_URL = "https://raw.githubusercontent.com/Sandiagal/DoujinFix/master/updates.json";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("DoujinFix v%1 Present by Sandiagal").arg(APP_VERSION));

    //读取配置
    connect(setting, &Setting::writeLog, logModel, &LogListModel::WriteLog);
    setting->loadSetting();
    ui->lineEdit_2->setText(setting->ORIGIN_PATH);
    ui->lineEdit_3->setText(setting->TARGET_PATH);
    ui->checkBox->setChecked(setting->INDEX_FIX);
    ui->checkBox_2->setChecked(setting->PARODY_MAP);
    ui->checkBox_3->setChecked(setting->UNKNOWN_TRANSLATOR);
    ui->checkBox_4->setChecked(setting->LOW_QUALITY);
    ui->spinBox_2->setValue(setting->WIDTH_THRESHOLD);
    ui->comboBox->setCurrentIndex(setting->NAMING_STYLE);
    ui->spinBox->setValue(setting->SIZE_THRESHOLD);

    //日志
    connect(this, &MainWindow::writeLog, logModel, &LogListModel::WriteLog);
    ui->listView->setModel(logModel);
    logModel->DelOldLog(7);
    connect(logModel, &LogListModel::doneWriting, ui->listView, &QListView::scrollToBottom);
    connect(ui->listView, &QListView::customContextMenuRequested, this, &MainWindow::context_menu_on_listView);
    logCnt = QVector<int>(4,0);

    //名称标准化
    standardiser = new Standardiser(setting);
    connect(standardiser, &Standardiser::writeLog, logModel, &LogListModel::WriteLog);
    connect(standardiser, &Standardiser::setProgressBarValue, ui->progressBar, &QProgressBar::setValue);
    connect(standardiser, &Standardiser::showMessage, ui->statusBar, &QStatusBar::showMessage);
    standardiser->moveToThread(&standardiserThread);
    connect(this, &MainWindow::startStandardiser, standardiser, &Standardiser::run);
    connect(standardiser, &Standardiser::workDone, this, &MainWindow::standardiserDone);
    standardiserThread.start();

    //压缩图像替换
    compressor = new Compressor(setting);
    connect(compressor, &Compressor::writeLog, logModel, &LogListModel::WriteLog);
    connect(compressor, &Compressor::setDirBarValue, ui->progressBar_4, &QProgressBar::setValue);
    connect(compressor, &Compressor::setFileBarValue, ui->progressBar_3, &QProgressBar::setValue);
    connect(compressor, &Compressor::showMessage, ui->statusBar, &QStatusBar::showMessage);
    compressor->moveToThread(&compressorThread);
    connect(this, &MainWindow::startCompressor, compressor, &Compressor::run);
    connect(compressor, &Compressor::workDone, this, &MainWindow::compressorDone);
    compressorThread.start();

    //    ("windowsvista", "Windows", "Fusion")
    qDebug()<<QStyleFactory::keys();
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    emit writeLog(QString::fromLocal8Bit("初始化完毕"), LogType::OKLOG);

    /* QSimpleUpdater is single-instance */
    m_updater = QSimpleUpdater::getInstance();
    connect(m_updater, &QSimpleUpdater::haveUpdate, this, &MainWindow::haveUpdate);
    connect(m_updater, &QSimpleUpdater::quitNow, this, &MainWindow::updateQuit);
    /* Apply the settings */
    m_updater->setModuleVersion (DEFS_URL, APP_VERSION);
    /* Check for updates */
    m_updater->checkForUpdates (DEFS_URL);
}

MainWindow::~MainWindow()
{
    standardiserThread.quit();
    standardiserThread.wait();
    if(standardiser) delete standardiser;

    compressorThread.quit();
    compressorThread.wait();
    if(compressor) delete compressor;

    setting->ORIGIN_PATH=ui->lineEdit_2->text();
    setting->TARGET_PATH=ui->lineEdit_3->text();
    setting->INDEX_FIX=ui->checkBox->isChecked();
    setting->PARODY_MAP=ui->checkBox_2->isChecked();
    setting->NAMING_STYLE=ui->comboBox->currentIndex();
    setting->SIZE_THRESHOLD=ui->spinBox->value();
    setting->saveSetting();
    delete setting;

    delete ui;

    logModel->FlushLogFile();
    emit writeLog(QString::fromLocal8Bit("系统关闭"), LogType::OKLOG);
    delete logModel;
}

void MainWindow::standardiserGroupEnable(bool status)
{
    ui->pushButton->setEnabled(status);
    ui->pushButton_5->setEnabled(status);
    ui->lineEdit_2->setEnabled(status);
    ui->comboBox->setEnabled(status);
    ui->checkBox->setEnabled(status);
    ui->checkBox_2->setEnabled(status);
    ui->checkBox_3->setEnabled(status);
    ui->checkBox_4->setEnabled(status);
    ui->spinBox_2->setEnabled(status);
}

void MainWindow::compressorGroupEnable(bool status)
{
    ui->pushButton_7->setEnabled(status);
    ui->pushButton_4->setEnabled(status);
    ui->lineEdit_3->setEnabled(status);
    ui->spinBox->setEnabled(status);
}

//名称标准化
void MainWindow::on_pushButton_clicked()
{
    logCnt[0]=logModel->logList.count();
    logCnt[2]=0;
    logCnt[3]=0;

    emit startStandardiser(0);

    standardiserGroupEnable(false);

    ui->pushButton_6->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

//撤销 名称标准化
void MainWindow::on_pushButton_2_clicked()
{
    logCnt[0]=logModel->logList.count();
    logCnt[2]=0;
    logCnt[3]=0;

    emit startStandardiser(1);

    ui->pushButton_2->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

void MainWindow::standardiserDone(int SR)
{
    logCnt[1]=logModel->logList.count();

    if(SR==0){
        ui->pushButton_2->setEnabled(true);

        ui->pushButton_3->setEnabled(true);
    }else{
        standardiserGroupEnable(true);

        compressorGroupEnable(true);

        ui->pushButton_3->setEnabled(true);
    }
}

//压缩图像替换
void MainWindow::on_pushButton_7_clicked()
{
    logCnt[2]=logModel->logList.count();

    emit startCompressor(0);

    compressorGroupEnable(false);

    ui->pushButton_2->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

//撤销 压缩图像替换
void MainWindow::on_pushButton_6_clicked()
{
    logCnt[2]=logModel->logList.count();

    emit startCompressor(1);

    ui->pushButton_6->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

void MainWindow::compressorDone(int SR)
{
    logCnt[3]=logModel->logList.count();

    if (SR==0) {
        ui->pushButton_6->setEnabled(true);

        ui->pushButton_3->setEnabled(true);
    } else {
        compressorGroupEnable(true);

        standardiserGroupEnable(true);

        ui->pushButton_3->setEnabled(true);
    }
}

//选择路径 origin
void MainWindow::on_pushButton_5_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    QString::fromLocal8Bit("打开目录"),
                                                    ui->lineEdit_2->text(),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        ui->lineEdit_2->setText(dir);
    }
}

//选择路径 target
void MainWindow::on_pushButton_4_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    QString::fromLocal8Bit("打开目录"),
                                                    ui->lineEdit_3->text(),
                                                    QFileDialog::ShowDirsOnly|
                                                    QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        ui->lineEdit_3->setText(dir);
    }
}

//重开
void MainWindow::on_pushButton_3_clicked()
{
    logCnt = QVector<int>(4,0);

    compressorGroupEnable(true);

    standardiserGroupEnable(true);

    ui->pushButton_6->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    ui->pushButton_3->setEnabled(false);

    ui->progressBar->setValue(0);
    ui->progressBar_3->setValue(0);
    ui->progressBar_4->setValue(0);
    logModel->FlushLogFile();
}

//备份目录
void MainWindow::on_actionasd_triggered()
{
    QString backupsDir = QDir(QDir::currentPath()).filePath("backups");
    QDesktopServices::openUrl(QUrl::fromLocalFile(backupsDir));
}

//配置文件
void MainWindow::on_action_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("config.ini"));
}

//项目地址
void MainWindow::on_action_3_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Sandiagal/DoujinFix/", QUrl::TolerantMode));
}

//原作映射
void MainWindow::on_action_4_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("parodyMap.txt"));
}

//关于软件
void MainWindow::on_action_2_triggered()
{
    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = QString::fromLocal8Bit("<h2>关于 DoujinFix</h2>");
    QString translatedTextAboutQtText;
    translatedTextAboutQtText = QString::fromLocal8Bit(
                "<h3>信息和介绍</h3>"
                "<p>名称：DoujinFix</p>"
                "<p>版本：%1</p>"
                "<p>作者：Sandiagal</p>"
                "<p>发布博客：<a href=\"https://www.sdyg.men/\">https://www.sdyg.men</a></p>"
                "<p>项目主页：<a href=\"https://github.com/Sandiagal/DoujinFix/\">DoujinFix</a></p>"
                "<p>DoujinFix 是一款基于 Qt 的批量整理同人志的软件，包括同人志文件名标准化和压缩图像替换功能。"
                "<h3>动机与需求</h3>"
                "<p>同人志为同人文化的产物，是指一群同好走在一起，所共同创作出版的书籍、刊物。"
                "在互联网模因的推动下，同人志呈现出百花齐放的态势，形成了现代不可忽略的文化因子之一。"
                "然而作为非官方非商业的产物，同人志的创作与流通没有统一标准。"
                "甚至各家都有一套自己的规定，导致人工的统一收集和储存都存在费时费力的困难。"
                "好在同人志的整体发展是趋于规范的，并且不同地方标准之间可以互相转换，所以我们开发了 DoujinFix 来自动地批量整理同人志。</p>"
                "<h3>制作和感谢</h3>"
                "<p>DoujinFix 基于跨平台的 C++ 应用程式框架 <a href=\"https://www.qt.io/\">Qt</a> 开发，"
                "并修改了以下优秀的代码模块用以定制：</p>"
                "<p>-配置管理，日志管理：原作者拉斐尔之翼的等模块用以定制软件；</p>"
                "<p>-在线更新：原作者 alex-spataru。</p>"
                "<p>感谢 Github 里的好友对 DoujinFix 的编写提供众多支持，拉斐尔之翼，alex-spataru，魂跃，Argh等。</p>"
                "<p>感谢所有使用和支持 DoujinFix 的网友。</p>"
                "<h3>声明和协议</h3>"
                "<p>版权所有 (C) 2019 Sandiagal</p>"
                "<p>本程序是自由软件，您可以遵照自由软件基金会 ( Free Software Foundation ) "
                "出版的 GNU 通用公共许可证条款 ( GNU General Public License ) 第3版或者任一更新版本的前提下"
                "修改和重新发布这一程序。</p>"
                "<p>发布这一程序的目的是希望它有用，但没有任何担保。"
                "甚至没有适合特定目的而隐含的担保。更详细的情况请参阅 GNU 通用公共许可证。</p>"
                "<p>你理当已收到一份GNU通用公共许可协议的副本，如果没有，请查阅"
                "<a href=\"https://www.gnu.org/licenses/\">https://www.gnu.org/licenses/</a>。</p>"
                ).arg(APP_VERSION);
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(QString::fromLocal8Bit("关于 DoujinFix"));
    msgBox->setText(translatedTextAboutQtCaption);
    msgBox->setInformativeText(translatedTextAboutQtText);

    QPixmap pm(QLatin1String(":/ico/Sandiagal.ico"));
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->setDefaultButton(msgBox->addButton(QMessageBox::Ok));
    msgBox->show();
}

//源头目录路径改变
void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    setting->ORIGIN_PATH=arg1;
}

//目标目录路径改变
void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    setting->TARGET_PATH=arg1;
}

//处理延迟改变
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    setting->PROCESS_DELAY=value;
}

//原作映射改变
void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    setting->PARODY_MAP=arg1;
}

//序号优化改变
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    setting->INDEX_FIX=arg1;
}

//未知译者改变
void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    setting->UNKNOWN_TRANSLATOR=arg1;
}

//低质图像改变
void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    setting->LOW_QUALITY=arg1;
    ui->spinBox_2->setEnabled(arg1);
}

//命名格式改变
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    setting->NAMING_STYLE=index;
}

//容量阈值改变
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    setting->SIZE_THRESHOLD=arg1;
}

//宽度阈值改变
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    setting->WIDTH_THRESHOLD=arg1;
}

// 更新按钮
void MainWindow::on_action_5_triggered()
{
    m_updater->doUpdates(DEFS_URL);
}

// 日志菜单
void MainWindow::context_menu_on_listView(const QPoint &pos)
{
    QPoint globalpos = ui->listView->mapToGlobal(pos);

    QMenu contextMenu;

    QModelIndex pointedItem = ui->listView->indexAt(pos);
    if(pointedItem.isValid()) {
        QString log = pointedItem.data().toString();
        qDebug()<<logModel->logCopy[pointedItem.row()];
        contextMenu.addAction(ui->action_7);
        logRow = pointedItem.row();

        if (log[1]==QString::fromLocal8Bit("序")||
                log[1]==QString::fromLocal8Bit("名")||
                log[1]==QString::fromLocal8Bit("替")||
                log[1]==QString::fromLocal8Bit("忽")||
                log[1]==QString::fromLocal8Bit("错")||
                log[1]==QString::fromLocal8Bit("警")) {

            if ( (logCnt[0]<logRow && logRow<logCnt[1]) ||
                  (logCnt[2]<logRow && logRow<logCnt[3]) ) {
                contextMenu.addAction(ui->action_10);
                contextMenu.addAction(ui->action_9);
                contextMenu.addSeparator();
                contextMenu.addAction(ui->action_11);
                contextMenu.addAction(ui->action_8);
            }else {
                contextMenu.addSeparator();
                contextMenu.addAction(new QAction(QString::fromLocal8Bit("日志已过期")));
            }
            logString = logModel->logCopy[logRow];
        }

        contextMenu.exec(globalpos);
    }
}

// 如果有更新
void MainWindow::haveUpdate()
{
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_5);
    emit writeLog(QString::fromLocal8Bit("有新内容"), LogType::OKLOG);
}

// 退出程序完成更新
void MainWindow::updateQuit()
{
    emit writeLog(QString::fromLocal8Bit("更新退出"), LogType::OKLOG);
    QApplication::quit();
}

// 赞助通道
void MainWindow::on_action_6_triggered()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(QString::fromLocal8Bit("您的赞助是我们前进的动力"));

    QPixmap pm(QLatin1String(":/ico/Receipt.ico"));
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->setDefaultButton(msgBox->addButton(QMessageBox::Ok));
    msgBox->show();
}

// 复制
void MainWindow::on_action_7_triggered()
{
    QApplication::clipboard()->setText(logModel->logList[logRow]);
}

// 打开路径
void MainWindow::on_action_8_triggered()
{
    logString.replace("/","\\");
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(logString);
    QProcess::startDetached("explorer", args);
}

// 复制完整目录路径
void MainWindow::on_action_9_triggered()
{
    QApplication::clipboard()->setText(logString);
}

// 复制目录名
void MainWindow::on_action_10_triggered()
{
    QApplication::clipboard()->setText(logString.section("/",-1));
}

// 打开
void MainWindow::on_action_11_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(logString));
}
