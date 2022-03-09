#ifndef PAGEFILE_H
#define PAGEFILE_H

#include <QDialog>
#include "QKeyEvent"
#include "QObject"
#include "QFileInfoList"

namespace Ui {
class PageFile;
}

class PageFile : public QDialog
{
    Q_OBJECT

public:
    explicit PageFile(QWidget *parent = nullptr);

    ~PageFile();

private slots:

    void on_quit_clicked();
private:
    Ui::PageFile *ui;

signals:
    //退出信号
    void quit();

protected:
    // 按键
    void keyPressEvent(QKeyEvent *event);

    // 页面切换状态
    int page_status = 0;

    // 光标所指向的文件
    int folder_directed = 0;

    // 文件数量
    int num_folder = 0;

    // 文件路径
    QString address;

    // 文件信息 根目录:文件名/大小 子目录下:文件名/文件数/修改时间
    QList<QStringList> info_folder;

    // 获取某一目录下文件列表
    QFileInfoList getfilelist(QString& path);

    // 获取某一目录下的文件信息
    QList<QStringList> getfileinfos(QString& ftp_dir);

    // 获取根目录文件信息
    QList<QStringList> getdiskinfos();
};

#endif // PAGEFILE_H
