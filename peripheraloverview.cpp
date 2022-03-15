#include "peripheraloverview.h"
#include "ui_peripheraloverview.h"

PeripheralOverview::PeripheralOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeripheralOverview)
{
    ui->setupUi(this);

    //按键使能设置
    ui->button1->setEnabled(false);
    ui->button2->setEnabled(false);
    ui->button3->setEnabled(false);
    ui->button4->setEnabled(false);
}

PeripheralOverview::~PeripheralOverview()
{
    delete ui;
}

void PeripheralOverview::on_quit_clicked()
{
    //发出退出信号 退回设置主页面
    emit quit();
}

// 暂时设计为按下刷新键 显示3个外设
void PeripheralOverview::on_refresh_clicked()
{
    QStringList Name = {"Software", "COG@u-Blox LEA-M8T", "u-Blox LEA-M8T"};
    QStringList Type = {"Compass", "Compass", "GNSS"};
    QStringList Version = {"---", "---", "03.01"};
    QStringList Port = {"---", "14", "14"};
    QStringList Status = {"OK", "OK", "OK"};

    //设置总布局
    QVBoxLayout *playout = new QVBoxLayout();

    //遍历添加元素
    for(int i = 0; i < Name.length(); i++){
        //设置选项控件
        QWidget *option = new QWidget();
        QLabel *name = new QLabel(Name[i]);
        QLabel *type = new QLabel(Type[i]);
        QLabel *version = new QLabel(Version[i]);
        QLabel *port = new QLabel(Port[i]);
        QLabel *status = new QLabel(Status[i]);

        //添加布局
        QHBoxLayout *oplayout = new QHBoxLayout();
        oplayout->addWidget(name);
        oplayout->addWidget(type);
        oplayout->addWidget(version);
        oplayout->addWidget(port);
        oplayout->addWidget(status);

        //设置拉伸因子
        oplayout->setStretch(0, 4);
        oplayout->setStretch(1, 1);
        oplayout->setStretch(2, 1);
        oplayout->setStretch(3, 1);
        oplayout->setStretch(4, 1);

        //设置字体样式
        name->setStyleSheet("color:white;font-size:15px;");
        type->setStyleSheet("color:white;font-size:15px;");
        version->setStyleSheet("color:white;font-size:15px;");
        port->setStyleSheet("color:white;font-size:15px;");
        status->setStyleSheet("color:white;font-size:15px;");

        //设置控件大小
        option->setMinimumHeight(40);

        //设置布局
        option->setLayout(oplayout);

        //向总布局中添加元素
        playout->addWidget(option);
    }
    //布局中添加弹簧
    playout->addStretch();

    //设置滚动页面大小
    ui->contents->setMinimumHeight(Name.size() * 40);

    //设置总布局
    ui->contents->setLayout(playout);
    ui->contents->layout()->setContentsMargins(0, 0, 0, 0);


}
