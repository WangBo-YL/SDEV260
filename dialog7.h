#ifndef DIALOG7_H
#define DIALOG7_H

#include <QDialog>

namespace Ui {
class Dialog7;
}

class Dialog7 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog7(QWidget *parent = nullptr);
    ~Dialog7();

private:
    Ui::Dialog7 *ui;
};

#endif // DIALOG7_H
