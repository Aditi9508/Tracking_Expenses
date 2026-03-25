#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class Expense {
public:
    char date[15];
    char category[20];
    float amount;
    char note[50];

    void input() {
        cout << "\n Date (dd-mm-yyyy): ";
        cin >> date;
        cout << " Category: ";
        cin >> category;
        cout << " Amount: ";
        cin >> amount;
        cin.ignore();
        cin.getline(note, 50);
        cout << "\n Expense Added Successfully!";
    }

    void display() const {
        cout << left << setw(12) << date << setw(15) << category << setw(10) << fixed << setprecision(2) << amount << endl;
    }
};

class ExpenseManager {
private:
    const string fileName = "expenses.dat";

public:
    void login() {
        string user, pass;
        cout << "================================" << endl;
        cout << "    LOGIN YOUR ACCOUNT" << endl;
        cout << "================================" << endl;
        cout << "Username: "; cin >> user;
        cout << "Password: "; cin >> pass;

        if (user == "admin" && pass == "1234") {
            cout << "\n Login Successful!\n";
        } else {
            cout << "\n Invalid Credentials!";
            exit(0);
        }
    }

    void addExpense() {
        Expense e;
        e.input();
        ofstream outFile(fileName, ios::binary | ios::app);
        outFile.write(reinterpret_cast<char*>(&e), sizeof(Expense));
        outFile.close();
    }

    void viewExpenses() {
        Expense e;
        ifstream inFile(fileName, ios::binary);
        if (!inFile) {
            cout << "\nNo data found!";
            return;
        }
        cout << "\n--- EXPENSE RECORDS ---\n";
        cout << left << setw(12) << "DATE" << setw(15) << "CATEGORY" << setw(10) << "AMOUNT" << endl;
        while (inFile.read(reinterpret_cast<char*>(&e), sizeof(Expense))) {
            e.display();
        }
        inFile.close();
    }

    void searchByCategory(string key) {
        Expense e;
        bool found = false;
        ifstream inFile(fileName, ios::binary);
        while (inFile.read(reinterpret_cast<char*>(&e), sizeof(Expense))) {
            if (string(e.category) == key) {
                e.display();
                found = true;
            }
        }
        if (!found) cout << "\n No expenses found in this category.";
        inFile.close();
    }

    void monthlyReport(string month) {
        Expense e;
        float total = 0;
        ifstream inFile(fileName, ios::binary);
        while (inFile.read(reinterpret_cast<char*>(&e), sizeof(Expense))) {
            string d(e.date);
            if (d.substr(3, 2) == month) {
                total += e.amount;
            }
        }
        cout << "\n Total for month " << month << ": " << total << endl;
        inFile.close();
    }
};

int main() {
    ExpenseManager em;
    em.login();

    int choice;
    while (true) {
        cout << "\n================================" << endl;
        cout << "    EXPENSE TRACKER MENU" << endl;
        cout << "================================" << endl;
        cout << "1. Add Expense\n2. View All\n3. Search Category\n4. Monthly Report\n5. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: em.addExpense(); break;
            case 2: em.viewExpenses(); break;
            case 3: {
                string cat;
                cout << "Enter Category: "; cin >> cat;
                em.searchByCategory(cat);
                break;
            }
            case 4: {
                string m;
                cout << "Enter Month (mm): "; cin >> m;
                em.monthlyReport(m);
                break;
            }
            case 5: return 0;
            default: cout << "\nInvalid choice!";
        }
    }
    return 0;
}
