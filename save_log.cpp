#include "save_log.h"

#include <QDateTime>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>

#define QDATE qPrintable(QDate::currentDate().toString("yyyy-MM-dd"))

//日志重定向
void Log(QtMsgType type, const QMessageLogContext& context,
         const QString& msg) {
    //加锁，防止多线程中qDebug太频繁导致崩溃
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QString content;
    //根据不同的类型输出日志内容
    switch (type) {
    case QtDebugMsg:
        content = QString("[Debug]:in %1 line %2, %3")
                  .arg(context.file)
                  .arg(context.line)
                  .arg(msg);
        break;
    case QtWarningMsg:
        content = QString("[Warning]:in %1 line %2, %3")
                  .arg(context.file)
                  .arg(context.line)
                  .arg(msg);
        break;
    case QtCriticalMsg:
        content = QString("[Critical]:in %1 line %2, %3")
                  .arg(context.file)
                  .arg(context.line)
                  .arg(msg);
        break;
    case QtFatalMsg:
        content = QString("[Fatal]:in %1 line %2, %3")
                  .arg(context.file)
                  .arg(context.line)
                  .arg(msg);
        break;
    default:
        break;
    }
    SaveLog::Instance()->SaveToFile(content);
}

QScopedPointer<SaveLog> SaveLog::self;

//双检锁，保证单例线程安全，且比直接上锁提高执行效率
SaveLog* SaveLog::Instance() {
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new SaveLog);
        }
    }
    return self.data();
}

SaveLog::~SaveLog() {
    file_->close();
}
//安装日志钩子
void SaveLog::Start() {
    qInstallMessageHandler(Log);
}
//卸载日志钩子
void SaveLog::Stop() {
    qInstallMessageHandler(0);
}
//设置日志文件存放路径
void SaveLog::SetFilePath(const QString& path) {
    file_path_ = path;
}
//设置日志文件名称
void SaveLog::SetFileName(const QString& name) {
    file_name_ = name;
}
//输出日志到文件
void SaveLog::SaveToFile(const QString& content) {
    //输出日志,当日期改变时新建和打开文件
    QString whole_file_name = QString("%1/%2_log_%3.txt")
                              .arg(file_path_)
                              .arg(file_name_)
                              .arg(QDATE);
    if (whole_file_name_ != whole_file_name) {
        whole_file_name_ = whole_file_name;
        if (file_->isOpen()) {
            file_->close();
        }
        file_->setFileName(whole_file_name);
        file_->open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    }
    QTextStream log_stream(file_);
    log_stream << content << "\n";
}

SaveLog::SaveLog(QObject* parent) : QObject(parent) {
    file_ = new QFile(this);
    //默认取应用程序根目录
    file_path_ = qApp->applicationDirPath();
    //默认取应用程序可执行文件名称
    QString path_str = qApp->applicationFilePath();
    QStringList list = path_str.split("/");
    file_name_ = list.at(list.count() - 1).split(".").at(0);
    whole_file_name_ = "";
}
