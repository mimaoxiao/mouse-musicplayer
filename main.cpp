#include "btbutton.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "table.h"
#include "free.h"
#include "jindutiaoa.h"
#include "head.h"

QStringList Name;
QStringList Path;
QStandardItemModel* model= new QStandardItemModel;
QMediaPlayer *player=new QMediaPlayer;
QMediaPlaylist *playlist=new QMediaPlaylist;

QPushButton *start=NULL;
QPushButton *swi=NULL;

int max=1000;
bool hand=true;
QTimer *Time=new QTimer;
QTimer limit;

int Now=0;
int Mode=0;//0列表循环 1顺序播放 2单曲循环 3随机

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
