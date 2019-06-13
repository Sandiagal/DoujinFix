#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QTimer>
#include <QStyleFactory>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    emit startStandardiser(0);

    standardiserGroupEnable(false);

    ui->pushButton_6->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

//撤销 名称标准化
void MainWindow::on_pushButton_2_clicked()
{
    emit startStandardiser(1);
    ui->pushButton_2->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

void MainWindow::standardiserDone(int SR)
{
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
    emit startCompressor(0);

    compressorGroupEnable(false);

    ui->pushButton_2->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

//撤销 压缩图像替换
void MainWindow::on_pushButton_6_clicked()
{
    emit startCompressor(1);
    ui->pushButton_6->setEnabled(false);

    ui->pushButton_3->setEnabled(false);
}

void MainWindow::compressorDone(int SR)
{
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
                                                    tr("Open Directory"),
                                                    ui->lineEdit_2->text(),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    ui->lineEdit_2->setText(dir);
}

//选择路径 target
void MainWindow::on_pushButton_4_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Open Directory"),
                                                    ui->lineEdit_3->text(),
                                                    QFileDialog::ShowDirsOnly|
                                                    QFileDialog::DontResolveSymlinks);
    ui->lineEdit_3->setText(dir);
}

//重开
void MainWindow::on_pushButton_3_clicked()
{
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
    QDesktopServices::openUrl(QUrl(backupsDir, QUrl::TolerantMode));
}

//配置文件
void MainWindow::on_action_triggered()
{
    QDesktopServices::openUrl(QUrl("config.ini", QUrl::TolerantMode));
}

//项目地址
void MainWindow::on_action_3_triggered()
{

}

//原作映射
void MainWindow::on_action_4_triggered()
{
    QDesktopServices::openUrl(QUrl("parodyMap.txt", QUrl::TolerantMode));
}

//关于软件
void MainWindow::on_action_2_triggered()
{
    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = QString::fromLocal8Bit("<h1>关于 DoujinFix</h1>");
    QString translatedTextAboutQtText;
    translatedTextAboutQtText = QString::fromLocal8Bit(
                "<h2>信息和介绍</h2>"
                "<p>名称：DoujinFix</p>"
                "<p>版本：3.02</p>"
                "<p>作者：Sandiagal</p>"
                "<p>发布博客：<a href=\"https://www.sdyg.men/\">https://www.sdyg.men</a></p>"
                "<p>项目主页：<a href=\"https://github.com/Sandiagal/FileManagement/\">FileManagement</a></p>"
                "<p>DoujinFix 是一款基于 C++ 的批量整理同人志的软件，包括同人志文件名标准化和压缩图像替换功能。"
                "软件提供了完善丰富的交互功能，可以满足不同用户的整理需求。便捷高效实用是 DoujinFix 的制作目标。</p>"
                "<h2>动机与需求</h2>"
                "<p>由于国内的二次创作领域存在诸多“吃力不讨好”的现象，同人志在分享时存在名称中乱加标签的问题，"
                "加之专用名词的不统一，不规范的问题，给同人志的整理和收藏带来了不小的困难。"
                "此外，部分同人志的图片质量过好体积巨大，不利于整理，故经常需要进行压制。"
                "但图片的分享存在编码和质量不统一的问题，在压制后需要对体积压缩结果进行对比，费时费力。"
                "对此，为了能更方便的整理同人志，需要一款软件来解决上述问题。</p>"
                "<h2>制作和感谢</h2>"
                "<p>DoujinFix基于跨平台的 C++ 应用程式框架 <a href=\"https://www.qt.io/\">Qt</a> 开发，"
                "并修改了拉斐尔之翼的配置管理，日志管理等模块用以定制软件。</p>"
                "<p>感谢同窗的好友对 DoujinFix 的编写提供众多支持， 拉斐尔之翼，魂跃，Argh等。</p>"
                "<p>感谢所有使用和支持 DoujinFix 的网友。 </p>"
                "<h2>声明和协议</h2>"
                "<p>本软件遵从GNU LGPL v3开源协议，详情请看"
                "<a href=\"https://www.gnu.org/licenses/lgpl-3.0.en.html\">GNU LESSER GENERAL PUBLIC LICENSE</a></p>"
                "<p>本软件允许个人免费试用，但不得更改任何信息。网站转载或商业应用必须得到作者认可。</p>"
                );
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("关于 DoujinFix");
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


