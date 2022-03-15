#ifndef PERIPHERALOVERVIEW_H
#define PERIPHERALOVERVIEW_H

#include <QWidget>

namespace Ui {
class PeripheralOverview;
}

class PeripheralOverview : public QWidget
{
    Q_OBJECT

public:
    explicit PeripheralOverview(QWidget *parent = nullptr);
    ~PeripheralOverview();

signals:
    //退出信号
    void quit();

private slots:
    void on_quit_clicked();

    void on_refresh_clicked();

private:
    Ui::PeripheralOverview *ui;
};

#endif // PERIPHERALOVERVIEW_H
