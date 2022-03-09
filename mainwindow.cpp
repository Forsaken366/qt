#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLabel"
#include "QtDebug"
#include "QButtonGroup"
#include "QDialog"
#include "QLineEdit"
#include "QPalette"
#include "QPixmap"
#include "QFile"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化变量
    this->isopend_pagefile = false;

    //应用样式 apply the qss style
    QFile file(":/qss/main.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    //状态栏显示(使用的窗口)
    //状态栏图标修改
    QPixmap *IconVolume = new QPixmap(":/image/icon/volume.png");
    QPixmap *IconElectricity = new QPixmap(":/image/icon/electricity.png");
    ui->icon_volume->setPixmap(*IconVolume);
    ui->icon_electricity->setPixmap(*IconElectricity);

    //设置选项卡方向 在南方
    ui->tabWidget->setTabPosition(QTabWidget::South);

    //设置选项卡大小 平均分配
    int width = ui->tabWidget->width();
    int tabCount = ui->tabWidget->count();
    int tabWidth = width / tabCount;
    this->setStyleSheet(QString("QTabBar::tab{width:%1px;height:40px;}"
                                "QMainWindow{"
                                "background-color:#000000;"
                                "color:white;font:'宋体';}").arg(tabWidth));
    //空选项卡使能
    ui->tabWidget->setTabEnabled(4, false);
    //文件页面实例化
    pagefile = new PageFile(this);

    //信号连接 回到主设置页面
    connect(pagefile, SIGNAL(quit()), this, SLOT(BackToMainWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


//点击对话框 “修改设置” 后选项
void MainWindow::on_ChangeNetwork_clicked()
{
    //模态对话框
    QDialog *dig = new QDialog(this);
    dig->resize(440, 325);
    dig->setWindowTitle("网络设置");

    //界面设置
    dig->setStyleSheet("QWidget{"
                       "background-color:#000000;"
                       "color:white;font:'宋体';}"
                       "QPushButton{background-color:rgb(54,65,83);color:white;font:'宋体'}");


    //获取初始值
    QString ValueIP = ui->value_IP->text();
    QString ValueMAC = ui->value_MAC->text();
    QString ValueMASK = ui->value_MASK->text();
    QString ValueGateway = ui->value_gateway->text();
    QString ValuePort = ui->value_port->text();
    bool ButtonNet = ui->button_net->isChecked();

    //构建控件
    QLabel *LabelNetwakeup = new QLabel(this);
    QLabel *LabelIP = new QLabel(this);
    QLabel *LabelMASK = new QLabel(this);
    QLabel *LabelGateway = new QLabel(this);
    QLabel *LabelPort = new QLabel(this);

    QLineEdit *LineEditIP = new QLineEdit(this);
    QLineEdit *LineEditMASK = new QLineEdit(this);
    QLineEdit *LineEditGateway = new QLineEdit(this);
    QLineEdit *LineEditPort = new QLineEdit(this);

    QRadioButton * ButtonWakeup = new QRadioButton(this);

    QPushButton * ButtonSave = new QPushButton(this);
    QPushButton * ButtonCancel = new QPushButton(this);


    //设置文本
    LineEditIP->setPlaceholderText(ValueIP);
    LineEditMASK->setPlaceholderText(ValueMASK);
    LineEditGateway->setPlaceholderText(ValueGateway);
    LineEditPort->setPlaceholderText(ValuePort);

    LabelNetwakeup->setText("网络唤醒");
    LabelIP->setText("IP地址");
    LabelMASK->setText("子网掩码");
    LabelPort->setText("端口");
    LabelGateway->setText("网关");

    ButtonSave->setText("保存");
    ButtonCancel->setText("取消");

    ButtonWakeup->setChecked(ButtonNet);

    //嵌套栅格布局
    QGridLayout * UpLayout = new QGridLayout();
    //第一行 网络唤醒及按钮
    UpLayout->addWidget(LabelNetwakeup, 0, 0, 1, 1);
    UpLayout->addWidget(ButtonWakeup, 0, 1, 1, 1);

    //第二行 IP地址
    UpLayout->addWidget(LabelIP, 1, 0, 1, 1);
    UpLayout->addWidget(LineEditIP, 1, 1, 1, 1);

    //第三行 子网掩码
    UpLayout->addWidget(LabelMASK, 2, 0, 1, 1);
    UpLayout->addWidget(LineEditMASK, 2, 1, 1, 1);

    //第四行 网关
    UpLayout->addWidget(LabelGateway, 3, 0, 1, 1);
    UpLayout->addWidget(LineEditGateway, 3, 1, 1, 1);

    //第五行 端口
    UpLayout->addWidget(LabelPort, 4, 0, 1, 1);
    UpLayout->addWidget(LineEditPort, 4, 1, 1, 1);

    UpLayout->addWidget(ButtonSave, 5, 0, 1, 1);
    UpLayout->addWidget(ButtonCancel, 5, 1, 1, 1);

    //嵌套
    //上页面 设置等显示 setContentsMargins(int left, int top, int right, int bottom);
    QWidget * SettingWindow = new QWidget(this);
    UpLayout->setContentsMargins(10, 0, 10, 0);

    SettingWindow->setLayout(UpLayout);

    //下页面 保存和取消按钮
    QHBoxLayout * DownLayout = new QHBoxLayout();
    QWidget * DownWindow = new QWidget(this);

    //上色
    //DownWindow->setStyleSheet ("background-color: #000000;color: #ffffff;");
    DownWindow->setMinimumHeight(30);
    DownWindow->setMaximumHeight(30);
    DownLayout->addStretch();
    DownLayout->addWidget(ButtonSave);
    DownLayout->addWidget(ButtonCancel);
    DownLayout->setContentsMargins(10, 0, 10, 0);
    DownWindow->setLayout(DownLayout);

    //总页面
    QVBoxLayout * WindowLay = new QVBoxLayout();
    WindowLay->addWidget(SettingWindow);
    WindowLay->addWidget(DownWindow);
    WindowLay->setStretchFactor(SettingWindow, 4);
    WindowLay->setStretchFactor(DownWindow, 1);

    //保存按钮
    connect(ButtonSave, &QPushButton::clicked, [=](){
        //dig->setLayout(WindowLay);
        dig->reject();
        if(!LineEditIP->text().isEmpty()){
            ui->value_IP->setText(LineEditIP->text());
        }
        if(!LineEditMASK->text().isEmpty()){
            ui->value_MASK->setText(LineEditMASK->text());
        }
        if(!LineEditPort->text().isEmpty()){
            ui->value_port->setText(LineEditPort->text());
        }

        if(!LineEditGateway->text().isEmpty()){
            ui->value_gateway->setText(LineEditGateway->text());
        }

        ui->button_net->setChecked(ButtonWakeup->isChecked());
        qDebug() << "保存";
    });

    //取消按钮
    connect(ButtonCancel, &QPushButton::clicked, [=](){
        //dig->setLayout(WindowLay);
        dig->reject();
        qDebug() << "取消";
    });
    //setLayout(Layout);
    dig->setLayout(WindowLay);
    dig->exec();
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    //点击 "文件" 后
    if (index == 5){
        qDebug() << "打开文件";
        // 把tabWidget设置为隐藏
        ui->tabWidget->hide();
        //ui->status->hide();
        //构建控件
        //pagefile->move(250,250);
//        ui->centralWidget->setStyleSheet("QWidget{"
//                                         "background-color:#ffffff;}");
//        ui->status->setStyleSheet("QWidget{"
//                                  "background-color:#ffffff;}");
        ui->centralWidget->layout()->addWidget(pagefile);
        pagefile->show();
        // 设置焦点
        pagefile->setFocus();
        this->isopend_pagefile = true;
    }
}

//返回主页面
void MainWindow::BackToMainWindow(){
    if(this->isopend_pagefile){
        //重新显示标签页
        ui->tabWidget->show();
        //隐藏"文件"页
        pagefile->hide();
        //改变参数
        pagefile->setFocusPolicy(Qt::NoFocus);
        this->isopend_pagefile = false;
        //设置页重新设置为初始页即“仪表配置页”
        ui->tabWidget->setCurrentIndex(0);
    }
}

//打开天线列表
void MainWindow::on_antennalist_clicked()
{
    // 把tabWidget设置为隐藏
    ui->tabWidget->hide();
}
