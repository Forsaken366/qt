#include "pagefile.h"
#include "ui_pagefile.h"
#include "QPixmap"
#include "QtDebug"
#include "QString"
#include "QKeyEvent"
#include "QList"
#include "mainwindow.h"
#include <QTime>
#include <QCheckBox>
#include "QStorageInfo"
#include "QScrollBar"
PageFile::PageFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PageFile)
{
    ui->setupUi(this);
    //自定义窗口标题栏|没有边框的窗口
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    //this->setParent(MainWindow);
    //图标
    QPixmap *IconPickedFolder = new QPixmap(":/image/icon/pickedfolder.png");
    //初始化界面
    ui->icon_pickedfolder->setPixmap(*IconPickedFolder);

    //页面1中的filepath标签隐藏
    ui->filepath->hide();

    //项目数内存使用暂时文本不显示
    ui->items->setText("");
    ui->memoryuse->setText("");

    //弹出等键暂时unable
    ui->popout->setEnabled(false);
    ui->createfolder->setEnabled(false);
    ui->rename->setEnabled(false);
    ui->showcontent->setEnabled(false);
    ui->showdriver->setEnabled(false);

    //页面1中滚动页面设置垂直布局
    QVBoxLayout* pLayout = new QVBoxLayout();
    //获取根目录文件信息
    QList<QStringList> drivers = getdiskinfos();

    //初始化数据
    this->num_folder = drivers.length();
    this->info_folder = drivers;
    this->folder_directed = 0;

    //添加查询到的文件
    for(int i = 0; i < drivers.length(); i++){
        //图标
        QLabel *icon_driver = new QLabel();
        //根目录下文件名字
        QLabel *name_driver = new QLabel();
        //根目录下文件信息-大小
        QLabel *info_capacity_driver = new QLabel();
        //设置图标
        icon_driver->setPixmap(QPixmap(":/image/icon/folder.png"));
        //设置页面元素 图标大小
        icon_driver->setMaximumWidth(30);
        //设置文本
        name_driver->setText(drivers[i][0]);
        //设置大小信息
        info_capacity_driver->setText(drivers[i][1]);
        //设置信息长度
        info_capacity_driver->setMinimumWidth(100);

        //设置水平布局 选项框
        QHBoxLayout* SelectBoxLayout = new QHBoxLayout();
        //添加元素
        SelectBoxLayout->addWidget(icon_driver);
        SelectBoxLayout->addWidget(name_driver);
        //添加弹簧
        SelectBoxLayout->addStretch();
        SelectBoxLayout->addWidget(info_capacity_driver);
        //设置布局
        QWidget* SelectBox = new QWidget();
        //设置大小
        SelectBox->setMaximumHeight(40);
        SelectBox->setMinimumHeight(40);
        SelectBox->setLayout(SelectBoxLayout);
        //如果是第一栏 则设置背景 + 下划线
        if (i == 0)
        {
            SelectBox->setStyleSheet("background-color: rgb(67,85,96);color:white;font:'宋体';"
                                  "text-align:left;font-size:15px;");
            name_driver->setStyleSheet("text-decoration: underline white;");
        }

        //文本左对齐 文字大小15px
        else
            SelectBox->setStyleSheet("text-align:left;font-size:15px;font:'宋体'");
        pLayout->addWidget(SelectBox);
    }
    //初始化滚轮位置
    ui->pathprefix->verticalScrollBar()->setSliderPosition(0);
    //布局中添加弹簧
    pLayout->addStretch();
    //设置滚动页面大小
    ui->files->setMinimumHeight(drivers.length() * 40);
    //滚动页面应用布局
    ui->files->setLayout(pLayout);
    ui->files->layout()->setContentsMargins(0, 0, 0, 0);
}

PageFile::~PageFile()
{
    delete ui;
}


