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


void Free::Clear()
{
    QString qu=QCoreApplication::applicationDirPath();

    QFile f(qu+"/meow.txt");
    f.open(QFile::WriteOnly|QFile::Truncate);
    f.close();
}

void Free::Print()
{
    QString qu=QCoreApplication::applicationDirPath();//当前程序所在地址
    QFile f(qu+"/meow.txt");
    f.open(QIODevice::Text|QIODevice::ReadWrite);

    QTextStream out(&f);

    for(int j=0;j<Name.size();j++)//输出播放列表
    {
        out<<Name[j]<<endl;
        out<<Path[j]<<endl;
    }
    f.close();
}

void Free::Switch(int from, int to)
{
    if(to==Name.size())   //改变播放列表
    {
        QString a=Name[from];
        Name.removeAt(from);
        Name<<a;

        a=Path[from];
        Path.removeAt(from);
        Path<<a;

        Free::Clear();
        Free::Print();

        return;
    }

    QString a;
    a=Name[from];
    Name.removeAt(from);
    if(from<to)
    {
        Name.insert(to-1,a);

    }
    else
    {
        Name.insert(to,a);
    }

    a=Path[from];
    Path.removeAt(from);
    if(from<to)
    {
        Path.insert(to-1,a);

    }
    else
    {
        Path.insert(to,a);
    }

    Free::Clear();
    Free::Print();

    return;
}

void Free::Reloadnum()//加载表格
{
    for(int h=0;h<Name.size();h++)
    {
        model->setItem(h, 0, new QStandardItem(QString::number(h+1)));
        model->item(h, 0)->setTextAlignment(Qt::AlignCenter);//左列居中
    }
}

void Free::Skipdown()
{
    Time->stop();
    if(Name.size()==0)
    {
        Time->start();
        return;
    }
    if(Mode==3)
    {
        if(Name.size()==1)
        {
            player->stop();
            playlist->clear();
            playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
            player->setPlaylist(playlist);
            player->play();

            Time->start();
            return;
        }
        int next=rand()%((Path.size()-1)-0+1)+0;
        while(next==Now)
        {
            next=rand()%((Path.size()-1)-0+1)+0;
        }
        Now=next;

        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();

        Time->start();
        return;
    }
    if(Now==Path.size()-1)
    {
        if(Mode==1)
        {
            player->stop();
            Now=0;
            Free::ICON();

            Time->start();
            return;
        }
        Now=0;
        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();

        Time->start();
        return;
    }
    else
    {
        Now++;
        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();
        Free::ICON();

    }
    Time->start();
}

void Free::Skipup()
{
    if(Name.size()==0)
    {
        Time->start();
        return;
    }
    Time->stop();
    if(Mode==3)
    {
        if(Name.size()==1)
        {
            player->stop();
            playlist->clear();
            playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
            player->setPlaylist(playlist);
            player->play();

            Time->start();
            return;
        }
        int next=rand()%((Path.size()-1)-0+1)+0;
        while(next==Now)
        {
            next=rand()%((Path.size()-1)-0+1)+0;
        }
        Now=next;

        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();

        Time->start();
        return;
    }
    if(Now==0)
    {
        Now=Path.size()-1;
        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();

        Time->start();
        return;
    }
    else
    {
        Now--;
        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();
        Free::ICON();
    }
    Time->start();
}

void Free::Modeswitch()
{
    switch (Mode)
    {
    case 0: swi->setText(("顺序播放"));break;
    case 1: swi->setText(("单曲循环"));break;
    case 2: swi->setText(("随机"));break;
    case 3: swi->setText(("列表循环"));break;
    }
}

void Free::Start()
{
    player->play();
}

void Free::ICON() //正在播放标志
{
    for(int i=0;i<Path.size();i++)
    {
        if(i!=Now)
        {
            model->setItem(i, 1, new QStandardItem(""));
        }
        else
        {
            QStandardItem *o=new QStandardItem;
            QIcon a(":/new/prefix1/icon/aimm.png");
            o->setIcon(a);
            model->setItem(i, 1, o);
        }
    }
}

void Free::StateC()
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
