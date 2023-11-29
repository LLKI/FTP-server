#include "ftp.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>

Ftp::Ftp():
    ftp(new ftplib())
{
    ftp->SetConnmode(ftplib::port);// 设置连接模式为端口模式
}

Ftp::~Ftp()
{
    delete ftp;
}

bool Ftp::login(const FTP_DATA &data)//登录
{
    if(!ftp->Connect(data.host.toStdString().c_str())) return false;//连接服务器
    return ftp->Login(data.user.toStdString().c_str(),data.pass.toStdString().c_str());//登录
}

QString Ftp::pwd()//获取当前工作目录
{
    QByteArray arr(256,'\0');//创建256个(0x100)字节的QByteArray数组,全部初始化为空(null),用来存当前路径
    ftp->Pwd(arr.data(),arr.size());//调用ftplib库的Pwd方法获取当前路径
    return arr;//自动将QByteArray转换为QString
}

std::vector<FTP_FILE_INFO> Ftp::dir()// 获取当前工作目录下的文件列表(读取)
{
    //ftp->Dir("dir.txt",cur_path.c_str());
    if(!ftp->Dir("dir.txt",pwd().toStdString().c_str())) return {};// 获取文件列表并保存到本地文件
    QFile file(R"(dir.txt)");
    file.open(QFile::ReadOnly);
    QString list = file.readAll();
    file.close();

    // 解析文件列表
    std::vector<FTP_FILE_INFO> _list;
    QStringList lines = list.split("\r\n");
    for(auto& line : lines)
    {
        auto args = line.split(" ");
        args.removeAll("");
        if(args.size() < 9)continue;
        QString date = args[5] + " " + args[6] +  " " + args[7];
        QString file_name = args[8];
        for(int i{9};i < args.size();i++) file_name +=  " " + args[i];
        _list.push_back({
            .access = args[0]
            ,.link_cnt = args[1]
            ,.ower = args[2]
            ,.group = args[3]
            ,.size = args[4]
            ,.date = date
            ,.file_name = file_name
        });
    }
    return _list;
}

bool Ftp::cd(const QString &path)// 切换到指定目录
{
    return ftp->Chdir(path.toStdString().c_str());
}

bool Ftp::cdup()// 返回上一级目录
{
    return ftp->Cdup();
}

bool Ftp::put(const QString &put_file)// 上传文件 arg:要上传的本地文件的路径
{
//    return ftp->Put(put_file.toStdString().c_str(),"test.txt",ftplib::image);
    //arg:要上传的本地文件的路径 上传到FTP服务器后的文件名 二进制图像模式(上传文件模式)
    return ftp->Put(put_file.toStdString().c_str(),QFileInfo(put_file).fileName().toStdString().c_str(),ftplib::image);
}

bool Ftp::get(const QString &get_file,const QString& remote_file)//下载文件 args:要下载的FTP服务器上的文件的路径 下载后存储的本地文件的路径
{
    return ftp->Get(get_file.toStdString().c_str(),remote_file.toStdString().c_str(),ftplib::image);
}

bool Ftp::del(const QString &file)//删除
{
    bool ret = ftp->Delete(file.toStdString().c_str());//先尝试删除文件
    if(ret) return true;
    return ftp->Rmdir(file.toStdString().c_str());//删除不了则尝试删除目录
}

QString Ftp::error()// 获取FTP操作的错误信息
{
    return ftp->LastResponse();
}

/*

在FTP中，传输模式决定了如何处理文件的字节流。FTP客户端和服务器之间的文件传输可以使用两种主要的模式：文本模式和二进制模式。

文本模式：
文本模式将文本文件视为由文本行组成，其中每行由换行符（"\n"）或回车符（"\r"）和换行符组成。在文本模式下，文件的行尾符可能会根据系统（Windows、Linux、Mac等）进行转换。
这种模式通常用于传输纯文本文件，如文本文件、脚本文件等。

二进制模式：
二进制模式将文件视为字节的序列，不对其进行任何处理或转换。在这种模式下，每个字节都按照其原始值进行传输，不进行换行符的转换。
这种模式通常用于传输二进制文件，如图像、音频、视频文件等。
*/
