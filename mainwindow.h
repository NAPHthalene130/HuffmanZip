#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

// 前置声明 Qt 类，加快编译
class QLineEdit;
class QPushButton;
class QProgressBar;
class QPlainTextEdit;
class QRadioButton;
class QButtonGroup;
class QLabel;
class QFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // --- ZipUtil 调用的公共接口 (线程安全) ---

    // 写入日志到界面下方
    void logWrite(std::string line);

    // 设置进度条最大值
    void setBarTotal(int value);

    // 进度条当前值 +1
    void addBarProgress();

    // 设置进度条状态及颜色 (0:成功绿, 1:警告黄, 2:失败红, 3:进行中蓝)
    void setBarState(int type);

protected:
    // 拖拽事件重写
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onSelectInput();
    void onSelectOutput();
    void onExecute();       // 点击执行按钮
    void onModeChanged(int id); // 模式改变时触发(用于更新UI提示)

private:
    // UI 组件
    QButtonGroup *modeGroup;
    QRadioButton *rbCompress;
    QRadioButton *rbDecompress;

    QFrame *dragDropFrame;  // 拖拽区域容器
    QLabel *dragLabel;      // 拖拽提示文字
    QLineEdit *inputPathEdit;
    QPushButton *inputBrowseBtn;

    QLineEdit *outputPathEdit;
    QPushButton *outputBrowseBtn;

    QPushButton *executeBtn;   // 核心执行按钮
    QProgressBar *progressBar;
    QPlainTextEdit *logArea;

    // 私有辅助方法
    void setupUi();        // 布局初始化
    void setupStyle();     // 加载美化样式表 (CSS)

    // 实际执行逻辑的入口
    void startWorkerThread(int type, std::string inputPath, std::string outputPath);
};
#endif // MAINWINDOW_H