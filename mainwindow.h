#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "tp_test.h"
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void delay_seconds(unsigned int i);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:

    void slot_tp_timer_out();
    void slot_TP_test_exit();
    void slot_TP_test_clear();

private:
    Ui::MainWindow *ui;

    QTimer tp_timer;
    QTableWidget TP_test;
    QPoint touch_point;
    void tp_test_init();
    void touch_screent_test_failure();
    void touch_screent_test_success();
};

#endif // MAINWINDOW_H
