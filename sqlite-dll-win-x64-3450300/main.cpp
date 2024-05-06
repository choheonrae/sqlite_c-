#include <iostream>
#include <sqlite3.h>

int main() {
    sqlite3* db;
    int rc = sqlite3_open("sj.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM grade;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    int totalScore = 0;
    int count = 0;
    int mathTotal = 0;
    int englishTotal = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int score1 = sqlite3_column_int(stmt, 4);
        int score2 = sqlite3_column_int(stmt, 5);

        totalScore += score1 + score2;
        mathTotal += score1;
        englishTotal += score2;
        count++;

        std::cout << "Name: " << name << ", Total Score: " << (score1 + score2) << std::endl;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error reading data: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    double average = static_cast<double>(totalScore) / (count * 2);
    double mathAverage = static_cast<double>(mathTotal) / count;
    double englishAverage = static_cast<double>(englishTotal) / count;

    std::cout << "Total Score: " << totalScore << ", Average Score: " << average << std::endl;
    std::cout << "Math Total: " << mathTotal << ", Math Average: " << mathAverage << std::endl;
    std::cout << "English Total: " << englishTotal << ", English Average: " << englishAverage << std::endl;

    return 0;
}
