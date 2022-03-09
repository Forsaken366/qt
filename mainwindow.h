#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pagefile.h>
#include <antenna.h>

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
    bool isopend_pagefile;
    // 文件页
    PageFile *pagefile;

    // 天线列表页


private slots:
    void on_ChangeNetwork_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void BackToMainWindow();
    void on_antennalist_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
