#include <QCoreApplication>

#include "save_log.h"
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    //启动日志服务
    SaveLog::Instance()->Start();
    qDebug("debug");
    qWarning("warning");

    //停止日志服务
    SaveLog::Instance()->Stop();
    return a.exec();
}
