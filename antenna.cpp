#include "antenna.h"
#include "ui_antenna.h"
#include <QHeaderView>
#include "qdebug.h"
#include "QScrollBar"
Antenna::Antenna(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Antenna)
{
    ui->setupUi(this);
    //展示页面隐藏状态栏
    ui->status->hide();
    //按钮使能
    ui->edit->setEnabled(false);
    ui->remove->setEnabled(false);
    ui->block->setEnabled(false);
    //初始化页面
    this->page = 0;
}

Antenna::~Antenna()
{
    delete ui;
}

// 按下添加天线按钮
void Antenna::on_add_clicked()
{
    if(page == 0){
        //页面自增
        page++;
        //显示步骤栏
        ui->status->show();
        //标题切换名字
        ui->name->setText("名称");
        ui->usedbegin->setText("起始频率");
        ui->usedend->setText("终止频率");
        ui->anttype->hide();
        //选项栏
        QList<QStringList> options;
        QStringList Name = {"未定义的天线模块", "HALFWAVE DIPOL", "HE200 0.009-30MHz", "HE200 20-200MHz", "HE200 200-500MHz", "HE200 500-3000MHz", "HE300 0.009-30MHz", "HE300 20-200MHz"};
        QStringList beginfre = {"", "8 kHz", "8 kHz", "20 MHz", "200 MHz", "500 MHz", "8 kHz", "20 MHz"};
        QStringList endfre = {"", "26.5 GHz", "30 MHz", "200 MHz", "500 MHz", "3 GHz", "30 MHz", "200 MHz"};
        //qDebug() << "G" << beginfre.size();
        for(int i = 0; i < beginfre.size(); i++){
            //qDebug() << i << Name[i];
            QStringList option;
            option.append(Name[i]);
            option.append(beginfre[i]);
            option.append(endfre[i]);
            options.append(option);
        }

        //制造假数据
        for(int i = 0; i < beginfre.length(); i++){
            QStringList option;
            option.append(Name[rand()%7]);
            option.append(beginfre[rand()%7]);
            option.append(endfre[rand()%7]);
            options.append(option);
        }
        //初始化数据
        this->sum = options.size();
        this->directed = 0;
        //设置总垂直布局
        QVBoxLayout *playout = new QVBoxLayout();
        //页面填充选项
        for(int i = 0; i < options.size(); i ++){
            QWidget *option = new QWidget();
            QLabel *name = new QLabel();
            QLabel *frebegin = new QLabel();
            QLabel *freend = new QLabel();
            //水平布局
            QHBoxLayout *childlayout = new QHBoxLayout();
            //设置扩展值
            name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
            //添加控件
            childlayout->addWidget(name);
            childlayout->addWidget(frebegin);
            childlayout->addWidget(freend);
            //设置拉伸因子
            childlayout->setStretch(0, 4);
            childlayout->setStretch(1, 1);
            childlayout->setStretch(2, 1);
            //设置文本
            name->setText(options[i][0]);
            frebegin->setText(options[i][1]);
            freend->setText(options[i][1]);

            //设置最小高度及样式
            option->setMinimumHeight(40);
            if(i == 0){
                //第一个设置选择框
                option->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:1px solid gold;}"
                                      "QLabel{border-left:0px;border-right:0px;}");
                name->setStyleSheet("QLabel{border-left:1px solid gold}");
                freend->setStyleSheet("QLabel{border-right:1px solid gold;}");
            }else{
                option->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;}");
                freend->setStyleSheet("QLabel{border:0px;}");
                name->setStyleSheet("QLabel{}");
            }
            //设置布局
            option->setLayout(childlayout);
            //去除边界
            option->layout()->setContentsMargins(0, 0, 0, 0);
            //添加到总布局中
            playout->addWidget(option);
        }
        //布局中添加弹簧
        playout->addStretch();
        //设置滚动页面大小
        ui->contents->setMinimumHeight(options.size() * 40);
        //设置总布局
        ui->contents->setLayout(playout);
        ui->contents->layout()->setContentsMargins(0, 0, 0, 0);
        //更改按钮值
        ui->add->setText("取消");
        ui->edit->setText("");
        ui->remove->setText("");
        ui->reflesh->setText("");
        ui->quit->setText("下一个");
        //取消按钮重设样式
        ui->add->setStyleSheet("QPushButton{background-color:rgb(50, 63, 72);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle.png);background-repeat:no-repeat;background-position:top right;}");
        ui->edit->setStyleSheet("QPushButton{background-color:rgb(77, 77, 77);font: 14pt 微软雅黑;}");
        ui->quit->setStyleSheet("QPushButton{background-color:rgb(50, 63, 72);font: 14pt 微软雅黑;}");
        //刷新按钮使能no
        ui->reflesh->setEnabled(false);
        ui->edit->setEnabled(false);
        //滚动位置归0
        ui->options->verticalScrollBar()->setSliderPosition(0);
    }else if(page == 1){
        //退回到page 0页面
        page--;
        //更改按钮值
        ui->add->setText("添加");
        ui->edit->setText("编辑");
        ui->remove->setText("移除");
        ui->reflesh->setText("刷新");
        ui->quit->setText("退出");
        //按钮使能
        ui->remove->setEnabled(false);
        ui->block->setEnabled(false);
        //按钮样式设置
        ui->add->setStyleSheet("QPushButton{background-color:rgb(50, 63, 72);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle2.png);background-repeat:no-repeat;background-position:bottom right;}");
        ui->edit->setStyleSheet("QPushButton{background-color:rgb(77, 77, 77);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle2.png);background-repeat:no-repeat;background-position:bottom right;}");
        ui->quit->setStyleSheet("QPushButton{background-color:rgb(50, 63, 72);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle.png);background-repeat:no-repeat;background-position:top right;}");
        //获取当前页面子元素并删除
        //获取所有子控件
        QList<QWidget*> List = ui->contents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        //删除滚动页面上的控件
        while (ui->contents->layout()->count()) {
            QWidget *pWidget=ui->contents->layout()->itemAt(0)->widget();
            if (pWidget)
            {
                pWidget->setParent (nullptr);
                ui->contents->layout()->removeWidget(pWidget);
                delete pWidget;
            }
            else{
                QLayout *pLayout=ui->contents->layout()->itemAt(0)->layout();
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
                ui->contents->layout()->removeItem(ui->contents->layout()->itemAt(0));
            }
        }
        delete ui->contents->layout();
        //展示页面隐藏状态栏
        ui->status->hide();
        //标题切换名字
        ui->name->setText("Name");
        ui->anttype->show();
        ui->usedbegin->setText("Used Begin");
        ui->usedend->setText("Used End");
        //滚动位置归0
        ui->options->verticalScrollBar()->setSliderPosition(0);
        //滚动页面大小暂时设置为0
        ui->contents->setMinimumHeight(0);
    }else if(page == 2){
        //页数-1
        page--;
        //删除页面2
        delete antenna2;
        //重新显示滚轮页面
        ui->options->show();
        ui->header->show();
        //滚动页面大小暂时设置为0
        ui->contents->setMinimumHeight(0);
        //更改按钮名
        ui->quit->setText("下一个");
        //重新设置步骤2样式
        ui->step2->setStyleSheet("QLabel{background-image:url(:/image/icon/unprogress.png);"
                                 "background-repeat:no-repeat;"
                                 "background-position:center;}");
    }

}

