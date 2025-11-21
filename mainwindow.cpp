#include "mainwindow.h"
#include "ziputil.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFrame>
#include <QDateTime>
#include <QScrollBar>
#include <QStyle>
#include <thread>
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupStyle();
    setAcceptDrops(true);
    resize(700, 600);
    setWindowTitle("Huffman Zip Tool");
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    QFrame *topCard = new QFrame(this);
    topCard->setObjectName("Card"); // 用于 CSS 选择器
    QHBoxLayout *topLayout = new QHBoxLayout(topCard);

    QLabel *titleLabel = new QLabel("操作模式", this);
    titleLabel->setObjectName("SectionTitle");

    rbCompress = new QRadioButton("文件压缩", this);
    rbDecompress = new QRadioButton("文件解压", this);
    rbCompress->setChecked(true);
    rbCompress->setCursor(Qt::PointingHandCursor);
    rbDecompress->setCursor(Qt::PointingHandCursor);

    modeGroup = new QButtonGroup(this);
    modeGroup->addButton(rbCompress, 0);
    modeGroup->addButton(rbDecompress, 1);
    connect(modeGroup, &QButtonGroup::idClicked, this, &MainWindow::onModeChanged);

    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(rbCompress);
    topLayout->addSpacing(20);
    topLayout->addWidget(rbDecompress);
    mainLayout->addWidget(topCard);
    QFrame *fileCard = new QFrame(this);
    fileCard->setObjectName("Card");
    QVBoxLayout *fileLayout = new QVBoxLayout(fileCard);
    dragDropFrame = new QFrame(this);
    dragDropFrame->setObjectName("DragArea"); // CSS 重点美化对象
    QVBoxLayout *dragLayout = new QVBoxLayout(dragDropFrame);

    dragLabel = new QLabel("拖拽文件或文件夹到此处\n或者点击右侧按钮浏览", this);
    dragLabel->setAlignment(Qt::AlignCenter);
    dragLabel->setObjectName("DragLabel");
    dragLayout->addWidget(dragLabel);

    // 输入框组合
    QHBoxLayout *inputHBox = new QHBoxLayout();
    inputPathEdit = new QLineEdit(this);
    inputPathEdit->setPlaceholderText("源文件路径...");
    inputPathEdit->setReadOnly(true); // 防止手滑输错
    inputBrowseBtn = new QPushButton("浏览文件", this);
    inputBrowseBtn->setCursor(Qt::PointingHandCursor);
    connect(inputBrowseBtn, &QPushButton::clicked, this, &MainWindow::onSelectInput);

    inputHBox->addWidget(inputPathEdit);
    inputHBox->addWidget(inputBrowseBtn);

    // 输出框组合
    QHBoxLayout *outputHBox = new QHBoxLayout();
    outputPathEdit = new QLineEdit(this);
    outputPathEdit->setPlaceholderText("输出保存路径...");
    outputBrowseBtn = new QPushButton("另存为...", this);
    outputBrowseBtn->setCursor(Qt::PointingHandCursor);
    connect(outputBrowseBtn, &QPushButton::clicked, this, &MainWindow::onSelectOutput);

    outputHBox->addWidget(outputPathEdit);
    outputHBox->addWidget(outputBrowseBtn);

    fileLayout->addWidget(dragDropFrame);
    fileLayout->addLayout(inputHBox);
    fileLayout->addSpacing(10);
    fileLayout->addLayout(outputHBox);
    mainLayout->addWidget(fileCard);

    // 进度条
    progressBar = new QProgressBar(this);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setFixedHeight(20);
    mainLayout->addWidget(progressBar);

    executeBtn = new QPushButton("开始执行", this);
    executeBtn->setObjectName("ExecuteBtn"); // CSS 重点美化
    executeBtn->setCursor(Qt::PointingHandCursor);
    executeBtn->setFixedHeight(50); // 按钮高度
    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::onExecute);
    mainLayout->addWidget(executeBtn);

    //日志区
    QLabel *logTitle = new QLabel("运行日志", this);
    logTitle->setStyleSheet("font-weight: bold; color: #555; margin-top: 10px;");
    mainLayout->addWidget(logTitle);

    logArea = new QPlainTextEdit(this);
    logArea->setReadOnly(true);
    logArea->setObjectName("LogArea");
    logArea->setPlaceholderText("等待操作...");
    mainLayout->addWidget(logArea);
}

