#include"BudgetClass.h"

// Constructor
Budget::Budget() {
    // Attempt to open the database
    if (sqlite3_open_v2("dataBase.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Failed to open or create the database.");
    }
}


// Get items of a specific budget
void Budget::getItems(std::string budgetName) throw(std::invalid_argument) {
    if (budgetName.empty()) {
        throw std::invalid_argument("Budget name cannot be empty.");
    }

    sqlite3_stmt* stmt;
    const char* query = "SELECT name, total, cap FROM items WHERE budget_name = ?";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL statement.");
    }

    sqlite3_bind_text(stmt, 1, budgetName.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double total = sqlite3_column_double(stmt, 1);
        double cap = sqlite3_column_double(stmt, 2);
        std::cout << "Item: " << name << ", Total: " << total << ", Cap: " << cap << std::endl;
    }

    sqlite3_finalize(stmt);
}


// Get total of a specific budget
double Budget::getTotal(std::string budgetName) throw(std::invalid_argument) {
    if (budgetName.empty()) {
        throw std::invalid_argument("Budget name cannot be empty.");
    }

    sqlite3_stmt* stmt;
    const char* query = "SELECT SUM(total) FROM items WHERE budget_name = ?";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL statement.");
    }

    sqlite3_bind_text(stmt, 1, budgetName.c_str(), -1, SQLITE_STATIC);
    double total = 0.0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}


// Get all budgets
std::string Budget::getBudgets() const {
    std::string budgets;
    sqlite3_stmt* stmt;
    const char* query = "SELECT DISTINCT budget_name FROM items";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL statement.");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!budgets.empty()) {
            budgets += ", ";
        }
        budgets += reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return budgets;
}


// Add a new budget with items
void Budget::addBudget(std::string name, double total, std::vector<item>& items) throw(std::invalid_argument) {
    if (name.empty()) {
        throw std::invalid_argument("Budget name cannot be empty.");
    }

    sqlite3_stmt* stmt;
    const char* insert_budget = "INSERT INTO budgets (name, total) VALUES (?, ?)";
    const char* insert_item = "INSERT INTO items (name, total, cap, budget_name) VALUES (?, ?, ?, ?)";

    // Start transaction
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    // Insert the budget
    if (sqlite3_prepare_v2(db, insert_budget, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing insert statement for budget: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error preparing SQL insert statement for budget.");
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, total);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting budget: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error inserting budget into database.");
    }

    sqlite3_finalize(stmt);

    // Insert each item
    for (const auto& it : items) {
        if (sqlite3_prepare_v2(db, insert_item, -1, &stmt, NULL) != SQLITE_OK) {
            std::cerr << "Error preparing insert statement for item: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
            throw std::runtime_error("Error preparing SQL insert statement for item.");
        }

        sqlite3_bind_text(stmt, 1, it.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, it.total);
        sqlite3_bind_double(stmt, 3, it.cap);
        sqlite3_bind_text(stmt, 4, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error inserting item: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
            throw std::runtime_error("Error inserting item into database.");
        }

        sqlite3_finalize(stmt);
    }

    // Commit transaction
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
}