//按下下一个/退出/完成
void Antenna::on_quit_clicked()
{
    if(page == 0){
        //发出退出信号 退回设置主页面
        emit quit();
    }else if (page == 1) {
        //按下下一个按钮 进入下一个页面
        page ++;
        //实例化页面2
        antenna2 = new Antenna2();
        ui->childwindow->layout()->addWidget(antenna2);
        ui->header->hide();
        //滚轮页面暂时隐藏
        ui->options->hide();
        //状态栏进度条改变
        ui->step2->setStyleSheet("QLabel{background-image:url(:/image/icon/progress.png);"
                                 "background-repeat:no-repeat;"
                                 "background-position:center;}");
        //退出按钮变为完成
        ui->quit->setText("完成");
    }else if (page == 2){
        //完成按钮
        page = 0;
        //更改按钮值
        ui->add->setText("添加");
        ui->edit->setText("编辑");
        ui->remove->setText("移除");
        ui->reflesh->setText("刷新");
        ui->quit->setText("退出");
        //按钮使能
        ui->remove->setEnabled(false);
        ui->block->setEnabled(false);
        //按钮样式设置
        ui->add->setStyleSheet("QPushButton{background-color:rgb(50, 63, 72);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle2.png);background-repeat:no-repeat;background-position:bottom right;}");
        ui->edit->setStyleSheet("QPushButton{background-color:rgb(77, 77, 77);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle2.png);background-repeat:no-repeat;background-position:bottom right;}");
        ui->quit->setStyleSheet("QPushButton{background-color:rgb(50, 63, 72);font: 14pt 微软雅黑;background-image:url(:/image/icon/triangle.png);background-repeat:no-repeat;background-position:top right;}");
        //获取当前滚轮页面子元素并删除
        //获取所有子控件
        QList<QWidget*> List = ui->contents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        //删除滚动页面上的控件
        while (ui->contents->layout()->count()) {
            QWidget *pWidget=ui->contents->layout()->itemAt(0)->widget();
            if (pWidget)
            {
                pWidget->setParent (nullptr);
                ui->contents->layout()->removeWidget(pWidget);
                delete pWidget;
            }
            else{
                QLayout *pLayout=ui->contents->layout()->itemAt(0)->layout();
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
                ui->contents->layout()->removeItem(ui->contents->layout()->itemAt(0));
            }
        }
        delete ui->contents->layout();
        //展示页面隐藏状态栏
        ui->status->hide();
        //标题切换名字
        ui->name->setText("Name");
        ui->anttype->show();
        ui->usedbegin->setText("Used Begin");
        ui->usedend->setText("Used End");
        //滚动位置归0
        ui->options->verticalScrollBar()->setSliderPosition(0);
        //页面2释放掉
        delete antenna2;
        //显示滚轮页面
        ui->options->show();
        ui->header->show();
        //滚动页面大小暂时设置为0
        ui->contents->setMinimumHeight(0);
    }
}

