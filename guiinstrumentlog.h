#ifndef GUIINSTRUMENTLOG_H
#define GUIINSTRUMENTLOG_H

#include <QWidget>
#include "QFileInfoList"
#include "QStorageInfo"
#include "QKeyEvent"
#include "QtDebug"

namespace Ui {
class GUIInstrumentLog;
}

class GUIInstrumentLog : public QWidget
{
    Q_OBJECT

public:
    explicit GUIInstrumentLog(QWidget *parent = nullptr);
    ~GUIInstrumentLog();

signals:
    //退出信号
    void quit();

private slots:
    void on_quit_clicked();

    void on_leadout_clicked();

protected:
    int page;   // 导出页面目录下所在的地址 0为根目录 +1下一级目录
    int folder_directed = 0;    // 光标所指向的文件
    int num_folder = 0;     // 文件数量

    void keyPressEvent(QKeyEvent *event);   // 按下方向键

    QFileInfoList getfilelist(QString& path);   // 获取某一目录下文件列表

    QList<QStringList> initialization();    // 初始化
    QList<QStringList> getfileinfos(QString& ftp_dir);  // 获取某一目录下的文件信息
    QList<QStringList> getdiskinfos();  // 获取根目录文件信息
    QList<QStringList> info_folder; // 文件信息 根目录:文件名/大小 子目录下:文件名/文件数/修改时间

    QWidget* infochildwindow(int model, QString foldername);    // 信息栏子窗口模板

private:
    Ui::GUIInstrumentLog *ui;
};

#endif // GUIINSTRUMENTLOG_H