// Delete a budget and its associated items
void Budget::deleteBudget(std::string name) throw(std::invalid_argument) {
    if (name.empty()) {
        throw std::invalid_argument("Budget name cannot be empty.");
    }

    const char* delete_budget = "DELETE FROM budgets WHERE name = ?";
    const char* delete_items = "DELETE FROM items WHERE budget_name = ?";

    // Start transaction
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    sqlite3_stmt* stmt;

    // Delete items
    if (sqlite3_prepare_v2(db, delete_items, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing delete statement for items: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error preparing SQL delete statement for items.");
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting items: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error deleting items from database.");
    }

    sqlite3_finalize(stmt);

    // Delete budget
    if (sqlite3_prepare_v2(db, delete_budget, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing delete statement for budget: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error preparing SQL delete statement for budget.");
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting budget: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error deleting budget from database.");
    }

    sqlite3_finalize(stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
}


// Add a new item to a specific budget
void Budget::addItem(std::string budgetName, item newItem) throw(std::invalid_argument) {
    if (budgetName.empty() || newItem.name.empty()) {
        throw std::invalid_argument("Budget name and item name cannot be empty.");
    }

    const char* insert_item = "INSERT INTO items (name, total, cap, budget_name) VALUES (?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insert_item, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing insert statement for item: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL insert statement for item.");
    }

    sqlite3_bind_text(stmt, 1, newItem.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, newItem.total);
    sqlite3_bind_double(stmt, 3, newItem.cap);
    sqlite3_bind_text(stmt, 4, budgetName.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting item: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error inserting item into database.");
    }

    sqlite3_finalize(stmt);
}


// Delete an item from a specific budget
void Budget::deleteItem(std::string itemName, std::string budgetName) throw(std::invalid_argument) {
    if (itemName.empty() || budgetName.empty()) {
        throw std::invalid_argument("Item name and budget name cannot be empty.");
    }

    const char* delete_item = "DELETE FROM items WHERE name = ? AND budget_name = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, delete_item, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing delete statement for item: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL delete statement for item.");
    }

    sqlite3_bind_text(stmt, 1, itemName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, budgetName.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting item: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error deleting item from database.");
    }

    sqlite3_finalize(stmt);
}


// Add funds to a specific budget
void Budget::addFunds(std::string name, double total) throw(std::invalid_argument) {
    if (name.empty()) {
        throw std::invalid_argument("Budget name cannot be empty.");
    }

    if (total < 0) {
        throw std::invalid_argument("Total must be a non-negative number.");
    }

    const char* update_budget = "UPDATE budgets SET total = total + ? WHERE name = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, update_budget, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing update statement for budget: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL update statement for budget.");
    }

    sqlite3_bind_double(stmt, 1, total);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error updating budget: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error updating budget in database.");
    }

    sqlite3_finalize(stmt);
}


// Add an expense to a specific item in a budget
void Budget::addExpense(std::string catName, std::string budgetName, std::string itemName, double total) throw(std::invalid_argument) {
    if (catName.empty() || budgetName.empty() || itemName.empty()) {
        throw std::invalid_argument("Category name, budget name, and item name cannot be empty.");
    }

    if (total < 0) {
        throw std::invalid_argument("Total must be a non-negative number.");
    }

    const char* update_item = "UPDATE items SET total = total - ? WHERE name = ? AND budget_name = ? AND category = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, update_item, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing update statement for item: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Error preparing SQL update statement for item.");
    }

    sqlite3_bind_double(stmt, 1, total);
    sqlite3_bind_text(stmt, 2, itemName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, budgetName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, catName.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error updating item: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error updating item in database.");
    }

    sqlite3_finalize(stmt);
}


// Add a new category to the database
void Budget::addCategory(std::string name) throw(std::invalid_argument) {
    if (name.empty()) {
        throw std::invalid_argument("Category name cannot be empty.");
    }

    const char* insert_category = "INSERT INTO categories (name) VALUES (?)";
    sqlite3_stmt* stmt;

    // Start transaction
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    if (sqlite3_prepare_v2(db, insert_category, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing insert statement for category: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error preparing SQL insert statement for category.");
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting category: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error inserting category into database.");
    }

    sqlite3_finalize(stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
}


// Delete a category from the database
void Budget::deleteCategory(std::string name) throw(std::invalid_argument) {
    if (name.empty()) {
        throw std::invalid_argument("Category name cannot be empty.");
    }

    const char* delete_category = "DELETE FROM categories WHERE name = ?";
    sqlite3_stmt* stmt;

    // Start transaction
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    if (sqlite3_prepare_v2(db, delete_category, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing delete statement for category: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error preparing SQL delete statement for category.");
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting category: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
        throw std::runtime_error("Error deleting category from database.");
    }

    sqlite3_finalize(stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
}
