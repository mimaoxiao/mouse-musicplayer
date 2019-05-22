#ifndef JINDUTIAOA_H
#define JINDUTIAOA_H

#include "head.h"

class jindutiaoA:public QSlider
{
    Q_OBJECT
public:
    jindutiaoA(QWidget *parent=0);

    void mousePressEvent(QMouseEvent *e);

signals:
    void KOI();
};


#endif // JINDUTIAOA_H
