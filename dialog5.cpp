#include "dialog5.h"
#include "ui_dialog5.h"

Dialog5::Dialog5(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog5)
{
    ui->setupUi(this);
}

Dialog5::~Dialog5()
{
    delete ui;
}
