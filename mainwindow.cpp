#include "btbutton.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "table.h"
#include "free.h"
#include "jindutiaoa.h"
#include "head.h"

extern QStringList Name;
extern QStringList Path;
extern QStandardItemModel* model;
extern QMediaPlayer *player;
extern QMediaPlaylist *playlist;
extern QPushButton *swi;
extern QPushButton *start;
extern int Mode;
extern int Now;
extern int max;
extern QTimer *Time;
extern bool hand;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Now=0;
    srand(time(0));
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);

    SetUp();
    start=ui->start;
    swi=ui->switch_2;

    Reloading();
    if(Name.size()==0)
    {
        ui->jindutiao->setEnabled(false);
    }
    else
    {
        ui->jindutiao->setEnabled(true);

        ui->voice->setRange(0,100);
        ui->voice->setValue(100);
        Voice();
        ui->jindutiao->setRange(0,max);
    }

    Time->setInterval(1000);
    Time->start();

    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(GOGOGO()));

    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(ICON()));
    connect(Time,SIGNAL(timeout()),this,SLOT(Jindutiao()));//进度条位置更改

    connect(ui->voice,SIGNAL(valueChanged(int)),this,SLOT(Voice()));//音量

    connect(ui->jindutiao,SIGNAL(KOI()),this,SLOT(JindutiaoA()));//进度条改变

    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(StateC()));//改变开始暂停标志

    Super();

    if(Name.size()!=0)
    {
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
    }

    Butset();

    MIN();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Table::Table(QWidget *parent)
    : QTableView(parent)
{
    setAcceptDrops(true);

    mLabel =new QLabel(this);

    this->setAttribute(Qt::WA_TranslucentBackground,true);//背景透明

    mLabel->setFixedHeight(2);//改变拖动标志黑条属性
    mLabel->setGeometry(1, 0, width(), 1);
    mLabel->setStyleSheet("background-color:black;");
    mLabel->hide();

    mModel=NULL;
    RowHeight=30;
    ValidPress=false;
    RowFrom=0;
    RowTo=0;

    {//菜单
        menu=new QMenu;
        A=new QAction;
        TH->setContextMenuPolicy(Qt::CustomContextMenu);
        A->setText(QString("删除"));
        menu->addAction(A);

        connect(TH,
                SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(Rightmenu(QPoint)));

        connect(A,SIGNAL(triggered()),
                this,SLOT(Rocket()));

    }
}

void Table::SetModel(QStandardItemModel *model)//可以在table外操纵table内的数据
{
    mModel = model;
    QTableView::setModel(model);
}

void Table::Move(int from, int to)
{
    if(from==to)
    {
        return;
    }

    QStandardItem *item=mModel->item(from,2);
    if(item)
    {
        QString str=item->text();

        QList<QStandardItem*> items;
        QStandardItem* item0 = new QStandardItem("");//第一格为空格
        QStandardItem* item1 = new QStandardItem("");//第二格为空格
        QStandardItem* item2 = new QStandardItem(str);//第三格为原字符
        items.append(item0);
        items.append(item1);
        items.append(item2);

        Free::Switch(from,to);

        mModel->insertRow(to, items);//插入新行

        if (from < to)//删除旧行
        {
            mModel->removeRow(from);
        }
        else
        {
            mModel->removeRow(from+1);
        }


        if(from+1==to)
        {
            Free::Reloadnum();
            Free::ICON();
            return;
        }
        if(from==Now)
        {
            Now=to;
            if(to==Name.size())
            {
                Now--;
            }
        }
        else if(to==Now)
        {
            Now++;
        }
        else if((from>Now)&&(to<Now))
        {
            Now++;
        }
        else if((from<Now)&&(to>Now))
        {
            Now--;
        }
        else if(((from>Now)&&(to>Now))||((from<Now)&&(to<Now)))
        {

        }

        Free::Reloadnum();
        Free::ICON();
    }
}

void MainWindow::SetUp()
{
    t=ui->tableView;

    t->setStyleSheet("background-color:rgba(0,0,0,0)");
    t->horizontalHeader()->hide();    //隐藏列头(带列头的情况需要修改代码，重新计算高度偏移)
    t->verticalHeader()->hide();  //隐藏行头
    t->verticalHeader()->setDefaultSectionSize(30);         //默认列高度
    t->horizontalHeader()->setStretchLastSection(true);     //最后一列自适应宽度
    t->setEditTriggers(QTableView::NoEditTriggers);          //不能编辑
    t->setSelectionBehavior(QTableView::SelectRows);     //一次选中整行
    t->setSelectionMode(QTableView::SingleSelection);       //单行选中
    t->setShowGrid(false);                                  //去掉网格线
    t->setFocusPolicy(Qt::NoFocus);                         //去掉item选中时虚线框

    model = new QStandardItemModel();

    t->SetModel(model);     //可以在table外操纵table内的数据
    t->setColumnWidth(0,30);//第一列调窄
    t->selectRow(0);
}

