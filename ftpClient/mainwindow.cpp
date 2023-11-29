#include <ftplib.h>//ftp库
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QTime>//时间类
#include <QTimerEvent>//定时器时间库
#include <QMessageBox>//弹窗库
#include <QCloseEvent>//关闭事件库
#include <ftpconnectdlg.h>
#include <QFileDialog>

void MainWindow::init_menu()
{
    QMenu* menu = new QMenu(ui->list_file);
    auto cdupAction = menu->addAction("返回上一级目录");
    auto refAction = menu->addAction("刷新");
    auto putAction = menu->addAction("上传");
    auto getAction = menu->addAction("下载");
    auto delAction = menu->addAction("删除");
    ui->list_file->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->list_file,&QTableWidget::customContextMenuRequested,[=](const QPoint& pos){
        qDebug() << ui->list_file->currentRow();
        menu->exec(ui->list_file->viewport()->mapToGlobal(pos));
    });
    connect(cdupAction, &QAction::triggered, this,&MainWindow::cdupAction);
    connect(refAction, &QAction::triggered, this,&MainWindow::refAction);
    connect(putAction, &QAction::triggered, this,&MainWindow::putAction);
    connect(getAction, &QAction::triggered, this,&MainWindow::getAction);
    connect(delAction, &QAction::triggered, this,&MainWindow::delAction);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,ui_connect_status("就绪")
    ,ui_pwd("当前路径：/")
    ,ui_cur_time(cur_time())
{
    ui->setupUi(this);
    ui->statusbar->addWidget(&ui_connect_status);//连接状态
    ui->statusbar->addWidget(&ui_pwd);//当前路径
    ui->statusbar->addPermanentWidget(&ui_cur_time);//当前时间
    connect(ui->action_exit,&QAction::triggered,this,[=](){
        qDebug() << "action_exit";
        if(QMessageBox::question(this,this->windowTitle(),"你确定要退出吗？") != QMessageBox::Yes) return;
        exit(0);
    });
    connect(ui->action_connect,&QAction::triggered,this,&MainWindow::onConnect);
    timer1 = startTimer(10);

    ui->list_file->setColumnCount(7);
    ui->list_file->setHorizontalHeaderLabels({"权限","链接数","属主","属组","大小","日期","文件名"});

    connect(ui->list_file,&QTableWidget::cellDoubleClicked,this,&MainWindow::cellDoubleClicked);

    init_menu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *e)//通过定时器事件更新状态栏中的当前时间。
{
    if(e->timerId() != timer1)return;
    ui_cur_time.setText(cur_time());
}

void MainWindow::closeEvent(QCloseEvent *e)//函数重写，关闭窗口时自动调用,QCloseEvent *e由QT生成传递到这个函数
{
    qDebug() << "closeEvent";
    if(QMessageBox::question(this,this->windowTitle(),"你确定要退出吗？") == QMessageBox::Yes)return;//关闭窗口
    e->ignore();//忽略这个QCloseEvent对象,窗口不会关闭
}

void MainWindow::onConnect()
{
    FtpConnectDlg dlg;
    if(dlg.exec() != QDialog::Accepted) return;//弹出FTP连接对话框，获取用户输入的FTP连接信息
    if(!ftp.login(dlg.ftp_data())) return;//登录
    ui_connect_status.setText("连接成功：" + dlg.ftp_data().host);//更新连接状态

    clear_ui_list();//清空文件列表
    set_pwd();
    insert_list(ftp.dir());//显示当前路径下的文件
}

void MainWindow::cellDoubleClicked(int row, int idx)
{
    if(row == -1)return;
    QString file_name = ui->list_file->item(row,6)->text();//在文件列表表格中双击某一行时，获取对应的文件名
    if(ui->list_file->item(row,0)->text()[0] != 'd')file_name = file_name.split(" ")[0];
    if(!ftp.cd(file_name)){//如果是目录，则尝试切换到该目录
        QMessageBox::warning(this,this->windowTitle(),QString("%1切换失败，%1可能不是目录").arg(file_name));
        return;
    }
    set_pwd();//更新当前路径显示
    auto list = ftp.dir();
    if(list.empty()){
        QMessageBox::warning(this,this->windowTitle(),QString("%1切换失败，%1可能不是目录").arg(file_name));
        return;
    }
    clear_ui_list();
    insert_list(list);//更新文件列表
}

