//Budget Class Header File

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include "lib/sqlite3/sqlite3.h"

class Budget {
private:
	std::string name = "Default";

public:
	//Structs
	struct category {
		double total = 0;
		std::string name = "";//Name of category to add/alter.
	};

	struct item {
		double total = 0;//Current amount of line item. Default 0.
		double cap = 0;//Cap for budget line item. Default 0.
		std::string name = "";//Name of budget line item to add.
	};

	//Constructor
	Budget();

	//Accessors
	void getItems(std::string name) throw(std::invalid_argument) {//Return item struct

		//Data Fields
		int choice = 0;//User menu choice.
		sqlite3* db;//My database file.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			std:: cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
			sqlite3_close(db);
		}
		else {
			std::cout << "Data base opened.\n";
		}
	}

	void getTotal(std::string name) throw(std::invalid_argument);//Return double
	void getBudgets() const;//Return string

	//Mutators
	void addBudget(std::string name, double total, std::vector<item>& items) throw(std::invalid_argument);
	void deleteBudget(std::string name) throw(std::invalid_argument);
	void addItem(std::string name, item newItem) throw(std::invalid_argument);
	void deleteItem(std::string itemName, std::string budgetName) throw(std::invalid_argument);
	void addFunds(std::string name, double total) throw(std::invalid_argument);
	void addExpense(std::string catName, std::string budgetName, std::string itemName, double total) throw(std::invalid_argument);
	void addCategory(std::string name) throw(std::invalid_argument);
	void deleteCategory(std::string name) throw(std::invalid_argument);

};//End of class