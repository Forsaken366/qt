#include "antenna.h"
#include "ui_antenna.h"

Antenna::Antenna(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Antenna)
{
    ui->setupUi(this);
}

Antenna::~Antenna()
{
    delete ui;
}
