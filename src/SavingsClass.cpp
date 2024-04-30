//Savings Class Source File

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "SavingsClass.h"
#include "sqlite3.h"


	//Database calls.
	//Start a transaction in the database. Returns sqlite ok.
	int Savings::startTransaction(sqlite3* db) throw(std::runtime_error){
		std::string statement = "BEGIN TRANSACTION; ";

		//Sending statement to database.
		if (sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
			std::string error = "There was an error beginging transaction. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		return SQLITE_OK;
	}//End of startTransaction

	//Rolls database back if transaction faild. Returns sqlite ok.
	int Savings::rollback(sqlite3* db) throw(std::runtime_error) {
		std::string statement = "ROLLBACK; ";

		//Sending statement to database
		if (sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
			std::string error = "There was an error rolling back database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		return SQLITE_OK;
	}//End of rollback

	//Commits changes if everything went fine. Returns sqlite ok.
	int Savings::commit(sqlite3* db) throw(std::runtime_error) {
		std::string statement = "COMMIT; ";

		//Sending statement to database
		if (sqlite3_exec(db, statement.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
			std::string error = "There was an error commiting to database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
			rollback(db);
		}
		return SQLITE_OK;
	}//End of commit

	//Runs a simple query that needs no output. returns 0 for failed or 1 for success. 
	int Savings::simpleQuery(sqlite3* db, std::string query) throw(std::runtime_error) {

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

	//Returns savings id as a int. Needs savings name. Tested Working************
	int Savings::getSavingsID(std::string savingsName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		int id = 0;//Total to send back.

		//Checking name integrity.
		if (savingsName.size() == 0) {
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
			query = "SELECT sav_id FROM saving WHERE sav_name LIKE '";
			query += savingsName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in savings id query. ";
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


	//Constructor
	Savings::Savings() {

	}

	//Destructor
	Savings::~Savings() {

	}

	//Accessors
	//Returns a double for goal. Needs a savings name. Tested Working**************
	double Savings::getGoal(std::string savingsName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		double goal = 0;//Goal to send back.

		//Checking name integrity.
		if (savingsName.size() == 0) {
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
			query = "SELECT goal FROM saving WHERE sav_name LIKE '";
			query += savingsName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in savings goal query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				sqlite3_step(qres);//Step in.
				goal = sqlite3_column_double(qres, 0);//Set total from result
				sqlite3_finalize(qres);//Close result.
			}
		}//End of main else.

		//Closing database.
		sqlite3_close(db);

		return goal;
	}//End of getGoal

	//Returns a double for total. Needs a savings name. Tested Working***************
	double Savings::getTotal(std::string savingsName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields.
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		double total = 0;//Goal to send back.

		//Checking name integrity.
		if (savingsName.size() == 0) {
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
			query = "SELECT total FROM saving WHERE sav_name LIKE '";
			query += savingsName + "'; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in savings goal query. ";
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

	//Returns a list of savings as strings. Tested Working*************
	std::vector<std::string> Savings::getSavingsList() throw(std::runtime_error) {
		//Data fields
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<std::string> savingsList;//Vector of strings to return.

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
			query = "SELECT sav_name FROM saving; ";

			//Making query and checking for error.
			if (sqlite3_prepare_v2(db, query.c_str(), -1, &qres, NULL) != SQLITE_OK) {
				sqlite3_finalize(qres);
				std::string error = "There was an error in get savings names list query. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
			}
			else {
				//Filling in vector. Step into qres is in while loop.
				while (sqlite3_step(qres) == SQLITE_ROW) {
					savingsList.push_back((reinterpret_cast<const char*>(sqlite3_column_text(qres, 0))));
				}
				sqlite3_finalize(qres);//Close result.
			}//End of main else.

			//Closing database.
			sqlite3_close(db);
		}//End of main else.
		return savingsList;
	}//End of getSavingsList

	//Returns vector of doubles that are transaction amounts. Needs a savings name. Tested Working********************
	std::vector<double> Savings::getSavingsTransactionsList(std::string savingsName) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string query = "";//Query to send to database.
		sqlite3* db;//My database file.
		sqlite3_stmt* qres;//Results from query.
		std::vector<double> transactionList;//Vector of doubles to return.
		int savingsID = 0;//ID number of savings.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			//Getting savings ID number.
			savingsID = this->getSavingsID(savingsName);

			//Building query
			query = "SELECT total FROM transactions WHERE sav_id = " + std::to_string(savingsID) + "; ";

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


			//Closing database.
			sqlite3_close(db);
		}//End of main else.
		return transactionList;
	}//End of getBudgets

	//Mutators
	//Deletes a savings after setting all related transactions to sav_id = NULL. Needs a savings name. Tested Working**************
	void Savings::deleteSavings(std::string savingsName) throw(std::invalid_argument, std::runtime_error) {
		//Datafields
		std::string statement = "";//String being used for database statement.
		sqlite3* db;//My database file.
		int id = 0;//ID number for savings.

		//Checking name integrity.
		if (savingsName.size() == 0) {
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
			//Getting savings id.
			id = this->getSavingsID(savingsName);

			//Starting dataebase transaction.
			this->startTransaction(db);

			//Building statement to update all related transactions item_id to NULL.
			statement = "UPDATE transactions SET sav_id = NULL WHERE sav_id = " + std::to_string(id) + "; ";

			//Making changes to transactions table.
			this->simpleQuery(db, statement);

			//Creating statement to delete savings.
			statement = "DELETE FROM saving WHERE sav_name LIKE '" + savingsName + "'; ";

			//Making changes to item table.
			this->simpleQuery(db, statement);

			//Comminting changes and closing database.
			this->commit(db);
			sqlite3_close(db);
		}//End of main else.
	}//End of deleteSavings

	//Changes a savings goal. Needs savings name and new goal amount. Tested Working************
	void Savings::changeGoal(std::string savingsName, double newGoal) throw(std::invalid_argument, std::runtime_error) {
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
			if (savingsName.empty()) {
				throw std::invalid_argument("Name cannot be empty.\n");
			}
			else {
				//Building statement for goal change.
				dataBaseString = "UPDATE saving SET goal = " + std::to_string(newGoal) + " WHERE sav_name LIKE '" + savingsName + "'; ";

				//Sending statement to database.
				if (this->simpleQuery(db, dataBaseString) == 0) {
					std::string error = "There was an error updating savings goal. ";
					error += sqlite3_errmsg(db);
					error += "\n";
					throw std::runtime_error(error);
					rollback(db);
				}
				//Closing database.
				sqlite3_close(db);
			}//End of second else
		}//End of main else
	}//End of changeGoal.

	//Changes a savings total. Adds a transaction to reflect a deposit or withdraw. Needs savings name and new total amount. Tested Working************
	void Savings::changeTotal(std::string savingsName, double newTotal) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string dataBaseString = "";//String being used for database statement.
		sqlite3* db;//My database file.
		double currentTotal = 0;//Total of savings currently.
		int savingsID = 0;//ID number of savings

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::runtime_error(error);
		}
		else {
			if (savingsName.empty()) {
				throw std::invalid_argument("Name cannot be empty.\n");
			}
			else {
				//Getting current total.
				currentTotal = this->getTotal(savingsName);

				//Getting savings ID number.
				savingsID = this->getSavingsID(savingsName);

				//Building statement for total change.
				dataBaseString = "UPDATE saving SET total = " + std::to_string(newTotal) + " WHERE sav_name LIKE '" + savingsName + "'; ";

				//Starting transaction
				this->startTransaction(db);

				//Sending statement to database.
				if (this->simpleQuery(db, dataBaseString) == 0) {
					std::string error = "There was an error updating savings total. ";
					error += sqlite3_errmsg(db);
					error += "\n";
					throw std::runtime_error(error);
					rollback(db);
				}

				//If subtracting or adding
				if (currentTotal > newTotal) {
					//Building statement
					dataBaseString = "INSERT INTO transactions (total, item_id, sav_id, cat_name) VALUES (" + std::to_string(currentTotal - newTotal) + ", NULL, " + std::to_string(savingsID) + ", NULL); ";
					//Sending statement to database.
					if (this->simpleQuery(db, dataBaseString) == 0) {
						std::string error = "There was an error adding savings transaction subtracting. ";
						error += sqlite3_errmsg(db);
						error += "\n";
						throw std::runtime_error(error);
						rollback(db);
					}
				}
				else {
					//Building statement
					dataBaseString = "INSERT INTO transactions (total, item_id, sav_id, cat_name) VALUES (" + std::to_string(newTotal - currentTotal) + ", NULL, " + std::to_string(savingsID) + ", NULL); ";
					//Sending statement to database.
					if (this->simpleQuery(db, dataBaseString) == 0) {
						std::string error = "There was an error adding savings transaction adding. ";
						error += sqlite3_errmsg(db);
						error += "\n";
						throw std::runtime_error(error);
						rollback(db);
					}
				}

				//Commiting Changes
				this->commit(db);

				//Closing database.
				sqlite3_close(db);
			}//End of second else
		}//End of main else
	}//End of changeGoal.

	//Adds a savings. Needs a name and goal. Tested Working************
	void Savings::addSavings(std::string savingsName, double goal) throw(std::invalid_argument, std::runtime_error) {
		//Data fields
		std::string dataBaseString = "";//String being used for database statement.
		sqlite3* db;//My database file.

		//Opening database and checking for errors.
		if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
			sqlite3_close(db);
			std::string error = "Error opening database. ";
			error += sqlite3_errmsg(db);
			error += "\n";
			throw std::invalid_argument(error);
		}
		else {
			try {
				if (savingsName.empty()) {
					throw std::invalid_argument("Name cannot be empty.\n");
				}
				if (goal < 0) {
					throw std::invalid_argument("Goal cannot be a negative number.\n");
				}
			}
			catch (std::invalid_argument& e) {
				throw e.what();
			}

			//Building statement for budget name and starting total.
			dataBaseString = "INSERT INTO saving (sav_name, goal) VALUES('";
			dataBaseString += savingsName + "', " + std::to_string(goal);
			dataBaseString += "); ";
			//Inserting into budget table.
			if (this->simpleQuery(db, dataBaseString) == 0) {
				std::string error = "There was an error inserting into savings table. ";
				error += sqlite3_errmsg(db);
				error += "\n";
				throw std::runtime_error(error);
				rollback(db);
			}
			//Closing database.
			sqlite3_close(db);
		}//End of main else.
	}//End of addSavings