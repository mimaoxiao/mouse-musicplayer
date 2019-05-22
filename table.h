#ifndef TABLE_H
#define TABLE_H

#include "head.h"

class Table : public QTableView
{
    Q_OBJECT

public:
    Table(QWidget *parent=Q_NULLPTR);
    void SetModel(QStandardItemModel *model);

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void Drag();
    void Move(int from,int to);
    void Deleterow(int aim);

private:
    QStandardItemModel *mModel;
    QLabel* mLabel;
    QString DragText;
    QPoint DragPoint;
    QPoint rightaim;

    bool ValidPress;
    int RowHeight;
    int RowFrom;
    int RowTo;    

private:
    QMenu *menu;
    QAction *A;
    QTableView *TH=this;

private slots:
    void Rightmenu(QPoint p);
    void Rocket();

};


#endif // TABLE_H
