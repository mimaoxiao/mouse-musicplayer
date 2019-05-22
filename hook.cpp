#include "btbutton.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "table.h"
#include "free.h"
#include "jindutiaoa.h"
#include "head.h"

static int x1=0;
static int x2=0;
static int ya=0;
static int yb=0;
static POINT XA;
static POINT XB;

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


HHOOK meow=NULL;

LRESULT CALLBACK Hook(int nCode,WPARAM wParam,LPARAM lParam )
{
    if(nCode == HC_ACTION)
    {

       if(lParam==WM_LBUTTONDOWN)
       {
           GetCursorPos(&XA);
            x1=XA.x;
            ya=XA.y;
            x2=x1;
            yb=ya;
       }

       if(lParam==WM_LBUTTONUP)
       {

           GetCursorPos(& XB);
            x2= XB.x;
            yb= XB.y;

            if((( x2- x1)>=500)&&(abs( ya- yb)<=300))
            {
                hand = false;
                Free::Skipdown();
                hand=true;
            }
            if((( x2- x1)<=-500)&&(abs( ya- yb)<=300))
            {
                hand = false;
                Free::Skipup();
                hand=true;
            }

            if((( yb- ya)>=500)&&(abs( x1- x2)<=500))
            {

                Mode++;
                if(Mode==4)
                {
                    Mode=0;
                }
                Free::Modeswitch();

             }
            if((( yb- ya)<=-500)&&(abs( x1- x2)<=500))
            {
                Mode--;
                if(Mode==-1)
                {
                    Mode=3;
                }
                Free::Modeswitch();
            }
            ya=yb;
            x1=x2;
       }
    }

    return false;//CallNextHookEx(meow,nCode,wParam,lParam);
}

void MainWindow::Hookset()
{
    meow=SetWindowsHookEx( WH_MOUSE_LL,Hook,NULL,0);
}

void MainWindow::Hooknot()
{
    UnhookWindowsHookEx(meow);
}
