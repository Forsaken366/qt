#include "guiinstrumentlog.h"
#include "ui_guiinstrumentlog.h"
#include "QDateTime"
#include "QScrollBar"
GUIInstrumentLog::GUIInstrumentLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GUIInstrumentLog)
{
    ui->setupUi(this);
    //隐藏地址栏 和 信息栏
    ui->address->hide();
    ui->infobar->hide();
    //初始化获取数据
    QList<QStringList> data = initialization();
    //根据数据布局 总页面垂直布局
    QVBoxLayout *playout = new QVBoxLayout();
    for(int i = 0; i < data.length(); i++){
        //每行水平布局
        QHBoxLayout *slayout = new QHBoxLayout();
        //创建一个窗口
        QWidget *option = new QWidget();
        //窗口中布局 图标 和 文本信息
        QLabel *icon = new QLabel();
        QLabel *info = new QLabel();
        //图标路径 分别为 错误 告警 信息 0/1/2
        QString iconaddress[] = {":/image/icon/wrong.png",
                                 ":/image/icon/warning.png",
                                 ":/image/icon/info.png"};
        //设置图标
        icon->setPixmap(QPixmap(iconaddress[data[i][0].toInt()]));
        //设置图标大小
        icon->setMaximumWidth(40);
        //设置文本信息
        info->setText(data[i][1] + ": " + data[i][2]);
        //设置字体大小
        info->setStyleSheet("font-size:15px;font-family:SimHei;");
        //允许换行显示
        info->setWordWrap(true);
        //布局添加 图标 和 文本信息
        slayout->addWidget(icon);
        slayout->addWidget(info);
        //设置每一行大小
        option->setMinimumHeight(32);
        option->setMaximumHeight(32);
        option->setMinimumWidth(440);
        option->setMinimumWidth(440);
        //布局设置
        slayout->setContentsMargins(0, 0, 0, 0);
        //应用布局
        option->setLayout(slayout);
        //总布局中添加布局
        playout->addWidget(option);
    }
    //初始化滚轮位置
    ui->logs->verticalScrollBar()->setSliderPosition(0);
    //布局中添加弹簧
    playout->addStretch();
    //设置滚动页面大小
    ui->contents->setMinimumHeight(data.length() * 40);
    ui->logs->horizontalScrollBar()->hide();
    //滚动页面应用布局
    ui->contents->setLayout(playout);
    ui->contents->layout()->setContentsMargins(0, 0, 0, 0);
}

GUIInstrumentLog::~GUIInstrumentLog()
{
    delete ui;
}

void GUIInstrumentLog::on_quit_clicked()
{
    //发出退出信号 退回设置主页面
    emit quit();
}

/**
 * 初始化获取数据
 * @return 返回值为QList<QStringList> 每个QStringList代表每个错误/告警/信息所含信息
 * 格式为 标志(错误/告警/信息)-时间-信息
 * 其中 错误 告警 信息 分别标志为 0 1 2
 */
QList<QStringList> GUIInstrumentLog::initialization(){
    //截屏信息前缀文本 后面+保存位置获取
    QString address = "/media/SD Card/ScreenShots/";
    QString screenshot = "截屏已保存 to";
    //错误信息
    QString wronginfo = "GNSS error;Antenna line open";
    //设定种子
    srand(QDateTime::currentDateTime().toTime_t());
    //返回数据
    QList<QStringList> Data;
    for(int i = 0; i < 100; i++){
        //单行信息
        QStringList singleinfo;
        //随机生成时间
        int timeBegin,timeNow,t;
        QString strtime;
        QDateTime time=QDateTime::currentDateTime();//获取当前时间
        QDateTime timeFinal;//种子
        timeNow=time.toTime_t();//时间转换为时间戳
        timeBegin=1640000000;//https://tool.lu/timestamp/这个时间戳转换网站 这边默认开始时间固定，也可以随时指定，加在函数变量中即可
        t=rand()%(timeNow-timeBegin); //获取一个随机数
        t=timeNow-t;//当前时间减去随机数得到目标随机时间的时间戳
        timeFinal=QDateTime::fromTime_t(t);//时间戳转为时间
        strtime=timeFinal.toString("yyyy-MM-dd hh:mm:ss");//时间输出的格式化
        //随机生成图标 所代表的number
        QString icon;
        icon = QString::number(rand()%3);
        //列表插入 图标 时间
        singleinfo.append(icon);
        singleinfo.append(strtime);
        //根据图标生成相应信息
        switch (icon.toInt()) {
            //错误
            case 0:
                singleinfo.append(wronginfo);
                break;
            //告警
            case 1:
                singleinfo.append("警告！有语法错误");
                break;
            //信息
            case 2:
                singleinfo.append(screenshot + address + "screeshot-" + time.toString("yyyy-MM-dd-hh-mm-ss-000") + ".png");
                break;
            default:
                singleinfo.append("未知信息");
        }
        Data.append(singleinfo);
    }
    return Data;
}

