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


void Table::Rightmenu(QPoint p)
{
    QModelIndex index = TH->indexAt(p);
    rightaim=p;
    if (index.isValid())
    {
        menu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
    }
}

void Table::Rocket()
{
   QModelIndex i=indexAt(rightaim);
   mModel->removeRow(i.row());

   Deleterow(i.row());
   Free::Clear();
   Free::Print();
   Free::Reloadnum();
}

void Table::Deleterow(int aim)
{
    hand=false;
    Name.removeAt(aim);
    Path.removeAt(aim);

    int flag=1;
    if(player->state()==QMediaPlayer::PlayingState)
    {
        flag=0;
    }

    Time->stop();
    if(Now==aim)
    {
        if(Name.size()==0)
        {
            player->stop();
        }
        else
        {
            player->stop();
            playlist->clear();
            if(Now==(Path.size()))
            {
                Now--;
                playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
            }
            else
            {
                playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
            }
            player->setPlaylist(playlist);
            if(flag==0)
            {
                player->play();
            }
        }
    }
    Free::ICON();
    hand=true;
    Time->start();
}
