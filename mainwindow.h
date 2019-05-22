#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "head.h"

class Table;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetUp();
    void ADD();
    void Hookset();
    void Reloading();
    void Super();
    void Readin(QString name, QString place);
    void Minmax();
    void Butset();
    void Hooknot();
    void MIN();
    void Systemmenu(QSystemTrayIcon *min);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    Table* t=NULL;

private slots:
    void on_rightskip_clicked();
    void on_leftskip_clicked();
    void on_start_clicked();
    void on_hook_clicked();
    void on_open_clicked();
    void on_switch_2_clicked();
    void on_min_clicked();
    void on_close_clicked();

    void StateC();
    void ICON();

    void Jindutiao();
    void JindutiaoA();

    void Voice();

    void Ashow(QSystemTrayIcon::ActivationReason reason);

    void GOGOGO();

private:
    Ui::MainWindow *ui;
    
    QPoint Move;
    int dragp=0;
    int i=0;
};

#endif // MAINWINDOW_H
