#include "btbutton.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "table.h"
#include "free.h"
#include "jindutiaoa.h"
#include "head.h"

btbutton::btbutton(QWidget *parent) //构造函数
: QPushButton(parent)
{
    setAutoRepeat(false);
}