void Table::Drag()
{
    QDrag *D=new QDrag(this);
    QMimeData *M=new QMimeData;

    //设定拖动图片
    M->setText(DragText);
    D->setMimeData(M);
    QPixmap pic(width(),RowHeight);
    pic.fill(QColor(255,255,255,100));
    QPainter P(&pic);
    P.setPen(QColor(0,0,0,200));
    P.drawText(QRectF(40,0,width(),RowHeight),DragText,QTextOption(Qt::AlignVCenter));

    D->setPixmap(pic);
    D->setHotSpot(QPoint(0,0));//左上角跟随鼠标行动

    if (D->exec(Qt::MoveAction) == Qt::MoveAction)
    {
        // 加入后才可实行拖动
    }
}

void MainWindow::Readin(QString name,QString place)
{
    int flag=0;
    if(Name.size()==0)
    {
        Now=0;
        flag=1;
    }
    Name<<name;
    Path<<place;

    if(flag==1)
    {
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
    }
    ADD();
}

void MainWindow::ADD()
{
    for(int g=0;g<Name.size();g++)
    {
        model->setItem(g, 0, new QStandardItem(QString::number(g+1)));
        model->setItem(g, 2, new QStandardItem(Name[g]));
        model->item(g, 0)->setTextAlignment(Qt::AlignCenter);//左列居中
    }
    t->setColumnWidth(0,30);
    t->setColumnWidth(1,30);
    Free::Print();
    Free::ICON();
}

void MainWindow::Reloading()
{
    QString qu=QCoreApplication::applicationDirPath();
    QFile f(qu+"/meow.txt");
    f.open(QIODevice::Text|QIODevice::ReadWrite);

    QTextStream input(&f);
    QString in;

    while(!input.atEnd())
    {
        in = input.readLine();
        Name << in;
        in = input.readLine();
        Path << in;
    }
    ADD();

    f.close();
}

void MainWindow::on_rightskip_clicked()
{
    hand=false;
    Free::Skipdown();
    hand=true;
}

void MainWindow::on_leftskip_clicked()
{
    hand=false;
    Free::Skipup();
    hand=true;
}

int playor=0;//0停止 1开始 2暂停

void MainWindow::on_start_clicked()
{
    hand=false;
    if(player->state()!=QMediaPlayer::PlayingState)
    {
        player->play();
    }
    else
    {
        player->pause();
    }
    hand=true;
}

static int uu=0;
void MainWindow::on_hook_clicked()
{   
    if(uu==0)
    {
        Hookset();
        uu=1;
        QPushButton *p=ui->hook;
        p->setText("鼠标手势已开启");
    }
    else
    {
        Hooknot();
        uu=0;
        QPushButton *p=ui->hook;
        p->setText("鼠标手势已关闭");
    }
}

void MainWindow::on_open_clicked()
{
    QFileDialog F;
    QString name;
    QString place;
    QString a;
    QFileInfo b;

    a=F.getOpenFileName(this,"打开音乐",".","music(*.mp3)");
    b=QFileInfo(a);

    name=b.fileName();
    place=b.filePath();

    Readin(name,place);
    if(Name.size()!=0)
    {
        ui->jindutiao->setEnabled(true);
        ui->jindutiao->setRange(0,max);
    }
}

void MainWindow::on_switch_2_clicked()
{
    Mode++;
    if(Mode==4)
    {
        Mode=0;
    }
    Free::Modeswitch();
}

void MainWindow::ICON()
{
    Free::ICON();
}

void MainWindow::Jindutiao()
{
    if(Name.size()!=0)
    {
        ui->jindutiao->setValue((player->position())*max/(player->duration()));
    }
}

void MainWindow::JindutiaoA()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {
        player->setPosition((ui->jindutiao->value())*player->duration()/max);\
    }
}

void MainWindow::Voice()
{
    player->setVolume(ui->voice->value());
}

void MainWindow::Super()
{
    
}

void MainWindow::on_min_clicked()
{
    //this->showMinimized();
    this->hide();
}

void MainWindow::on_close_clicked()
{
    Hooknot();
    this->close();
}

