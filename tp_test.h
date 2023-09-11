#ifndef TP_TEST_H
#define TP_TEST_H

#include <QWidget>

namespace Ui {
class TP_TEST;
}

class TP_TEST : public QWidget
{
    Q_OBJECT

public:
    explicit TP_TEST(QWidget *parent = nullptr);
    ~TP_TEST();

private:
    Ui::TP_TEST *ui;
};

#endif // TP_TEST_H
