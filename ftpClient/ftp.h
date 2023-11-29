#ifndef FTP_H
#define FTP_H
#include <QObject>
#include <ftplib.h>

//用户连接信息
struct FTP_DATA
{
    QString host;
    QString user;
    QString pass;
};

//文件结构信息
struct FTP_FILE_INFO
{
    QString access;//权限
    QString link_cnt;//链接数
    QString ower;//属主
    QString group;//属组
    QString size;//大小
    QString date;//日期(3)
    QString file_name;//文件名(last)
//    uintptr_t size;
//    bool is_dir;
};

class Ftp
{
public:
    Ftp();
    ~Ftp();
public:
    bool login(const FTP_DATA& data);  // 登录FTP服务器
    QString pwd();  // 获取当前工作目录
    std::vector<FTP_FILE_INFO> dir();  // 获取当前工作目录下的文件列表
    bool cd(const QString& path);  // 切换到指定目录
    bool cdup();  // 返回上一级目录
    bool put(const QString& put_file);  // 上传文件
    bool get(const QString& get_file, const QString& remote_file);  // 下载文件
    bool del(const QString& file);  // 删除文件或目录
    QString error();  // 获取FTP操作的错误信息

private:
    ftplib* ftp;
    std::string cur_path;//当前路径
};

#endif // FTP_H
