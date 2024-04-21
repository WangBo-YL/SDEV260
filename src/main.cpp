#include "BudgetManagementSystem.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    BudgetManagementSystem w;
    w.show();
    return a.exec();
   
}