//键盘按键
void Antenna::keyPressEvent(QKeyEvent *event){
    switch (event->key()){
    // 按上下键时选择
    case Qt::Key_Up:{
        //获取所有子控件
        QList<QWidget*> List = ui->contents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        //如果指向第一项 则指向最后一项
        if(this->directed == 0){
            this->directed = this->sum - 1;
            //旧项取消背景
            List[0]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:0px;}");
            List[0]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border:0px;}");
            List[0]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border:0px;}");
            //新项添加背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:1px solid gold;}"
                                                "QLabel{border-left:0px;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border-left:1px solid gold;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border-right:1px solid gold;border-left:0px;}");
        }else{
            //旧项取消背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border:0px;}");
            List[this->directed--]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border:0px;}");
            //新项添加背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:1px solid gold;}"
                                                  "QLabel{border-left:0px;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border-left:1px solid gold;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border-right:1px solid gold;border-left:0px;}");
        }
        //判断是否超出边界 如果超出了则设置滚动条位置
        //获取当前滚动条相对值
        int placeofbar = ui->options->verticalScrollBar()->value();
        if(this->directed * 40 + 40 > ui->options->height() + placeofbar || this->directed * 40 < placeofbar){
            if((this->directed - 1) * 40 < 0){
                ui->options->verticalScrollBar()->setSliderPosition(0);
            }else
                ui->options->verticalScrollBar()->setSliderPosition((this->directed - 3) * 40);
        }
        break;
    }

    case Qt::Key_Down:{
        //获取所有子控件
        QList<QWidget*> List = ui->contents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        //如果指向最后一项 则指向第一项
        //qDebug() << "G" << List[0]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        if(this->directed == this->sum - 1){
            //旧项取消背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border:0px;}");
            this->directed = 0;
            //新项添加背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:1px solid gold;}"
                                                  "QLabel{border-left:0px;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border-left:1px solid gold;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border-right:1px solid gold;border-left:0px;}");
        }else{
            //旧项取消背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border:0px;}");
            List[this->directed++]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border:0px;}");
            //新项添加背景
            List[this->directed]->setStyleSheet("QWidget{color:white;font:'微软雅黑';text-align:left;font-size:15px;border:1px solid gold;}"
                                                  "QLabel{border-left:0px;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[0]->setStyleSheet("QLabel{border-left:1px solid gold;border-right:0px;}");
            List[this->directed]->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[2]->setStyleSheet("QLabel{border-right:1px solid gold;border-left:0px;}");
        }
        //判断是否超出边界 如果超出了则设置滚动条位置
        //获取当前滚动条相对值
        int placeofbar = ui->options->verticalScrollBar()->value();
        if(this->directed * 40 + 40 > ui->options->height() + placeofbar || this->directed * 40 < placeofbar){
            if((this->directed - 1) * 40 < 0){
                ui->options->verticalScrollBar()->setSliderPosition(0);
            }else
                ui->options->verticalScrollBar()->setSliderPosition((this->directed - 3) * 40);
        }
        break;
    }
    default:
        //qDebug() << event->key();
        break;
    }
}
