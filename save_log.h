/*
 * 名称：输出日志文件类
 * 创建者：Lorewalker-chen
 * 创建日期：2020-11-24
 * 描述：接管qt应用程序中qDebug()、qWarning()等函数，输出至日志文件
 * 使用方式：将头文件加入到工程，
 *         使用Start()函数启动日志服务，
 *         使用Stop()函数停止日志服务。
 * 备注：在Windows10,Ubuntu16.04环境下，qt5.14.2版本测试可用。
 *      release下默认舍弃QMessageLogContext类的文件信息和行数，
 *      可在.pro中加入DEFINES += QT_MESSAGELOGCONTEXT开启。
 *      qt4下无QMessageLogContext类，无法定位到文件和行数。
 */

#ifndef SAVELOG_H
#define SAVELOG_H

#include <QObject>

class QFile;
class SaveLog : public QObject {
    Q_OBJECT
  public:
    static SaveLog* Instance();//返回自己的唯一实例
    ~SaveLog();

  public slots:
    void Start();//启动日志服务
    void Stop();//停止日志服务
    void SetFilePath(const QString& path);//设置日志文件存放路径
    void SetFileName(const QString& name);//设置日志文件名
    void SaveToFile(const QString& content);//输出日志到文件

  private:
    explicit SaveLog(QObject* parent = nullptr);

    static QScopedPointer<SaveLog> self;
    QFile* file_;
    QString file_path_;
    QString file_name_;
    QString whole_file_name_;//包含路径的完整名称
};

#endif // SAVELOG_H
