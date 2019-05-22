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
extern int Now;
extern int max;
extern QTimer *Time;
extern bool hand;


void Table::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QModelIndex i=indexAt(event->pos());//标记选择的行

        if(i.isValid())
        {
            ValidPress=true;
            DragPoint=event->pos();
            DragText=mModel->item(i.row(),2)->text();
            RowFrom=i.row();
        }

    }
}

void Table::mouseMoveEvent(QMouseEvent *event)
{
    if((!ValidPress))
    {
        return;
    }

    if((event->buttons() !=Qt::LeftButton))
    {
        return;
    }

    if ((event->pos() - DragPoint).manhattanLength()  < QApplication::startDragDistance())
    {
        return;//判定是否拖放
    }
    mLabel->show();
    Drag();
    mLabel->hide();
    ValidPress=false;
}

void Table::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText())
    {
        event->acceptProposedAction();//允许拖放
    }
    else
    {
        event->ignore();
    }
}

void Table::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasText())
    {
        int aim=0;

        QModelIndex i=indexAt(event->pos());

        if(i.isValid())
        {
            int py=event->pos().y();
            int ry=i.row()*RowHeight;

            if(py-ry>=(RowHeight/2))
            {
                aim=i.row()+1;
            }
            else
            {
                aim=i.row();
            }
        }
        else
        {
            aim=mModel->rowCount();
        }

        RowTo=aim;
        mLabel->setGeometry(1,(RowTo*RowHeight),width()-2,2);//改变标记黑线的位置

        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void Table::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasText())
    {
        if(RowFrom!=RowTo)
        {
            Move(RowFrom,RowTo);
        }
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

extern int playor;
void Table::mouseDoubleClickEvent(QMouseEvent *e)
{
    QModelIndex i=indexAt(e->pos());
    if(i.isValid())
    {
        hand=false;
        Now=i.row();
        player->stop();
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(Path[Now]));
        player->setPlaylist(playlist);
        player->play();
        hand=true;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons()==Qt::LeftButton)
    {
        if(e->pos().y()<100)
        {
            dragp=1;
            Move=e->globalPos()-pos();//???
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(dragp==1)
    {
        move(e->globalPos()-Move);
        Move=e->globalPos()-pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    dragp=0;
}
