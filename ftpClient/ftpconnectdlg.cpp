#include "ftpconnectdlg.h"
#include "ui_ftpconnectdlg.h"

FtpConnectDlg::FtpConnectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FtpConnectDlg)
{
    ui->setupUi(this);
//    ui->edit_host->setText("127.0.0.1");//192.168.217.214
    ui->edit_host->setText("192.168.56.1");//192.168.217.214
    ui->edit_user->setText("xb1520");
    ui->edit_pass->setText("xb1520");
    connect(ui->btn_connect,&QPushButton::clicked,this,&FtpConnectDlg::onConnect);
}

FtpConnectDlg::~FtpConnectDlg()
{
    delete ui;
}

FTP_DATA FtpConnectDlg::ftp_data()
{
    return
    {
        .host = ui->edit_host->text()
        ,.user = ui->edit_user->text()
        ,.pass = ui->edit_pass->text()
    };
}

void FtpConnectDlg::onConnect()
{
    QDialog::accept();
}
