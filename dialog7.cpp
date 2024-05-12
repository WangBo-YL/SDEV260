#include "dialog7.h"
#include "ui_dialog7.h"

Dialog7::Dialog7(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog7)
{
    ui->setupUi(this);
}

Dialog7::~Dialog7()
{
    delete ui;
}
