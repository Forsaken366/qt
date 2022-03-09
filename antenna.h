#ifndef ANTENNA_H
#define ANTENNA_H

#include <QWidget>

namespace Ui {
class Antenna;
}

class Antenna : public QWidget
{
    Q_OBJECT

public:
    explicit Antenna(QWidget *parent = nullptr);
    ~Antenna();

private:
    Ui::Antenna *ui;
};

#endif // ANTENNA_H
