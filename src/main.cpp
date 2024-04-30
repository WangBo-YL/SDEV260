#include "BudgetManagementSystem.h"
#include <QtWidgets/QApplication>
#include "BudgetClass.h"
#include "SavingsClass.h"


int main(int argc, char *argv[])
{   

    QApplication a(argc, argv);
    BudgetManagementSystem w;
    w.show();
    return a.exec();
}


