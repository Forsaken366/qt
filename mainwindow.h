#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pagefile.h>
#include <antenna.h>
#include <peripheraloverview.h>
#include <peripheralport.h>
#include <guiinstrumentlog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool isopend_pagefile;  // 文件页是否打开
    bool isopend_antenna;   // 天线列表页是否打开
    bool isopend_periview;  // 外设概览页是否打开
    bool isopend_periport;  // 外设端口是否打开
    bool isopend_guilog;    // Gui和仪表日志是否代开

    PageFile *pagefile;             // 文件页
    Antenna *antenna;               // 天线列表页
    PeripheralOverview *periview;   // 外设概览页
    PeripheralPort *periport;       // 外设端口
    GUIInstrumentLog *guilog;       // Gui和仪表日志

private slots:
    void on_ChangeNetwork_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void BackToMainWindow();

    void on_antennalist_clicked();

    void on_periview_clicked();

    void on_periport_clicked();

    void on_showGuiInstrulog_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
