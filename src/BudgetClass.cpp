//Budget Class Source File
#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include "BudgetClass.h"
#include "sqlite3.h"


	//Database calls.
	//Start a transaction in the database. Returns sqlite ok.
	int Budget::startTransaction(sqlite3* db) throw(std::runtime_error) {
		std::string statement = "BEGIN TRANSACTION; ";

		//Sending statement to database.
		if (sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
			std::string error = "There was an error beginging transaction. ";
			error + sqlite3_errmsg(db);
			error + "\n";
			throw std::runtime_error(error);
		}
		return SQLITE_OK;
	}//End of startTransaction

	//Rolls database back if transaction faild. Returns sqlite ok.
	int Budget::rollback(sqlite3* db) throw(std::runtime_error) {
		std::string statement = "ROLLBACK; ";

		//Sending statement to database
		if (sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
			std::string error = "There was an error rolling back database. ";
			error + sqlite3_errmsg(db);
			error + "\n";
			throw std::runtime_error(error);;
		}
		return SQLITE_OK;
	}//End of rollback

	//Commits changes if everything went fine. Returns sqlite ok.
	int Budget::commit(sqlite3* db) throw(std::runtime_error) {
		std::string statement = "COMMIT; ";

		//Sending statement to database
		if (sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
			std::string error = "There was an error commiting to database. ";
			error + sqlite3_errmsg(db);
			error + "\n";
			throw std::runtime_error(error);
			rollback(db);
		}
		return SQLITE_OK;
	}//End of commit

	//Runs a simple query that needs no output. returns 0 for failed or 1 for success. 
	int Budget::simpleQuery(sqlite3* db, std::string query) throw(std::runtime_error) {

		//Running query and returning 0 if bad, 1 if good.
		if (sqlite3_exec(db, query.c_str(), NULL, NULL, NULL) == SQLITE_OK) {
			return 1;
		}
		else {
			std::string error = "Simple Query Error: ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
			return 0;
		}
	}//End of simpleQuery.

	//Returns budget id as a int. Needs budget name. Tested Working**************
	int Budget::getBudgetID(std::string budgetName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		int id = 0;//Total to send back.

		//Checking name integrity.
		if (budgetName.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::invalid_argument(error);
		}
		else {
			//Building query
			query = "SELECT budget_id FROM budget WHERE budget_name LIKE '";
			query += budgetName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in budget id query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				id = sqlite3_column_int(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return id;
	}//End of getBudgetID

	//Returns a vector of item id's. Needs a budget name. Tested Working**************
	std::vector<int> Budget::getItemIDList(std::string budgetName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<int> idList;//List of id numbers to send back.
		int budgetID = 0;//ID of budget we are working with.

		//Checking name integrity.
		if (budgetName.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting budget id number.
			budgetID = this->getBudgetID(budgetName);

			//Building query
			query = "SELECT item_id FROM item WHERE budget_id = ";
			query += std::to_string(budgetID) + "; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in item id list query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				//Filling in vector. Step into qres is in while loop.
				while (sqlite3_step(qres) == SQLITE_ROW) {
					idList.push_back(sqlite3_column_int(qres, 0));
				}
			}
			sqlite3_finalize(qres);//Close result.
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return idList;
	}//End of getItemIDList

	//Returns a id number for item. Tested Working**********
	int Budget::getItemID(std::string itemName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		int id = 0;//Total to send back.

		//Checking name integrity.
		if (name.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error + sqlite3_errmsg(db);
			error + "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building query
			query = "SELECT item_id FROM item WHERE item_name LIKE '";
			query += itemName + "'; ";
			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in item id query. ";
				error + sqlite3_errmsg(db);
				error + "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				id = sqlite3_column_int(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return id;
	}//End of getItemID

	//Constructor
	Budget::Budget() {

	}

	//Destructor
	Budget::~Budget() {

	}

	//Accessors
	//Returns double for budget total. Needs budget name. Tested Working*********
	double Budget::getBudgetTotal(std::string budgetName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		double total = 0;//Total to send back.

		//Checking name integrity.
		if (budgetName.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building query
			query = "SELECT total FROM budget WHERE budget_name LIKE '";
			query += budgetName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in budget total query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				total = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return total;
	}//End of getTotal

	//Returns double for category total. Needs category name. Tested Working***********
	double Budget::getCategoryTotal(std::string categoryName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		double total = 0;//Total to send back.

		//Checking name integrity.
		if (categoryName.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building query
			query = "SELECT total FROM category WHERE cat_name LIKE '";
			query += categoryName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in category total query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				total = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return total;
	}//End of getTotal

	//Returns double for item total. Needs item name. Tested Working******
	double Budget::getItemTotal(std::string itemName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		double total = 0;//Total to send back.

		//Checking name integrity.
		if (itemName.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);;
		}
		else {
			//Building query
			query = "SELECT total FROM item WHERE item_name LIKE '";
			query += itemName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in item total query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				total = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return total;
	}//End of getTotal

	//Returns double for item cap. Needs item name. Tested Working******
	double Budget::getItemCap(std::string itemName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		double cap = 0;//Cap to send back.

		//Checking name integrity.
		if (itemName.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building query
			query = "SELECT cap FROM item WHERE item_name LIKE '";
			query += itemName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in item cap query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				cap = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return cap;
	}//End of getTotal

	//Returns vector of strings that are budget names. Tested Working********************
	std::vector<std::string> Budget::getBudgetList() throw(std::runtime_error) {
		//Data fields
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<std::string> budgetList;//Vector of strings to return.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building query
			query = "SELECT budget_name FROM budget; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in get budget names list query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				//Filling in vector. Step into qres is in while loop.
				while (sqlite3_step(qres) == SQLITE_ROW) {
					budgetList.push_back((reinterpret_cast<const char*>(sqlite3_column_text(qres, 0))));
				}
				sqlite3_finalize(qres);//Close result.
			}//End of main else.

			//Closing database.
			sqlite3_close(db);
		}//End of main else.
		return budgetList;
	}//End of getBudgets

	//Returns a list of category name strings. Needs a budget name.  Tested Working************
	std::vector<std::string> Budget::getCategoryList(std::string budgetName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<std::string> categoryList;//Vector of strings to return.
		int budgetID = 0;//ID number of budget.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting budget id.
			budgetID = this->getBudgetID(budgetName);

			//Building query
			query = "SELECT cat_name FROM category WHERE budget_id = " + std::to_string(budgetID) + "; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in get category names list query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				//Filling in vector. Step into qres is in while loop.
				while (sqlite3_step(qres) == SQLITE_ROW) {
					categoryList.push_back((reinterpret_cast<const char*>(sqlite3_column_text(qres, 0))));
				}
				sqlite3_finalize(qres);//Close result.
			}//End of main else.

			//Closing database.
			sqlite3_close(db);
		}//End of main else.
		return categoryList;
	}//End of getCategoryList

	//Returns vector of item structs. Needs budget name. Tested Working****************
	std::vector<Budget::item> Budget::getItemList(std::string budgetName) throw(std::invalid_argument, std::runtime_error) {

		//Data Fields
		std::string query = "";//Query to send to database.
		int budgetID = 0;//ID number for budget to get items from.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<item> items;//Vector of item structs to return.

		//Checking name integrity.
		if (name.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting budget id.
			budgetID = this->getBudgetID(budgetName);

			//Creating string query.
			query = "SELECT item_name, cap, total FROM item WHERE budget_id = ";
			query += std::to_string(budgetID) + "; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in get item names list query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				//Filling in vector. Step into qres is in while loop.
				while (sqlite3_step(qres) == SQLITE_ROW) {
					item temp = item();
					temp.name = reinterpret_cast<const char*>(sqlite3_column_text(qres, 0));
					temp.cap = sqlite3_column_double(qres, 1);
					temp.total = sqlite3_column_double(qres, 2);
					items.push_back(temp);
				}
				sqlite3_finalize(qres);//Close result.
			}

			//Closing database.
			sqlite3_close(db);

		}//End of main else.
		return items;
	}//End of getItems

	//Returns vector of doubles that are transaction amounts. Needs a budget name. Tested Working********************
	std::vector<double> Budget::getBudgetTransactionsList(std::string budgetName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<int> itemIDList;//List of item id numbers.
		std::vector<double> transactionList;//Vector of doubles to return.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting list of item id numbers that belong to this budget.
			itemIDList = this->getItemIDList(budgetName);

			for (int i = 0; i < itemIDList.size(); i++) {
				//Building query
				query = "SELECT total FROM transactions WHERE item_id = " + std::to_string(itemIDList[i]) + "; ";

				//Making query and checking for error.
				if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
					sqlite3_finalize(qres);
					std::string error = "There was an error in get budget transactions list query. ";
					error += sqlite3_errmsg(db);
					error += "\n";
					throw std::runtime_error(error);
				}
				else {
					//Filling in vector. Step into qres is in while loop.
					while (sqlite3_step(qres) == SQLITE_ROW) {
						transactionList.push_back(sqlite3_column_double(qres, 0));
					}
					sqlite3_finalize(qres);//Close result.
				}//End of second else.
			}//End of for loop.

			//Closing database.
			sqlite3_close(db);
		}//End of main else.
		return transactionList;
	}//End of getBudgets

	//Returns vector of doubles that are transaction amounts. Needs a item name. Tested Working********************
	std::vector<double> Budget::getItemTransactionsList(std::string itemName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<double> transactionList;//Vector of doubles to return.
		int itemID = 0;//ID number of item.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting item id number.
			itemID = this->getItemID(itemName);

			//Building query
			query = "SELECT total FROM transactions WHERE item_id = " + std::to_string(itemID) + "; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in get item transactions list query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				//Filling in vector. Step into qres is in while loop.
				while (sqlite3_step(qres) == SQLITE_ROW) {
					transactionList.push_back(sqlite3_column_double(qres, 0));
				}
				sqlite3_finalize(qres);//Close result.
			}//End of second else.
			//Closing database.
			sqlite3_close(db);
		}//End of main else.
		return transactionList;
	}//End of getBudgets

	//Mutators
	//Needs a budget name, budget starting total, and vector of items. Tested Working*************
	void Budget::addBudget(std::string budgetName, double total, std::vector<Budget::item>& items) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string dataBaseString = "";//String being used for database statement.
		sqlite3* db;//My database file.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			try {
				if (budgetName.empty()) {
					throw std::invalid_argument("Name cannot be empty.\n");
				}
				if (total < 0) {
					throw std::invalid_argument("Total cannot be a negative number.\n");
				}
			}
			catch (std::invalid_argument& e) {
				throw e.what();
			}

			//Building statement for budget name and starting total.
			dataBaseString = "INSERT INTO budget (budget_name, total) VALUES('";
			dataBaseString += budgetName + "', " + std::to_string(total);
			dataBaseString += "); ";
			//Inserting into budget table.
			if (this->simpleQuery(db, dataBaseString) == 0) {
				std::string error = "There was an error inserting into budget table. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
				rollback(db);
			}

			//Checking struct list data integrity.
			for (int i = 0; i < items.size(); i++) {
				if (items[i].name.empty()) {
					throw std::invalid_argument("Item name cannot be empty.\n");
				}
				if (items[i].cap < 0) {
					throw std::invalid_argument("Item cap cannot be negative.\n");
				}
				if (items[i].total < 0) {
					throw std::invalid_argument("Item cap cannot be negative.\n");
				}
			}

			//Loop for building statement and inserting budget items.
			for (int i = 0; i < items.size(); i++) {
				//Building string for budget item insertion.
				dataBaseString = "INSERT INTO item (item_name, cap, total, budget_id) VALUES('";
				dataBaseString += items[i].name + "', ";
				dataBaseString += std::to_string(items[i].cap) + ", ";
				dataBaseString += std::to_string(items[i].total) + ", ";
				dataBaseString += std::to_string(this->getBudgetID(budgetName)) + ");";
				//Inserting new item into item table.
				if (this->simpleQuery(db, dataBaseString) == 0) {
					std::string error = "There was an error inserting into item table. ";
					error += sqlite3_errmsg(db);
					error += "\n";
					throw std::runtime_error(error);
					rollback(db);
				}
			}

			//Closing database.
			sqlite3_close(db);
		}//End of main else.
	}//End of addBudget

	//Needs a budget name. Tested Working**************
	void Budget::deleteBudget(std::string name) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		int budgetID = 0;//ID number for budget to delete.
		std::vector<int> itemIDList;//Vector of item structs.

		//Checking name integrity.
		if (name.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting items
			itemIDList = this->getItemIDList(name);

			//Getting budget id number.
			budgetID = this->getBudgetID(name);

			//Starting dataebase transaction.
			this->startTransaction(db);

			//Loop to change all related transactions to NULL budget id.
			for (int i = 0; i < itemIDList.size(); i++) {
				//Building statement
				statement = "UPDATE transactions SET item_id = NULL WHERE item_id = ";
				statement += std::to_string(itemIDList[i]) + "; ";

				//Sending statement to database.
				this->simpleQuery(db, statement);
			}

			//Building statement to delete all related budget items.
			statement = "DELETE FROM item WHERE budget_id = ";
			statement += std::to_string(budgetID) + "; ";

			//Deleting items from database.
			this->simpleQuery(db, statement);

			//Building statement to delete budget.
			statement = "DELETE FROM budget WHERE budget_id = ";
			statement += std::to_string(budgetID) + "; ";

			//Deleting items from database.
			this->simpleQuery(db, statement);
		}
		//Commiting changes.
		this->commit(db);

		//Closing database.
		sqlite3_close(db);
	}//End of deleteBudget

	//Needs a budget name and item struct. Tested Working*****************
	void Budget::addItem(std::string budgetName, Budget::item newItem) throw(std::invalid_argument, std::runtime_error) {
		//Datafields
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			try {//Checking data integrity.
				if (newItem.name.empty()) {
					throw std::invalid_argument("Item name cannot be empty.\n");
				}
				if (newItem.cap < 0) {
					throw std::invalid_argument("Cap cannot be a negative number.\n");
				}
				if (newItem.total < 0) {
					throw std::invalid_argument("Total cannot be a negative number.\n");
				}
				if (budgetName.empty()) {
					throw std::invalid_argument("Budget name cannot be empty..\n");
				}
			}
			catch (std::invalid_argument& e) {
				throw e.what();
			}
		}//End of check and database opening else.

		//Build insertion string.
		statement = "INSERT INTO item (item_name, cap, total, budget_id) VALUES ('";
		statement += newItem.name + "', " + std::to_string(newItem.cap) + ", " + std::to_string(newItem.total) + ", " + std::to_string(this->getBudgetID(budgetName)) + "); ";

		//Inserting new item.
		this->simpleQuery(db, statement);

		//Closing database
		sqlite3_close(db);
	}//End of addItem

	//Deletes item from table and sets all related transactions to NULL item_id. Needs item name. Tested Working*****************
	void Budget::deleteItem(std::string itemName) throw(std::invalid_argument, std::runtime_error) {
		//Datafields
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		int id = 0;//ID number for item.

		//Checking name integrity.
		if (name.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting item id number.
			id = this->getItemID(itemName);

			//Starting dataebase transaction.
			this->startTransaction(db);

			//Building statement to update all related transactions item_id to NULL.
			statement = "UPDATE transactions SET item_id = NULL WHERE item_id = " + std::to_string(id) + "; ";

			//Making changes to transactions table.
			this->simpleQuery(db, statement);

			//Creating statement to delete item.
			statement = "DELETE FROM item WHERE item_name LIKE '" + itemName + "'; ";

			//Making changes to item table.
			this->simpleQuery(db, statement);

			//Comminting changes and closing database.
			this->commit(db);
			sqlite3_close(db);
		}//End of main else.
	}//End of deleteItem

	//Adds funds to total of a budget. Needs budget name and amount to add. Test Working***************
	void Budget::addFunds(std::string name, double amount) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		double currentTotal = 0;//Current budget total.
		sqlite3_stmt* qres;//Results from rental id query.

		//Checking data integrity.
		if (name.size() == 0) {
			throw std::invalid_argument("Name cannot be blank.\n");
		}
		if (amount < 0) {
			throw std::invalid_argument("Can not add a negative number.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building statement
			statement = "SELECT total FROM budget WHERE budget_name LIKE '" + name + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, statement.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in add funds query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				currentTotal = sqlite3_column_int(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}

			//Building statement to update total.
			statement = "UPDATE budget SET total = " + std::to_string(currentTotal + amount) + " WHERE budget_name LIKE '" + name + "'; ";

			//Updating total in table.
			this->simpleQuery(db, statement);

			//Closing database
			sqlite3_close(db);
		}//End of main else
	}//End of addFunds

	//Adds a transaction. Needs a category name, item name, budget name, and amount of transaction. Tested Working***************
	void Budget::addExpense(std::string catName, std::string itemName, std::string budgetName, double amount) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		double currentBudgetTotal = 0;//Current budget total.
		double currentItemTotal = 0;//Current item total.
		int itemID = 0;//Id number of item getting transaction.
		sqlite3_stmt* qres;//Results from rental id query.

		//Checking data integrity.
		if (catName.size() == 0) {
			throw std::invalid_argument("Catagory name cannot be blank.\n");
		}
		if (catName.size() == 0) {
			throw std::invalid_argument("Item name cannot be blank.\n");
		}
		if (amount < 0) {
			throw std::invalid_argument("Can not add a negative number.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting item id number.
			itemID = this->getItemID(itemName);

			//Building statement to get budget total.
			statement = "SELECT total FROM budget WHERE budget_name LIKE '" + budgetName + "'; ";
			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, statement.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in add expense/budget total query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				currentBudgetTotal = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}

			//Building statement to get item total.
			statement = "SELECT total FROM item WHERE item_name LIKE '" + itemName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, statement.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in add expense/ item total query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
				return;
			}
			else {
				sqlite3_step(qres);//Step in.
				currentItemTotal = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}

			//Starting transaction.
			this->startTransaction(db);

			//Building statement to update budget total and sending to database.
			statement = "UPDATE budget SET total = " + std::to_string(currentBudgetTotal - amount) + " WHERE budget_name LIKE '" + budgetName + "'; ";
			this->simpleQuery(db, statement);

			//Building statement to update item total and sending to database.
			statement = "UPDATE item SET total = " + std::to_string(currentItemTotal + amount) + " WHERE item_id = " + std::to_string(itemID) + "; ";
			this->simpleQuery(db, statement);

			//Building statement to add transaction and sending to database.
			statement = "INSERT INTO transactions (total, item_id, sav_id, cat_name) VALUES (" + std::to_string(amount) + ", " + std::to_string(itemID) + ", NULL, '" + catName + "'); ";
			this->simpleQuery(db, statement);

			//Commiting changes and closing database.
			this->commit(db);
			sqlite3_close(db);
		}//End of main else.
	}//End of addExpense

	//Adds a category. Needs a category name and a budget name it belongs to.
	void Budget::addCategory(std::string catName, std::string budgetName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		int budgetID = 0;//ID number of budget.

		//Checking data integrity.
		if (catName.size() == 0) {
			throw std::invalid_argument("Catagory name cannot be blank.\n");
		}
		if (budgetName.size() == 0) {
			throw std::invalid_argument("Budget name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting budget id number.
			budgetID = getBudgetID(budgetName);

			//Building statement to add category.
			statement = "INSERT INTO category (cat_name, budget_id) VALUES ('" + catName + "', " + std::to_string(budgetID) + "); ";
			this->simpleQuery(db, statement);

			//Closing database
			sqlite3_close(db);
		}//End of main else.
	}//End of addCategory

	//Deletes a category. Needs a category name. Tested Working*************
	void Budget::deleteCategory(std::string catName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		int budgetID = 0;//ID number of budget.

		//Checking data integrity.
		if (catName.size() == 0) {
			throw std::invalid_argument("Catagory name cannot be blank.\n");
		}

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Building statement to delete category.
			statement = "DELETE FROM category WHERE cat_name LIKE '" + catName + "'; ";
			this->simpleQuery(db, statement);

			//Closing database
			sqlite3_close(db);
		}//End of main else.
	}//End of deleteCategory