void PageFile::on_quit_clicked()
{
    // 返回上一层目录
    if(this->page_status > 0){
        page_status --;
        QDir dir(this->address);
        dir.cdUp();
        //获取上一层目录路径
        QString uplevel = dir.absolutePath();
        //qDebug() << dir.absolutePath();
        QList<QStringList> folders;
        //删除滚动页面上的控件
        while (ui->files->layout()->count()) {
            QWidget *pWidget=ui->files->layout()->itemAt(0)->widget();
            if (pWidget)
            {
                pWidget->setParent (nullptr);
                ui->files->layout()->removeWidget(pWidget);
                delete pWidget;
            }
            else{
                QLayout *pLayout=ui->files->layout()->itemAt(0)->layout();
                if (pLayout)
                {
                    while(pLayout->count())
                    {
                        QWidget *pTempWidget=pLayout->itemAt(0)->widget();
                        if (pTempWidget)
                        {
                            pTempWidget->setParent (nullptr);

                            pLayout->removeWidget(pTempWidget);
                            delete pTempWidget;
                        }
                        else
                        {
                            pLayout->removeItem(pLayout->itemAt(0));
                        }
                    }
                }
                ui->files->layout()->removeItem(ui->files->layout()->itemAt(0));
            }
        }
        delete ui->files->layout();
        //重新设置垂直布局
        QVBoxLayout* pLayout = new QVBoxLayout();

        //如果回到磁盘页则使能 改名
        if(page_status == 0){
            //项目数内存使用暂时文本不显示
            ui->items->setText("");
            ui->memoryuse->setText("");

            //首先去除使能
            ui->popout->setEnabled(false);
            ui->createfolder->setEnabled(false);
            ui->rename->setEnabled(false);
            ui->showcontent->setEnabled(false);
            ui->showdriver->setEnabled(false);

            //更改文本
            ui->popout->setText("弹出");
            ui->createfolder->setText("");
            ui->rename->setText("");
            ui->showcontent->setText("");
            ui->showdriver->setText("");

            //隐藏标题栏
            ui->filepath->hide();
            //获取根目录文件信息
            QList<QStringList> drivers = getdiskinfos();

            //初始化数据
            this->num_folder = drivers.length();
            this->info_folder = drivers;
            this->folder_directed = 0;



            //添加查询到的文件
            for(int i = 0; i < drivers.length(); i++){
                //图标
                QLabel *icon_driver = new QLabel();
                //根目录下文件名字
                QLabel *name_driver = new QLabel();
                //根目录下文件信息-大小
                QLabel *info_capacity_driver = new QLabel();
                //设置图标
                icon_driver->setPixmap(QPixmap(":/image/icon/folder.png"));
                //设置页面元素 图标大小
                icon_driver->setMaximumWidth(30);
                //设置文本
                name_driver->setText(drivers[i][0]);
                //设置大小信息
                info_capacity_driver->setText(drivers[i][1]);
                //设置信息长度
                info_capacity_driver->setMinimumWidth(100);

                //设置水平布局 选项框
                QHBoxLayout* SelectBoxLayout = new QHBoxLayout();
                //添加元素
                SelectBoxLayout->addWidget(icon_driver);
                SelectBoxLayout->addWidget(name_driver);
                //添加弹簧
                SelectBoxLayout->addStretch();
                SelectBoxLayout->addWidget(info_capacity_driver);
                //设置布局
                QWidget* SelectBox = new QWidget();
                //设置大小
                SelectBox->setMaximumHeight(40);
                SelectBox->setMinimumHeight(40);
                SelectBox->setLayout(SelectBoxLayout);
                //如果是第一栏 则设置背景 + 下划线
                if (i == 0)
                {
                    SelectBox->setStyleSheet("background-color: rgb(67,85,96);color:white;font:'宋体';"
                                          "text-align:left;font-size:15px;");
                    name_driver->setStyleSheet("text-decoration: underline white;");
                }

                //文本左对齐 文字大小15px
                else
                    SelectBox->setStyleSheet("text-align:left;font-size:15px;font:'宋体'");
                pLayout->addWidget(SelectBox);
                //设置滚动页面大小
                ui->files->setMinimumHeight(drivers.length() * 40);
            }
        }else {
            //获取该路径的上一个路径所含项目数量
            //QDir lastDir(uplevel);
            //lastDir.cdUp();
            //QString lastpath = lastDir.path();
            QString lastitems = QString::number(getfilelist(uplevel).size()) + " items";
            ui->items->setText(lastitems);
            //获取新目录下的文件
            folders = getfileinfos(uplevel);
            //光标指向的文件重新设置为第一个
            this->folder_directed = 0;
            //记录文件路径
            this->address = uplevel;
            //页面1中的filepath标签显示并指向根目录
            ui->root->setText(this->address);
            //初始化数据
            this->num_folder = folders.length();
            this->info_folder = folders;
            this->folder_directed = 0;

            //设置滚动页面大小
            ui->files->setMinimumHeight(folders.length() * 40 + 10);
            for(int i = 0;i < folders.length(); i++){
                //勾选项
                QCheckBox *checked_folder = new QCheckBox ();
                //设置图标
                QLabel *icon_folder = new QLabel();
                icon_folder->setPixmap(QPixmap(":/image/icon/folder.png"));
                icon_folder->setMaximumWidth(30);
                //文件夹名字
                QLabel *name_folder = new QLabel();
                name_folder->setText(folders[i][0]);
                //文件夹含项目数
                QLabel *num_items = new QLabel();
                num_items->setText(folders[i][1]);
                //文件夹修改时间
                QLabel *changetime_folder = new QLabel();
                changetime_folder->setText(folders[i][2]);
                //设置水平布局 选项框
                QHBoxLayout* SelectBoxLayout = new QHBoxLayout();
                QWidget* SelectBox = new QWidget ();
                //设置大小
                SelectBox->setMaximumHeight(40);
                SelectBox->setMinimumHeight(40);
                //如果是第一栏 则设置金框
                if (i == 0)
                {
                    SelectBox->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                             "QLabel{border-left:0px;border-right:0px;}"
                                             "QCheckBox{border:0px;margin-left:10px;}");
                    changetime_folder->setStyleSheet("QLabel{border-right:1px solid gold}");
                }else{
                    SelectBox->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;}"
                                             "QCheckBox{border:0px;margin-left:10px;}");
                    changetime_folder->setStyleSheet("QLabel{margin-right:1px}");
                }
                //添加控件
                SelectBoxLayout->addWidget(checked_folder);
                SelectBoxLayout->addWidget(icon_folder);
                SelectBoxLayout->addWidget(name_folder);
                SelectBoxLayout->setContentsMargins(0, 0, 0, 0);
                SelectBox->setLayout(SelectBoxLayout);
                // 添加弹簧
                SelectBoxLayout->addStretch();
                SelectBoxLayout->addWidget(num_items);
                SelectBoxLayout->addWidget(changetime_folder);

                pLayout->addWidget(SelectBox);

            }

        }
        //初始化滚轮位置
        ui->pathprefix->verticalScrollBar()->setSliderPosition(0);
        //添加弹簧
        pLayout->addStretch();
        //滚动页面应用布局
        ui->files->setLayout(pLayout);
        ui->files->layout()->setContentsMargins(0, 0, 0, 0);

    }else{
        //发出退出信号 退回设置主页面
        emit quit();
        //qDebug() << "返回设置主页面";
    }
}