// 按下导出键
void GUIInstrumentLog::on_leadout_clicked()
{
    //页面置为0
    page = 0;
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
    //滚动页面设置垂直布局
    QVBoxLayout* pLayout = new QVBoxLayout();
    //获取根目录磁盘信息
    QList<QStringList> drivers = getdiskinfos();
    //初始化数据
    this->num_folder = drivers.length();
    this->folder_directed = 0;
    this->info_folder = drivers;
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
        //如果是第一栏 则设置背景 + 金框
        if (i == 0)
        {
            SelectBox->setStyleSheet("QWidget{color:white;font:'黑体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                     "QLabel{border:0px;}");
        }

        //文本左对齐 文字大小15px
        else
            SelectBox->setStyleSheet("text-align:left;font-size:15px;font:'黑体'");
        pLayout->addWidget(SelectBox);
    }
    //初始化滚轮位置
    ui->logs->verticalScrollBar()->setSliderPosition(0);
    //布局中添加弹簧
    pLayout->addStretch();
    //设置滚动页面大小
    ui->contents->setMinimumHeight(drivers.length() * 40);
    //滚动页面应用布局
    ui->contents->setLayout(pLayout);
    ui->contents->layout()->setContentsMargins(0, 0, 0, 0);
}

// 按下方向键
void GUIInstrumentLog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Return:{
            //如果页面在根目录
            if(page == 0){
                //显示地址栏 和 信息栏
                ui->address->show();
                ui->infobar->show();
                //更改地址栏 和 信息栏信息
                //保存磁盘的使用信息
                QString memoryused = this->info_folder[this->folder_directed][3] + " of " + this->info_folder[this->folder_directed][1];
                ui->disksize->setText(memoryused);
                //地址栏设置水平布局
                QHBoxLayout *addlayout = new QHBoxLayout ();
                ui->address->setLayout(addlayout);
                //地址栏添加子窗口(文件夹蓝色图标-文件夹名字)
                addlayout->addWidget(infochildwindow(0, this->info_folder[this->folder_directed][0]));
                //地址栏添加弹簧
                addlayout->addStretch();
            }else {

            }
            //导出日志保存地址
            ui->exportaddress->setText("正保存日志至" + this->info_folder[this->folder_directed][0]);
            //根据数据布局 总页面垂直布局
            QVBoxLayout *playout = new QVBoxLayout();
            //获取新目录下的文件
            QList<QStringList> folders = getfileinfos(this->info_folder[this->folder_directed][2]);
            //初始化数据
            this->num_folder = folders.length();
            this->info_folder = folders;
            this->folder_directed = 0;
            //删除滚动页面上所有控件
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
            for (int i = 0; i < this->num_folder ; i ++) {
                //一个子窗口 4个控件 图标 文件夹名 文件所含文件数 修改时间
                QLabel *icon = new QLabel ();
                QLabel *name = new QLabel ();
                QLabel *items = new QLabel ();
                QLabel *changetime = new QLabel ();
                //设置
                icon->setPixmap(QPixmap(":/image/icon/folder.png"));
                name->setText(folders[i][0]);
                items->setText(folders[i][1]);
                changetime->setText(folders[i][2]);
                //设置水平布局 选项框
                QHBoxLayout* SelectBoxLayout = new QHBoxLayout();
                QWidget* SelectBox = new QWidget ();
                //设置大小
                SelectBox->setMaximumHeight(40);
                SelectBox->setMinimumHeight(40);
                //如果是第一栏 则设置背景 + 金框
                if (i == 0)
                {
                    SelectBox->setStyleSheet("QWidget{color:white;font:'黑体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                             "QLabel{border:0px;}");
                }

                //文本左对齐 文字大小15px
                else
                    SelectBox->setStyleSheet("text-align:left;font-size:15px;font:'黑体'");
                //添加控件
                SelectBoxLayout->addWidget(icon);
                SelectBoxLayout->addWidget(name);
                SelectBoxLayout->setContentsMargins(0, 0, 0, 0);
                SelectBox->setLayout(SelectBoxLayout);
                // 添加弹簧
                SelectBoxLayout->addStretch();
                SelectBoxLayout->addWidget(items);
                SelectBoxLayout->addWidget(changetime);

                playout->addWidget(SelectBox);
            }
            //初始化滚轮位置
            ui->logs->verticalScrollBar()->setSliderPosition(0);
            //布局中添加弹簧
            playout->addStretch();
            //设置滚动页面大小
            ui->contents->setMinimumHeight(this->num_folder * 40);
            //滚动页面应用布局
            ui->contents->setLayout(playout);
            ui->contents->layout()->setContentsMargins(0, 0, 0, 0);
            //页面++
            page++;
            break;
        }
        case Qt::Key_Up:{
            //获取所有子控件
            QList<QWidget*> List = ui->contents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
            if(this->folder_directed > 0){
                //背景重新设置为黑
                List[this->folder_directed--]->setStyleSheet("text-align:left;font-size:15px;font:'黑体'");
                //上一个背景设置为选中背景
                List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'黑体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                           "QLabel{border:0px;}");
            }else{
                //若指针在第一项则指向最后项
                //背景重新设置为黑
                List[0]->setStyleSheet("text-align:left;font-size:15px;font:'黑体'");
                this->folder_directed = this->num_folder - 1;
                //最后一个背景设置为选中背景
                List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'黑体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                           "QLabel{border:0px;}");
            }
            break;
        }
        case Qt::Key_Down:{
            //获取所有子控件
            QList<QWidget*> List = ui->contents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
            if(this->folder_directed < this->num_folder - 1){
                //背景重新设置为黑
                List[this->folder_directed++]->setStyleSheet("text-align:left;font-size:15px;font:'黑体'");
                //上一个背景设置为选中背景
                List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'黑体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                           "QLabel{border:0px;}");
            }else{
                //若指针在第一项则指向最后项
                //背景重新设置为黑
                List[this->folder_directed]->setStyleSheet("text-align:left;font-size:15px;font:'黑体'");
                this->folder_directed = 0;
                //最后一个背景设置为选中背景
                List[this->folder_directed]->setStyleSheet("QWidget{color:white;font:'黑体';text-align:left;font-size:15px;border:1px solid gold;margin-bottom:5px;}"
                                                           "QLabel{border:0px;}");
            }
            break;
        }
        default:
            qDebug() << event->key();
    }
}


