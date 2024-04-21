#include "BudgetManagementSystem.h"
#include <QtWidgets/QApplication>
#include "BudgetClass.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    BudgetManagementSystem w;
    w.show();
    return a.exec();
   
}