void MainWindow::cdupAction()//返回上一级目录，并更新文件列表和当前路径显示
{
    if(!ftp.cdup()) return;
    set_pwd();
    clear_ui_list();
    insert_list(ftp.dir());
}

void MainWindow::refAction()//刷新文件列表，重新显示当前路径下的文件
{
    clear_ui_list();
    insert_list(ftp.dir());
}

void MainWindow::putAction()
{
    QFileDialog file(this,"上传文件");//弹出文件选择对话框，选择要上传的文件
    file.setFileMode(QFileDialog::FileMode::ExistingFile);
    if(file.exec() != QDialog::Accepted)return;
    auto files = file.selectedFiles();
    if(files.size() > 1) {
        QMessageBox::warning(this,"选择文件过多！","最多选择一个文件");
        return;
    }
    qDebug() << files[0];
    if(!ftp.put(files[0])){//上传
        QMessageBox::warning(this,this->windowTitle(),"上传失败\n错误：" + ftp.error());
        return;
    }
    QMessageBox::information(this,this->windowTitle(),"上传成功！");
    clear_ui_list();
    insert_list(ftp.dir());//更新列表
}

void MainWindow::getAction()//下载文件
{
    int row = ui->list_file->currentRow();//获取用户点击行数
    if(row == -1){
        QMessageBox::warning(this,this->windowTitle(),"没有选择下载文件");
        return;
    }
    QFileDialog file(this,"下载文件到");
    file.setFileMode(QFileDialog::FileMode::AnyFile);
    file.selectFile(ui->list_file->item(row,6)->text());//获取用户在文件列表中选择的文件名
    if(file.exec() != QDialog::Accepted)return;
    auto files = file.selectedFiles();
    if(files.size() > 1) {
        QMessageBox::warning(this,"选择文件过多！","最多选择一个文件");
        return;
    }
    qDebug() << files[0];
    if(!ftp.get(files[0],ui->list_file->item(row,6)->text())){//下载
        QMessageBox::warning(this,this->windowTitle(),"下载失败\n错误：" + ftp.error());
        return;
    }
    QMessageBox::information(this,this->windowTitle(),"下载成功！");
}

void MainWindow::delAction()//删除文件
{
    int row = ui->list_file->currentRow();
    if(row == -1){
        QMessageBox::warning(this,this->windowTitle(),"没有选择删除文件");
        return;
    }
    if(!ftp.del(get_file_name(row)))
    {
        QMessageBox::warning(this,this->windowTitle(),"删除失败\n错误：" + ftp.error());
        return;
    }
    QMessageBox::information(this,this->windowTitle(),"删除成功！");
    clear_ui_list();
    insert_list(ftp.dir());
}

QString MainWindow::cur_time()//获取当前时间
{
    return QTime::currentTime().toString();
}

QString MainWindow::get_file_name(int row)//获取文件名
{
    auto file_name = ui->list_file->item(row,6)->text();
    if(file_name.contains(" ") && ui->list_file->item(row,0)->text()[0] == 'l')
        file_name = file_name.split(" ")[0];//若文件名有空格,则以“ ”分开,前面的一截
    return file_name;
}

void MainWindow::set_pwd()//设置当前路径的显示
{
    ui_pwd.setText("当前路径：" + ftp.pwd());
}

void MainWindow::clear_ui_list()//清空文件列表
{
    while(ui->list_file->rowCount() > 0)ui->list_file->removeRow(0);
}

void MainWindow::insert_list(const std::vector<FTP_FILE_INFO> &list)//将FTP获取到的文件信息列表插入到文件列表表格中
{
    for(auto& info : list)
    {
        insert_row(ui->list_file->rowCount(),info);
    }
}

void MainWindow::insert_row(int row, const FTP_FILE_INFO &info)//在文件列表表格的指定行插入一行文件信息
{
    ui->list_file->insertRow(row);
    insert_item(row,0,info.access);
    insert_item(row,1,info.link_cnt);
    insert_item(row,2,info.ower);
    insert_item(row,3,info.group);
    insert_item(row,4,info.size);
    insert_item(row,5,info.date);
    insert_item(row,6,info.file_name);
}

void MainWindow::insert_item(int row,int idx,QString item)//在文件列表表格的指定行和列插入一个表格项
{
    ui->list_file->setItem(row,idx,new QTableWidgetItem(item));
    ui->list_file->item(row,idx)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);//表格项可选中的,可勾选,并可用的
}

