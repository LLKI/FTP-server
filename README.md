# FTP-server
平台：Ubuntu22.04+VScode
基于libevent事件库实现的FTP服务端<br>
主要实现了文件的上传、下载、删除、目录操作功能
## 设计思路
采用命令工厂模式，在工厂中注册了`PORT,RETR,STOR`等多个指令，这些指令的处理方式分别由`XFtpTask`的不同子类完成，运行时初始化多个线程并监听，在接收到客户端的连接后，创建任务放到空闲进程中进行处理<br><br>
![image](https://github.com/LLKI/FTP-server/assets/92627079/236867c6-91d6-4364-b1d7-e442783877b3)

**项目结构：**
   - **XFtpFactory：** 工厂模式的实现，用于创建不同类型的 FTP 任务对象。
   - **XFtpTask：** FTP 任务的基类，定义了一些基本的 FTP 操作。
   - **XFtpServerCMD：** 主要处理命令通道，包括用户认证、命令解析和分发等。
   - **XFtpRETR 和 XFtpSTOR：** 分别处理文件的下载和上传。
   - **XFtpPORT：** 处理主动模式下的数据通道建立。
   - **XFtpLIST：** 处理 LIST、PWD、CWD、CDUP 等命令。
     
# FTP-client
平台：Windows10+QT6
基于QT开发的简易FTP客户端，用于连接服务端，展示文件列表信息，进行文件的上传、下载、删除功能
## 设计思路
通过`ftblib.h`库，连接服务器，发送文件上传及下载等操作的指令，界面主要展示当前文件目录下的文件信息，通过指令`ls -lh`获取
![image](https://github.com/LLKI/FTP-server/assets/92627079/5002b2ab-b507-4b03-8df1-2b964631df4a)