void MainWindow::Butset()
{
    QPushButton *a=ui->min;
    a->setFlat(true);
    a->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/min.png);}"
                     "QPushButton:hover{border-image: url(:/new/prefix1/icon/min1.png);}"
                     "QPushButton:pressed{border-image: url(:/new/prefix1/icon/min2.png);}");

    a=ui->close;
    a->setFlat(true);
    a->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/close.png);}"
                     "QPushButton:hover{border-image: url(:/new/prefix1/icon/close1.png);}"
                     "QPushButton:pressed{border-image: url(:/new/prefix1/icon/close2.png);}");

    a=ui->rightskip;
    a->setFlat(true);
    a->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/down.png);}"
                     "QPushButton:hover{border-image: url(:/new/prefix1/icon/down1.png);}"
                     "QPushButton:pressed{border-image: url(:/new/prefix1/icon/down2.png);}");

    a=ui->leftskip;
    a->setFlat(true);
    a->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/up.png);}"
                     "QPushButton:hover{border-image: url(:/new/prefix1/icon/up1.png);}"
                     "QPushButton:pressed{border-image: url(:/new/prefix1/icon/up2.png);}");

    a=ui->start;
    a->setFlat(true);
    a->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/go.png);}"
                     "QPushButton:hover{border-image: url(:/new/prefix1/icon/go1.png);}"
                     "QPushButton:pressed{border-image: url(:/new/prefix1/icon/go2.png);}");

    a=ui->voiceb;
    a->setFlat(true);
    a->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/voice.png);}"
                     "QPushButton:hover{border-image: url(:/new/prefix1/icon/voice.png);}"
                     "QPushButton:pressed{border-image: url(:/new/prefix1/icon/voice.png);}");

    /*QPalette pa;
    QPixmap pb(":/new/prefix1/icon/back.png");
    pa.setBrush(QPalette::Window,QBrush(pb));
    this->setPalette(pa);*/
}

void MainWindow::StateC()
{
    switch(player->state())
    {
    case (QMediaPlayer::PausedState):start->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/go.png);}"
                                                      "QPushButton:hover{border-image: url(:/new/prefix1/icon/go1.png);}"
                                                      "QPushButton:pressed{border-image: url(:/new/prefix1/icon/go2.png);}");break;

    case (QMediaPlayer::StoppedState):start->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/go.png);}"
                                                      "QPushButton:hover{border-image: url(:/new/prefix1/icon/go1.png);}"
                                                      "QPushButton:pressed{border-image: url(:/new/prefix1/icon/go2.png);}");break;

    case (QMediaPlayer::PlayingState):start->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/icon/pau.png);}"
                                                       "QPushButton:hover{border-image: url(:/new/prefix1/icon/pau1.png);}"
                                                       "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pau2.png);}");break;

    }
}

void MainWindow::MIN()
{
    QIcon icon=QIcon(":/new/prefix1/icon/yinfu.png");

    QSystemTrayIcon *min=new QSystemTrayIcon;

    min =new QSystemTrayIcon(this);

    min->setIcon(icon);
    Systemmenu(min);
    min->setToolTip(("导力音乐播放器V1.0"));

    connect(min,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(Ashow(QSystemTrayIcon::ActivationReason)));

    min->show();
}


void MainWindow::Ashow(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
       case QSystemTrayIcon::Trigger:
           this->show();
           break;

       case QSystemTrayIcon::DoubleClick:
           this->show();
           break;

       default:
           break;
    }
}

void MainWindow::Systemmenu(QSystemTrayIcon *min)
{
    QMenu *m=new QMenu;

    QAction *up=new QAction;
    QAction *down=new QAction;
    QAction *stop=new QAction;
    QAction *shutdown= new QAction;

    up->setText("上一首");
    down->setText("下一首");
    if(player->state()==QMediaPlayer::PlayingState)
    {
        stop->setText("暂停");
    }
    else
    {
        stop->setText("开始");
    }
    shutdown->setText("退出");

    connect(up,SIGNAL(triggered()),this,SLOT(on_leftskip_clicked()));
    connect(down,SIGNAL(triggered()),this,SLOT(on_rightskip_clicked()));
    connect(stop,SIGNAL(triggered()),this,SLOT(on_start_clicked()));
    connect(shutdown,SIGNAL(triggered()),this,SLOT(on_close_clicked()));

    m->addAction(up);
    m->addAction(down);
    m->addAction(stop);
    m->addAction(shutdown);

    min->setContextMenu(m);
}

void MainWindow::GOGOGO()
{
    if(hand)
    {
        hand=false;
        if(Mode==2)
        {
            Time->stop();
            player->stop();
            player->play();
            Time->start();
        }
        else
        {
            Free::Skipdown();
        }
        hand=true;
    }
}
