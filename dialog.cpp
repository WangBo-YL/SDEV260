#include "dialog.h"
#include "ui_dialog.h"
#include "dialog6.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_2_clicked()
{
    Dialog6 dialog6;
    dialog6.setModal(true);
    dialog6.exec();

}