void MainWindow::setupStyle()
{
    QString qss = R"(
        /* 全局背景 */
        QMainWindow {
            background-color: #F0F2F5;
        }

        /* 卡片容器风格 */
        QFrame#Card {
            background-color: #FFFFFF;
            border-radius: 10px;
            border: 1px solid #E1E4E8;
        }

        /* 标题文字 */
        QLabel#SectionTitle {
            font-size: 16px;
            font-weight: bold;
            color: #333333;
        }

        /* 拖拽区域 - 虚线边框 */
        QFrame#DragArea {
            background-color: #FAFBFC;
            border: 2px dashed #D1D5DA;
            border-radius: 8px;
            padding: 20px;
        }
        QLabel#DragLabel {
            color: #999999;
            font-size: 14px;
        }

        /* 输入框 */
        QLineEdit {
            border: 1px solid #ced4da;
            border-radius: 4px;
            padding: 6px;
            background-color: #fff;
            color: #495057;
        }
        QLineEdit:focus {
            border: 1px solid #80bdff;
        }

        /* 普通按钮 */
        QPushButton {
            background-color: #6c757d;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5a6268;
        }
        QPushButton:pressed {
            background-color: #545b62;
        }

        /* 执行按钮 (主按钮 - 蓝色) */
        QPushButton#ExecuteBtn {
            background-color: #007BFF;
            color: white;
            font-size: 18px;
            border-radius: 6px;
        }
        QPushButton#ExecuteBtn:hover {
            background-color: #0056b3;
        }
        QPushButton#ExecuteBtn:disabled {
            background-color: #cccccc;
        }

        /* 进度条 */
        QProgressBar {
            border: none;
            background-color: #e9ecef;
            border-radius: 10px;
            text-align: center;
            color: black;
        }
        QProgressBar::chunk {
            border-radius: 10px;
            background-color: #28a745; /* 默认绿色 */
        }

        /* 日志区域 */
        QPlainTextEdit#LogArea {
            background-color: #212529; /* 深色背景 */
            color: #00ff00; /* 绿色黑客风格字体 */
            border-radius: 4px;
            font-family: Consolas, Monaco, monospace;
        }
    )";
    this->setStyleSheet(qss);
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        dragDropFrame->setStyleSheet("QFrame#DragArea { background-color: #E3F2FD; border: 2px dashed #2196F3; }");
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    dragDropFrame->setStyleSheet("");

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QString fileName = urls.first().toLocalFile();
    inputPathEdit->setText(fileName);

    onModeChanged(modeGroup->checkedId());

    logWrite("已加载文件: " + fileName.toStdString());
}

void MainWindow::onModeChanged(int id)
{
    QString inPath = inputPathEdit->text();
    if (inPath.isEmpty()) return;

    if (id == 0) { // 压缩模式
        outputPathEdit->setText(inPath + ".huff");
        executeBtn->setText("开始压缩");
    } else { // 解压模式
        if (inPath.endsWith(".huff")) {
            outputPathEdit->setText(inPath.left(inPath.length() - 5)); // 去掉后缀
        } else {
            outputPathEdit->setText(inPath + "_out");
        }
        executeBtn->setText("开始解压");
    }
}

void MainWindow::onSelectInput()
{
    QString path;
    if (rbCompress->isChecked()) {
        path = QFileDialog::getOpenFileName(this, "选择要压缩的文件");
    } else {
        path = QFileDialog::getOpenFileName(this, "选择要解压的文件", "", "Huffman Files (*.huff);;All Files (*)");
    }

    if (!path.isEmpty()) {
        inputPathEdit->setText(path);
        onModeChanged(modeGroup->checkedId());
        logWrite("选择源文件: " + path.toStdString());
    }
}

void MainWindow::onSelectOutput()
{
    QString path;
    if (rbCompress->isChecked()) {
        path = QFileDialog::getSaveFileName(this, "保存压缩文件", inputPathEdit->text() + ".huff");
    } else {
        path = QFileDialog::getExistingDirectory(this, "选择解压输出目录");
    }

    if (!path.isEmpty()) {
        outputPathEdit->setText(path);
    }
}

void MainWindow::onExecute()
{
    QString input = inputPathEdit->text();
    QString output = outputPathEdit->text();
    int type = modeGroup->checkedId();

    if (input.isEmpty() || output.isEmpty()) {
        logWrite("[错误] 请先选择输入和输出路径！");
        setBarState(2); // 变红
        return;
    }

    executeBtn->setEnabled(false);
    executeBtn->setText("正在处理...");
    setBarState(3); // 变蓝
    progressBar->setValue(0);

    // 启动线程
    startWorkerThread(type, input.toStdString(), output.toStdString());
}