// 获取下一级目录信息  文件名/文件数/修改时间
QList<QStringList> GUIInstrumentLog::getfileinfos(QString& ftp_dir){
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
QFileInfoList GUIInstrumentLog::getfilelist(QString& path){
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
QList<QStringList> GUIInstrumentLog::getdiskinfos(){
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
              if(storage.name().isEmpty())
                  RootFolderInfo.append(storage.rootPath());
              else
                  RootFolderInfo.append(storage.name());
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


/**
 * @brief GUIInstrumentLog::infochildwindow
 * @param model         模板号 模板0为初始模板 模板1+比模板0在前面多个→
 * @param foldername    文件夹名
 * @return              返回值为一个构造好的子窗口指针
 */
QWidget *GUIInstrumentLog::infochildwindow(int model, QString foldername){
    // 构造子窗口
    QWidget * child = new QWidget ();
    QLabel * icon = new QLabel ();
    QLabel * folder = new QLabel ();

    // 设置水平布局
    QHBoxLayout * playout = new QHBoxLayout ();
    child->setLayout(playout);

    // 将控件添加到布局中
    if(model){
        //模板号>0额外添加一个右箭头
        QLabel * right = new QLabel ();
        right->setPixmap(QPixmap(":/image/icon/right.png"));
        playout->addWidget(right);
    }
    playout->addWidget(icon);
    playout->addWidget(folder);

    // 设置属性
    icon->setPixmap(QPixmap(":/image/icon/pickedfolder.png"));
    folder->setText(foldername);
    playout->setContentsMargins(0, 0, 0, 0);

    // 返回子窗口
    return child;
}