void PageFile::keyPressEvent(QKeyEvent *event)
{

    switch (event->key())
    {   // 按Enter键进入
        case Qt::Key_Enter:{
            //获取所有子控件
            QList<QWidget*> List = ui->files->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
            QString downlevel;
            //记录地址
            if (page_status == 0){
                //将要进入的下一级目录地址
                downlevel = this->info_folder[this->folder_directed][2];
                //选项更改为选择 创建文件夹 重命名 显示内容 显示驱动 退出
                //首先使能
                ui->popout->setEnabled(true);
                ui->createfolder->setEnabled(true);
                ui->rename->setEnabled(true);
                ui->showcontent->setEnabled(true);
                ui->showdriver->setEnabled(true);

                //更改文本
                ui->popout->setText("选择");
                ui->createfolder->setText("创建文件夹");
                ui->rename->setText("重命名");
                ui->showcontent->setText("显示内容");
                ui->showdriver->setText("显示驱动");

                //保存磁盘的使用信息
                QString memoryused = this->info_folder[this->folder_directed][3] + " of " + this->info_folder[this->folder_directed][1];
                ui->memoryuse->setText(memoryused);

                //文件数赋值
                ui->items->setText(this->info_folder[this->folder_directed][4]);
            }
            else{
                downlevel = this->info_folder[this->folder_directed][3];
                ui->items->setText(this->info_folder[this->folder_directed][1]);
            }

            //qDebug() << "downlevel:" << downlevel;
            //光标指向的文件重新设置为第一个
            this->folder_directed = 0;
            //获取新目录下的文件
            QList<QStringList> folders = getfileinfos(downlevel);
            //记录文件路径
            this->address = downlevel;
            //删除滚动页面上的控件
            while (ui->files->layout()->count()) {
                QWidget *pWidget=ui->files->layout()->itemAt(0)->widget();
                if (pWidget)
                {
                    pWidget->setParent (nullptr);
                    ui->files->layout()->removeWidget(pWidget);
                    delete pWidget;
                }
                else{
                    QLayout *pLayout=ui->files->layout()->itemAt(0)->layout();
                    if (pLayout)
                    {
                        while(pLayout->count())
                        {
                            QWidget *pTempWidget=pLayout->itemAt(0)->widget();
                            if (pTempWidget)
                            {
                                pTempWidget->setParent (nullptr);

                                pLayout->removeWidget(pTempWidget);
                                delete pTempWidget;
                            }
                            else
                            {
                                pLayout->removeItem(pLayout->itemAt(0));
                            }
                        }
                    }
                    ui->files->layout()->removeItem(ui->files->layout()->itemAt(0));
                }
            }
            delete ui->files->layout();
            //重新设置垂直布局
            QVBoxLayout* pLayout = new QVBoxLayout();
            //页面1中的filepath标签显示并指向根目录
            ui->filepath->show();
            ui->root->setText(this->address);
            //初始化数据
            this->num_folder = folders.length();
            this->info_folder = folders;
            this->folder_directed = 0;
            for(int i = 0;i < folders.length(); i++){
                //勾选项
                QCheckBox *checked_folder = new QCheckBox ();
                //设置图标
                QLabel *icon_folder = new QLabel();
                icon_folder->setPixmap(QPixmap(":/image/icon/folder.png"));
                icon_folder->setMaximumWidth(30);
                //文件夹名字
                QLabel *name_folder = new QLabel();
                name_folder->setText(folders[i][0]);
                //文件夹含项目数
                QLabel *num_items = new QLabel();
                num_items->setText(folders[i][1]);
                //文件夹修改时间
                QLabel *changetime_folder = new QLabel();
                changetime_folder->setText(folders[i][2]);
                //设置水平布局 选项框
                QHBoxLayout* SelectBoxLayout = new QHBoxLayout();
                QWidget* SelectBox = new QWidget ();
                //设置大小
                SelectBox->setMaximumHeight(40);
                SelectBox->setMinimumHeight(40);
                //如果是第一栏 则设置金框
                if (i == 0)
                {
                    SelectBox->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                             "QLabel{border-left:0px;border-right:0px;}"
                                             "QCheckBox{border:0px;margin-left:10px;}");
                    changetime_folder->setStyleSheet("QLabel{border-right:1px solid gold}");
                }else{
                    SelectBox->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;}"
                                             "QCheckBox{border:0px;margin-left:10px;}");
                    changetime_folder->setStyleSheet("QLabel{margin-right:1px}");
                }
                //添加控件
                SelectBoxLayout->addWidget(checked_folder);
                SelectBoxLayout->addWidget(icon_folder);
                SelectBoxLayout->addWidget(name_folder);
                SelectBoxLayout->setContentsMargins(0, 0, 0, 0);
                SelectBox->setLayout(SelectBoxLayout);
                // 添加弹簧
                SelectBoxLayout->addStretch();
                SelectBoxLayout->addWidget(num_items);
                SelectBoxLayout->addWidget(changetime_folder);

                pLayout->addWidget(SelectBox);

            }
            //初始化滚轮位置
            ui->pathprefix->verticalScrollBar()->setSliderPosition(0);
            //添加弹簧
            pLayout->addStretch();
            //滚动页面应用布局
            ui->files->setLayout(pLayout);
            ui->files->layout()->setContentsMargins(0, 0, 0, 0);
            //设置滚动页面大小
            ui->files->setMinimumHeight(folders.length() * 40);
            //页面记录++
            page_status++;
            break;
        }

        // 按上下键时选择
        case Qt::Key_Up:{
            //qDebug() <<"UP";
            if(page_status > 0){
                //获取所有子控件
                QList<QWidget*> List = ui->files->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
                if(this->folder_directed > 0){
                    //背景重新设置为黑
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");
                    List[this->folder_directed--]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{margin-right:1px}");
                    //上一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                               "QLabel{border-left:0px;border-right:0px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");

                    List[this->folder_directed]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{border-right:1px solid gold}");
                }else{
                    //若指针在第一项则指向最后项
                    //背景重新设置为黑
                    List[0]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");
                    List[0]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{margin-right:1px}");
                    this->folder_directed = this->num_folder - 1;
                    //最后一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                               "QLabel{border-left:0px;border-right:0px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");

                    List[this->folder_directed]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{border-right:1px solid gold}");
                }
            }else{
                //初始页面
                //获取所有子控件
                QList<QWidget*> List = ui->files->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
                if(this->folder_directed > 0){
                    //背景重新设置为黑
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(0,0,0);text-align:left;font-size:15px;font:'宋体'");
                    //字体取消下划线
                    List[this->folder_directed--]->findChildren<QLabel*>()[1]->setStyleSheet("text-align:left;font-size:15px;font:'宋体'");
                    //上一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(67,85,96);color:white;font:'宋体';"
                                                    "text-align:left;font-size:15px;");
                    // 字体加上下划线
                    List[this->folder_directed]->findChildren<QLabel*>()[1]->setStyleSheet("text-decoration: underline white;");
                }else{
                    //若指针在第一项则指向最后项
                    List[0]->setStyleSheet("background-color: rgb(0,0,0);text-align:left;font-size:15px;font:'宋体'");
                    //字体取消下划线
                    List[0]->findChildren<QLabel*>()[1]->setStyleSheet("text-align:left;font-size:15px;font:'宋体'");
                    this->folder_directed = this->num_folder - 1;
                    //最后一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(67,85,96);color:white;font:'宋体';"
                                                    "text-align:left;font-size:15px;");
                    //设置下划线
                    List[this->folder_directed]->findChildren<QLabel*>()[1]->setStyleSheet("text-decoration: underline white;");
                }
            }
            //判断是否超出边界 如果超出了则设置滚动条位置
            //获取当前滚动条相对值
            int placeofbar = ui->pathprefix->verticalScrollBar()->value();
            if(this->folder_directed * 40 + 40 > ui->pathprefix->height() + placeofbar || this->folder_directed * 40 < placeofbar){
                if((this->folder_directed - 1) * 40 < 0){
                    ui->pathprefix->verticalScrollBar()->setSliderPosition(0);
                }else
                    ui->pathprefix->verticalScrollBar()->setSliderPosition((this->folder_directed - 3) * 40);
            }
            break;
        }

        case Qt::Key_Down:{
            if(page_status > 0){
                //获取所有子控件
                QList<QWidget*> List = ui->files->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
                if(this->folder_directed < this->num_folder - 1){
                    //背景重新设置为黑
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");
                    List[this->folder_directed++]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{margin-right:1px}");
                    //上一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                               "QLabel{border-left:0px;border-right:0px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");

                    List[this->folder_directed]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{border-right:1px solid gold}");
                }else{
                    //若指针在第一项则指向最后项
                    //背景重新设置为黑
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");
                    List[this->folder_directed]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{margin-right:1px}");
                    this->folder_directed = 0;
                    //最后一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'宋体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                               "QLabel{border-left:0px;border-right:0px;}"
                                                               "QCheckBox{border:0px;margin-left:10px;}");

                    List[this->folder_directed]->findChildren<QLabel*>()[3]->setStyleSheet("QLabel{border-right:1px solid gold}");
                }
            }else{
                //获取所有子控件
                QList<QWidget*> List = ui->files->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
                //若指针在最后一项则指向第一项
                if(this->folder_directed < this->num_folder - 1){
                    //背景重新设置为黑
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(0,0,0);text-align:left;font-size:15px;font:'宋体'");
                    //字体取消下划线
                    List[this->folder_directed++]->findChildren<QLabel*>()[1]->setStyleSheet("text-align:left;font-size:15px;font:'宋体'");
                    //下一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(67,85,96);color:white;font:'宋体';"
                                                    "text-align:left;font-size:15px;");
                    // 字体加上下划线
                    List[this->folder_directed]->findChildren<QLabel*>()[1]->setStyleSheet("text-decoration: underline white;");
                }else{
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(0,0,0);text-align:left;font-size:15px;font:'宋体'");
                    //字体取消下划线
                    List[this->folder_directed]->findChildren<QLabel*>()[1]->setStyleSheet("text-align:left;font-size:15px;font:'宋体'");
                    this->folder_directed = 0;
                    //最后一个背景设置为选中背景
                    List[this->folder_directed]->setStyleSheet("background-color: rgb(67,85,96);color:white;font:'宋体';"
                                                    "text-align:left;font-size:15px;");
                    //设置下划线
                    List[this->folder_directed]->findChildren<QLabel*>()[1]->setStyleSheet("text-decoration: underline white;");
                }
            }
            //获取当前滚动条相对值
            int placeofbar = ui->pathprefix->verticalScrollBar()->value();
            //qDebug() << placeofbar << ui->files->height() << ui->pathprefix->height() << this->folder_directed * 40;
            if(this->folder_directed * 40  + 40 > ui->pathprefix->height() + placeofbar || this->folder_directed * 40 < placeofbar){
                if((this->folder_directed - 1) * 40 < 0){
                    ui->pathprefix->verticalScrollBar()->setSliderPosition(0);
                }else
                    ui->pathprefix->verticalScrollBar()->setSliderPosition((this->folder_directed - 3) * 40);
            }
            break;
        }
        default:
            //qDebug() << event->key();
            break;
    }
}

