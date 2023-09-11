#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include<QElapsedTimer>
#include <unistd.h>
#include <QDesktopWidget>
#include <QDebug>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TP_test.setParent(this);
    TP_test.hide();
    setAttribute(Qt::WA_AcceptTouchEvents,true);
    this->installEventFilter(this);

    tp_test_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::delay_seconds(unsigned int time)
{
//    unsigned int ms = time * 1000;
    unsigned int ms = time * 200;
    QElapsedTimer t;
    t.start();
    t.elapsed();
    while(t.elapsed() < ms)
    {
        QCoreApplication::processEvents();
        usleep(20);
    }

}

void MainWindow::slot_tp_timer_out()
{
    //  qDebug()<<"time out";
       touch_screent_test_failure();
       slot_TP_test_exit();
}

void MainWindow::slot_TP_test_exit()
{
    delay_seconds(1);
//    TP_test.hide();
    tp_timer.stop();
    TP_test.close(); //Add by xhx
    QApplication::closeAllWindows();
    qApp->quit();

}


void MainWindow::slot_TP_test_clear()
{
   for(int i=0;i< TP_test.rowCount();i++)
   {
        for(int j=0;j<TP_test.columnCount() ;j++)
        {
           TP_test.item( i,j)->setBackgroundColor(Qt::white);
        }
   }
}

//针对触摸屏TP_test过滤触摸事件
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug()<<"eventtype"<<event->type();
    if(event->type() == QEvent::HoverMove&& !TP_test.isHidden())
    {
        //可以通过函数pos()、x()、y()得知鼠标事件发生时鼠标指针相对于窗口的位置
        touch_point = ((QMouseEvent *)event)->pos();
        //qDebug()<<touch_point;
        if(TP_test.itemAt(touch_point)!=NULL)
        {
            tp_timer.start(5000); //定时时间内没有任何操作，则会执行超时函数
            TP_test.itemAt(touch_point)->setBackgroundColor(Qt::green); //将鼠标所在的格子填充为绿色
            for(int i=0;i< TP_test.rowCount();i++)
            {
                 for(int j=0;j<TP_test.columnCount() ;j++)
                 {
                    if(TP_test.item( i,j)->backgroundColor() != Qt::green)
                    {
                        return true;
                    } else if(i==TP_test.rowCount()-1&&j==TP_test.columnCount()-1)
                    {
                        //所有格子填充为绿色，则标记测试成功并退出测试
                        this->removeEventFilter(this);
                        touch_screent_test_success();
                        slot_TP_test_exit();
                        return true;
                    }
                 }
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}



void MainWindow::tp_test_init()
{

    connect(&tp_timer,SIGNAL(timeout()),this,SLOT(slot_tp_timer_out()));

    QDesktopWidget* desktopWidget = QApplication::desktop();

    //设置行列头隐藏
    TP_test.verticalHeader()->setHidden(true);
    TP_test.horizontalHeader()->setHidden(true);

    //禁止滑动条
    TP_test.verticalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    TP_test.horizontalHeader()->setSectionResizeMode ( QHeaderView::Stretch );

    //设置为不可编辑
    TP_test.setEditTriggers(QAbstractItemView::NoEditTriggers);

    //生成格子（Tables)
    int num_row = desktopWidget->height()/50 -TP_test.rowCount();
    int num_column = desktopWidget->width()/100 - TP_test.columnCount();
    if(desktopWidget->width()%100!=0)
        num_column++;
    if( desktopWidget->height()%50!=0)
        num_row++;

    for(int i=0;i< num_row ;i++)
          TP_test.insertRow(0);
    for(int j=0;j<num_column ;j++)
          TP_test.insertColumn(0);

    for(int i=0;i< num_row ;i++)
    {
           TP_test.horizontalHeader()->resizeSection(i,50);
           for(int j=0;j<num_column ;j++)
           {
              QTableWidgetItem *item = new QTableWidgetItem();
              TP_test.setItem(i,j,item);
              TP_test.verticalHeader()->resizeSection(j,100);
              TP_test.item(i,j)->setBackgroundColor(Qt::white);
              TP_test.item(i,j)->setFlags(Qt::NoItemFlags);
           }
     }

    TP_test.setMinimumSize(desktopWidget->width(),desktopWidget->height());
//  TP_test.setWindowState(Qt::WindowFullScreen);
    setWindowState(Qt::WindowFullScreen);

    tp_timer.start(10000); //定时时间内没有任何操作，则会执行超时函数

    //将格子颜色恢复为白色，否则再次进入依然是上次测试的颜色
    slot_TP_test_clear();
    TP_test.show();

    //等待该测试结束，主要用在连续测试
    //delay_until_test_finished();
}

void MainWindow::touch_screent_test_success()
{
    QTextStream qout(stdout);
    qout <<"success, info: touchscreen test pass!\n";

    //system("echo  success");
    //system("sync");
}

void MainWindow::touch_screent_test_failure()
{
    QTextStream qout(stdout);
    qout <<"failed,info:Time out!!!\n";

   // system("echo \"failed\" > /home/vantron/tp_test.txt");
    //system("sync");
}
