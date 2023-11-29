#ifndef FTPCONNECTDLG_H
#define FTPCONNECTDLG_H

#include <QDialog>
#include "ftp.h"
namespace Ui {
class FtpConnectDlg;
}



class FtpConnectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FtpConnectDlg(QWidget *parent = nullptr);//explicit关键字用于防止隐式转换
    ~FtpConnectDlg();
public:
    FTP_DATA ftp_data();//返回ftp连接信息
public slots:
    void onConnect();
private:
    Ui::FtpConnectDlg *ui;//连接信息输入框ui指针
};

#endif // FTPCONNECTDLG_H
