#include "antenna2.h"
#include "ui_antenna2.h"

Antenna2::Antenna2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Antenna2)
{
    ui->setupUi(this);
}

Antenna2::~Antenna2()
{
    delete ui;
}
