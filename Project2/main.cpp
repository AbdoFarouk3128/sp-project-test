#include <iostream>
#include "sqlite3.h"
#include <string>

using namespace std;

struct workout {
    int id, set, reps;

    string Name;

    float duration;

    string exercises[10];
};

struct measurement {
    float weight,
        height,
        bodyfat,
        bmi,
        musclemass,
        caloriburn;
};

struct client; // forward declaration (needed now for trainer)

struct trainer {
    int id;
    string name, password, username, description;
    client* oldclients[30];      // Use pointers to avoid recursive size issues
    client* clientrequest[20];
};

struct client {
    int id, workoutplans[10], age, counter_days;
    string username, password, progresslogs;
    measurement measurements[100];
    trainer* Trainer;            // Also use pointer here
    workout workouts[6];
    char gender;
};



sqlite3* opendatabase() {
    sqlite3* db = nullptr;
    const char* dbPath = "database.db";

    if (sqlite3_open(dbPath, &db) == SQLITE_OK) {
        cout << "Database opened successfully: " << dbPath << endl;
    }
    else {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
    }

    return db;
}


bool validateLogin(sqlite3* db, string& username, string password, string userType) {
    string table = (userType == "Client") ? "Clients" : "Trainers";
    string sql = "SELECT COUNT(*) FROM " + table + " WHERE username = '" + username + "' AND password = '" + password + "';";


    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // QMessageBox::critical(nullptr, "Query Error", "Failed to prepare SQL statement.");
        return false;
    }

    int result = sqlite3_step(stmt);
    bool valid = (result == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0);

    sqlite3_finalize(stmt);
    return valid;
}

double calculateBMR(double weight, double height, int age, string gender)
{
    if (gender == "male" || gender == "Male")//Õ”«» BMR(„⁄œ· «·Õ—ﬁ «·«”«”Ì)
        return 10 * weight + 6.25 * height - 5 * age + 5;

    else
        return 10 * weight + 6.25 * height - 5 * age - 161;

}

double  activityMulti(string level)//Õ”«» „⁄œ· «·‰‘«ÿ
{
    if (level == "sedentary")
        return 1.2;

    else if (level == "light")
        return 1.375;

    else if (level == "moderate")
        return 1.55;

    else if (level == "active")
        return 1.725;

    else if (level == "very active")
        return 1.9;

    else//come back
        return 1.2;

}

double calculatetdee(double bmr, double activityMulti)//Õ”«» «·”⁄—«  «·Õ—«—ÌÂ
{
    return bmr * activityMulti;
}

struct Macros {
    double protein, carbs, fats;
};

Macros calcmacros(double tdee) {
    //Õ”«» «·»—Ê Ì‰ 30%„‰ tdee 
    //«·ﬁ”„Â ⁄·Ï 4 ⁄‘«‰   ÕÊ· „‰ ”⁄—«  Õ—«—ÌÂ ·Ã—«„«  »—Ê Ì‰

    Macros m;
    m.protein = (tdee * 0.3) / 4;
    m.carbs = (tdee * 0.45) / 4;
    m.fats = (tdee * 0.25) / 9;
    return m;
}


double CalculateBmi(float weight, float heightCm) {
    float heightM = heightCm / 100;
    float bmi = weight / (heightM * heightM);
    return bmi;
}

string interpretBMI(double bmi) {
    if (bmi < 18.5)
        return "Underweight";

    else if (bmi < 25)
        return "Normal weight ";

    else if (bmi < 30)
        return "Overweight";

    else
        return "seriously overweight";
}

void logout() {
    cout << "you have been logged out" << endl;
    //login();
}

int main() {
    sqlite3* db; // Database pointer
    db = opendatabase();

    string username = "mohamedn";
    string password = "Moh@med123!";

    if (validateLogin(db, username, password, "Client")) {
        cout << "Login Successful. Welcome, " << username << " !" << endl;
    }
    else {
        cout << "Login Failed. Invalid username or password." << endl;
    }

    // Close database if opened successfully
    if (db) {
        sqlite3_close(db);
        cout << "Database closed." << endl;
    }

    return 0;
}
