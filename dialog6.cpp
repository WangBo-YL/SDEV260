#include "dialog6.h"
#include "ui_dialog6.h"
#include "dialog7.h"

Dialog6::Dialog6(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog6)
{
    ui->setupUi(this);
}

Dialog6::~Dialog6()
{
    delete ui;
}

void Dialog6::on_pushButton_2_clicked()
{
    Dialog7 dialog7;
    dialog7.setModal(true);
    dialog7.exec();
}

