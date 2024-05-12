#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "dialog2.h"
#include "dialog3.h"
#include "dialog4.h"
#include "dialog5.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::button()
{

}


void MainWindow::on_pushButton_clicked()
{
    Dialog dialog;
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::on_pushButton_2_clicked()
{
    Dialog2 dialog2;
    dialog2.setModal(true);
    dialog2.exec();

}


void MainWindow::on_pushButton_3_clicked()
{
    Dialog3 dialog3;
    dialog3.setModal(true);
    dialog3.exec();

}


void MainWindow::on_pushButton_4_clicked()
{
    Dialog4 dialog4;
    dialog4.setModal(true);
    dialog4.exec();

}


void MainWindow::on_pushButton_5_clicked()
{
    Dialog5 dialog5;
    dialog5.setModal(true);
    dialog5.exec();

}

