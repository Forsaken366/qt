#ifndef PERIPHERALPORT_H
#define PERIPHERALPORT_H

#include <QDialog>

namespace Ui {
class PeripheralPort;
}

class PeripheralPort : public QDialog
{
    Q_OBJECT

public:
    explicit PeripheralPort(QWidget *parent = nullptr);
    ~PeripheralPort();

private slots:
    void on_close_clicked();

private:
    Ui::PeripheralPort *ui;
};

#endif // PERIPHERALPORT_H
