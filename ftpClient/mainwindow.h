#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ftp.h"
#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void init_menu(); // 初始化右键菜单
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void timerEvent(QTimerEvent *e); // 定时器事件
    void closeEvent(QCloseEvent *e); // 关闭窗口事件
public slots:
    void onConnect(); // 连接FTP服务器
    void cellDoubleClicked(int row, int idx); // 双击列表项事件
    void cdupAction(); // 返回上一级目录
    void refAction(); // 刷新目录
    void putAction(); // 上传文件
    void getAction(); // 下载文件
    void delAction(); // 删除文件

public:
    static QString cur_time(); // 获取当前时间
private:
    QString get_file_name(int row); // 获取文件名
    void set_pwd(); // 设置当前路径
    void clear_ui_list(); // 清空列表
    void insert_list(const std::vector<FTP_FILE_INFO>& list); // 插入文件列表
    void insert_row(int row,const FTP_FILE_INFO& info); // 插入一行文件信息
    void insert_item(int row,int idx,QString item); // 插入一项文件信息
private:
    Ui::MainWindow *ui;
    QLabel ui_connect_status;
    QLabel ui_pwd;
    QLabel ui_cur_time;
    int timer1;
    Ftp ftp;
};
#endif // MAINWINDOW_H
