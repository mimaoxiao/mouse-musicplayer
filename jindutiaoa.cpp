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
extern bool hand;

extern int Now;
extern int Mode;//0列表循环 1顺序播放 2单曲循环 3随机

extern QTimer *Time;
extern int max;

jindutiaoA::jindutiaoA(QWidget *parent) //构造函数
: QSlider(parent)
{

}

void jindutiaoA::mousePressEvent(QMouseEvent *e)
{
    double p=((e->pos().x()))/(double)(this->width());
    setValue(p*max);

    emit KOI();
}