// 获取下一级目录信息  文件名/文件数/修改时间
QList<QStringList> PageFile::getfileinfos(QString& ftp_dir){
    QList<QStringList> Folderinfos;
    QDir dir(ftp_dir);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    //qDebug() << ftp_dir;
    QFileInfoList list_sub = getfilelist(ftp_dir);
    //qDebug() << list_sub.size();
    for (int i = 0; i < list_sub.size(); ++i) {
        QStringList FileInfo;
        QString filepath;
        QString full_path;
        QFileInfo fileInfo = list_sub.at(i);
        //qDebug() << fileInfo.path();
        filepath.append(fileInfo.path());
        // 消除第二页面 c:\\的bug
        if(this->page_status == 0){
            filepath += fileInfo.fileName();
        }else
            filepath += '/' + fileInfo.fileName();
        //qDebug() << page_status << filepath;
        if (!filepath.isEmpty()){
            QFileInfo info(filepath);
            if (info.exists())    {
                QString file_size = tr("%1").arg(info.size());
                QDateTime dt = info.birthTime();
                QString create_time = tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
                dt = info.lastModified();
                QString modify_time = tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
                QString file_id = tr("%1").arg(i);
                //文件夹名
                FileInfo.append(info.fileName());
                //判断是否为文件
                if(info.isFile()){
                    FileInfo.append("File");
                }else{
                // 查看该目录文件夹下是否还有文件 文件数
                    QFileInfoList list_child = getfilelist(filepath);
                    if(list_child.size()){
                        FileInfo.append(QString::number(list_child.size()) + " items");
                    }else{
                        FileInfo.append("empty");
                    }
                    //qDebug() << "文件名:" << info.fileName() << "文件数:" << list_child.size();
                }
                //修改时间
                FileInfo.append(modify_time);
                //文件路径
                FileInfo.append(filepath);
                //qDebug() << "文件名:" << info.fileName() << " 大小(Byte) :" << file_size << " 创建时间 : " << create_time << " 修改时间 : "<<modify_time  << "是否是文件夹" << info.isFile();
                Folderinfos.append(FileInfo);
            }
        }
    }
    return Folderinfos;
}

