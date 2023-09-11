#include "tp_test.h"
#include "ui_tp_test.h"

TP_TEST::TP_TEST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TP_TEST)
{
    ui->setupUi(this);
}

TP_TEST::~TP_TEST()
{
    delete ui;
}
