#include "peripheralport.h"
#include "ui_peripheralport.h"

PeripheralPort::PeripheralPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeripheralPort)
{
    ui->setupUi(this);
    //设置自定义标题栏   对话框            无边框                    删除标题栏
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    //连接reject
    //取消按钮
    connect(ui->close, &QPushButton::clicked, [=](){
        //dig->setLayout(WindowLay);
        this->reject();
    });
}

PeripheralPort::~PeripheralPort()
{
    delete ui;
}