// 获取某一目录下的文件列表
QFileInfoList PageFile::getfilelist(QString& path){
    //qDebug() << path;
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i != folder_list.size(); i++) {
        //qDebug() << i;
        QString name = folder_list.at(i).absoluteFilePath();
        //qDebug() << name;
        file_list.append(name);
    }

    return file_list;
}


// 获取根目录文件信息
QList<QStringList> PageFile::getdiskinfos(){
    QList<QStringList> RootFolders;
    //获取本地磁盘信息
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
      if (storage.isValid() && storage.isReady()) {
          if (!storage.isReadOnly()) {
//              qDebug() << "";
//              qDebug() << "rootpath:"<< storage.rootPath();
//              qDebug() << "name:" << storage.name();
//              qDebug() << "displayname:" << storage.displayName();
//              qDebug() << "fileSystemType:" << storage.fileSystemType();
//              qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//              qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
              QStringList RootFolderInfo;
              //storage.
              //路径名构造
              RootFolderInfo.append(storage.name() + storage.rootPath());
              //内存总大小
              RootFolderInfo.append(QString::number(storage.bytesTotal()/1000/1000/1000) + "GB");
              //路径
              RootFolderInfo.append(storage.rootPath());
              //内存可用
              RootFolderInfo.append(QString::number(storage.bytesAvailable()/1000/1000/1000) + "GB");
              //文件数
              QString path = storage.rootPath();
              QFileInfoList list_sub = getfilelist(path);
              RootFolderInfo.append(QString::number(list_sub.size()) + " items");
              RootFolders.append(RootFolderInfo);

          }
       }
    }
    return RootFolders;
}

