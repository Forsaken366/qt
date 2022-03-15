#ifndef ANTENNA2_H
#define ANTENNA2_H

#include <QWidget>

namespace Ui {
class Antenna2;
}

class Antenna2 : public QWidget
{
    Q_OBJECT

public:
    explicit Antenna2(QWidget *parent = nullptr);
    ~Antenna2();

private:
    Ui::Antenna2 *ui;
};

#endif // ANTENNA2_H
