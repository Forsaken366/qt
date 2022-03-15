#ifndef ANTENNA_H
#define ANTENNA_H

#include <QWidget>
#include "QKeyEvent"
#include "antenna2.h"

namespace Ui {
class Antenna;
}

class Antenna : public QWidget
{
    Q_OBJECT

public:
    explicit Antenna(QWidget *parent = nullptr);
    ~Antenna();

private slots:
    void on_add_clicked();

    void on_quit_clicked();

signals:
    //退出信号
    void quit();

protected:
    // 按键
    void keyPressEvent(QKeyEvent *event);

    // 指针所指
    int directed;

    // 总数
    int sum;

    // 页数
    int page;

    // 第二页
    Antenna2 *antenna2;

private:
    Ui::Antenna *ui;
};

#endif // ANTENNA_H