void MainWindow::startWorkerThread(int type, std::string inputPath, std::string outputPath)
{
    namespace fs = std::filesystem;
    logWrite("任务开始...");

    std::thread worker([=]() {
        fs::path outP(outputPath);
        fs::path inP(inputPath);
        try {
            if (type == 0) {
                // 压缩
                double inSize = 0;
                if (fs::exists(inP)) {
                    if (is_directory(inP)) {
                        inSize = get_directory_size(inP);
                    } else {
                        inSize = file_size(inP);
                    }
                } else {
                    setBarState(1);
                    progressBar->setValue(progressBar->value() + 101);
                    logWrite("选择文件无效");
                    return;
                }
                ZipUtil::enCode(inputPath, outputPath, this);
                double outSize = 0;
                if (fs::exists(outP)) {
                    if (is_directory(outP)) {
                        outSize = get_directory_size(outP);
                    } else {
                        outSize = file_size(outP);
                    }
                }
                std::string inLine = "压缩前体积" + std::to_string(inSize) + "B";
                std::string outLine = "压缩后体积" + std::to_string(outSize) + "B";
                logWrite(inLine);
                logWrite(outLine);
                double percent = outSize / inSize * 100;
                std::string percentLine = "压缩后体积百分比：" + std::to_string(percent) + "%";
                logWrite(percentLine);
            } else {
                // 解压
                if (std::filesystem::is_directory(outP)) {
                    ZipUtil::deCode(inputPath,outputPath, this);
                } else {
                    setBarState(1);
                    progressBar->setValue(progressBar->value() + 101);
                    logWrite("解压输出目录设置错误，请查看是否设置的为文件");
                }

            }
        } catch (const std::exception &e) {
            std::string errMsg = e.what();
            QMetaObject::invokeMethod(this, [=](){
               logWrite("异常发生: " + errMsg);
               setBarState(2);
            });
        }

        // 任务结束恢复按钮
        QMetaObject::invokeMethod(this, [=](){
            executeBtn->setEnabled(true);
            if (modeGroup->checkedId() == 0) executeBtn->setText("开始压缩");
            else executeBtn->setText("开始解压");
        });
    });

    worker.detach();
}

void MainWindow::logWrite(std::string line)
{
    QString timeStr = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    QString qLine = QString::fromStdString(line);

    QMetaObject::invokeMethod(this, [=]() {
        logArea->appendPlainText(timeStr + qLine);
        logArea->verticalScrollBar()->setValue(logArea->verticalScrollBar()->maximum());
    }, Qt::QueuedConnection);
}

void MainWindow::setBarTotal(int value)
{
    QMetaObject::invokeMethod(this, [=]() {
        progressBar->setRange(0, value);
        progressBar->setValue(0);
    }, Qt::QueuedConnection);
}

void MainWindow::addBarProgress()
{
    QMetaObject::invokeMethod(this, [=]() {
        progressBar->setValue(progressBar->value() + 1);
    }, Qt::QueuedConnection);
}

void MainWindow::setBarState(int type)
{
    QMetaObject::invokeMethod(this, [=]() {
        QString color;
        switch (type) {
            case 0: color = "#28a745"; break; // Success Green
            case 1: color = "#ffc107"; break; // Warning Yellow
            case 2: color = "#dc3545"; break; // Error Red
            default: color = "#007BFF"; break; // Processing Blue
        }

        // 动态修改进度条 Chunk 的颜色
        // 注意：需要重新设置 StyleSheet 才能生效，因为我们用了 QSS
        QString style = QString("QProgressBar { border: none; background-color: #e9ecef; border-radius: 10px; text-align: center; color: black; } "
                                "QProgressBar::chunk { border-radius: 10px; background-color: %1; }").arg(color);
        progressBar->setStyleSheet(style);
    }, Qt::QueuedConnection);
}

double MainWindow::get_directory_size(const std::filesystem::path& path) {
    double total_size = 0;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        return 0; // 如果路径不存在或不是目录，返回 0
    }
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            // 检查条目是否是常规文件
            if (entry.is_regular_file()) {
                // 累加文件大小
                total_size += entry.file_size();
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "警告 (跳过): 无法访问: " << e.what() << std::endl;
    }

    return total_size;
}
