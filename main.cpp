#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctime> // for date handling
#include "functions.h"
using namespace std;

struct Bank {
    char name[40];
    char address[100];
    int branchCount;
    float capital; 
};

struct Branch {
    int branchId;
    int bankId;
    char location[100];
    int employeesCount;
    char openingDate[11]; // DD/MM/YYYY format for opening date
};

 int bankIdCounter = 0; // Глобальний лічильник id для банків
 int branchIdCounter = 0; // Глобальний лічильник id для філіалів


// Function to validate date format (DD/MM/YYYY)
bool isValidDateFormat(const string& date) {
    if (date.size() != 10) // Checking length
        return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) {
            if (date[i] != '/')
                return false;
        } else {
            if (!isdigit(date[i]))
                return false;
        }
    }
    return true;
}

Bank getBank(int position) {
    Bank bank;
    FILE* bankDB = fopen("banks.bin", "rb");
    fseek(bankDB, position * sizeof(Bank), SEEK_SET);
    fread(&bank, sizeof(Bank), 1, bankDB);
    fclose(bankDB);
    return bank;
}

void writeBank(Bank& bank, int position) {
    FILE* bankDB = fopen("banks.bin", "rb+");
    fseek(bankDB, position * sizeof(Bank), SEEK_SET);
    fwrite(&bank, sizeof(Bank), 1, bankDB);
    fclose(bankDB);
}

Branch getBranch(int position) {
    Branch branch;
    FILE* branchDB = fopen("branches.bin", "rb+");
    fseek(branchDB, position * sizeof(Branch), SEEK_SET);
    fread(&branch, sizeof(Branch), 1, branchDB);
    fclose(branchDB);
    return branch;
}

void writeBranch(Branch& branch, int position) {
    FILE* branchDB = fopen("branches.bin", "rb+");
    fseek(branchDB, position * sizeof(Branch), SEEK_SET);
    fwrite(&branch, sizeof(Branch), 1, branchDB);
    fclose(branchDB);
}

int getAmountOfBanks() {
    FILE* bankDB = fopen("banks.bin", "rb");
    if (bankDB == nullptr) {
        return 0;
    }
    fseek(bankDB, 0, SEEK_END);
    int count = ftell(bankDB) / sizeof(Bank);
    fclose(bankDB);
    return count;
}

int getAmountOfBranches() {
    FILE* branchDB = fopen("branches.bin", "rb");
    if (branchDB == nullptr) {
        return 0;
    }
    fseek(branchDB, 0, SEEK_END);
    int count = ftell(branchDB) / sizeof(Branch);
    fclose(branchDB);
    return count;
}

void addBank(Bank& bank, int& bankIdCounter, float initialCapital, int initialBranchCount) {
    bank.branchCount = initialBranchCount; // Встановлення початкової кількості філіалів
    bank.capital = initialCapital; // Початковий капітал
    bankIdCounter++; // Збільшуємо лічильник id для наступного банку
    int bankId = bankIdCounter; // Присвоюємо id банку
    FILE* bankDB = fopen("banks.bin", "ab+");
    fwrite(&bank, sizeof(Bank), 1, bankDB);
    fclose(bankDB);
}


void addBranch(Branch& branch) {
    branchIdCounter++; // Increment the branch ID counter
    branch.branchId = branchIdCounter; // Assign the new branch ID
    FILE* branchDB = fopen("branches.bin", "ab+");
    fwrite(&branch, sizeof(Branch), 1, branchDB);
    fclose(branchDB);

    // Now we need to update the branch count for the corresponding bank
    Bank bank = getBank(branch.bankId - 1); // Get the bank associated with the branch
    bank.branchCount++; // Increment the branch count
    writeBank(bank, branch.bankId - 1); // Write the updated bank back to file
}



void deleteBank(int id) {
    int amount = getAmountOfBranches();
    for (int i = 0; i < amount; ++i) {
        Branch branch = getBranch(i);
        if (branch.bankId == id) {
            branch.branchId = -1;
            writeBranch(branch, i);
        }
    }
    Bank bank = getBank(id - 1);
    bank.name[0] = '\0';
    writeBank(bank, id - 1);
    cout << "Bank removed along with its branches" << endl;
}

void deleteBranch(int id) {
    Branch branch = getBranch(id - 1);
    branch.branchId = -1;
    writeBranch(branch, id - 1);
}

void editBank(int id, string newName, string newAddress, int newBranchCount, float newCapital) {
    Bank bank = getBank(id - 1);
    strcpy(bank.name, newName.c_str());
    strcpy(bank.address, newAddress.c_str());
    bank.branchCount = newBranchCount;
    bank.capital = newCapital; 
    writeBank(bank, id - 1);
    cout << "Bank information updated successfully" << endl;
}

void editBranch(int id, string newLocation, int newEmployeesCount, string newDate) {
    Branch branch = getBranch(id - 1);
    strcpy(branch.location, newLocation.c_str());
    branch.employeesCount = newEmployeesCount;
    if (isValidDateFormat(newDate))
        strcpy(branch.openingDate, newDate.c_str());
    else {
        cout << "Invalid date format. Date format should be DD/MM/YYYY." << endl;
        return;
    }
    writeBranch(branch, id - 1);
    cout << "Branch information updated successfully" << endl;
}

void listBranchesForBank(int bankId) {
    int amount = getAmountOfBranches();
    bool foundBranch = false; 
    cout << "List of branches for bank with ID " << bankId << ":" << endl;
    for (int i = 0; i < amount; ++i) {
        Branch branch = getBranch(i);
        if (branch.bankId == bankId && branch.branchId != -1) {
            foundBranch = true; 
            cout << "Branch ID: " << i + 1 << endl;
            cout << "Location: " << branch.location << endl;
            cout << "Number of employees: " << branch.employeesCount << endl;
            cout << "Opening Date: " << branch.openingDate << endl << endl;
        }
    }
    if (!foundBranch) {
        cout << "There are no branches for the selected bank." << endl; 
    }
}
int getAmountOfBranchesForBank(int bankId) {
    int amount = getAmountOfBranches();
    int count = 0;
    for (int i = 0; i < amount; ++i) {
        Branch branch = getBranch(i);
        if (branch.bankId == bankId && branch.branchId != -1) {
            count++;
        }
    }
    return count;
}

int main() {

    Bank bank;
    Branch branch;
    int choice = 0;
    int id, bankId, employeesCount, amount;
    string name, address, location;
    float capital; 
    string openingDate;
   int bankIdCounter = 0;

    getOptions();
    while (true) {
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Wrong input, try again" << endl;
        }
    switch (choice) {
     case 1:
        getBankInfo(name, address, bank.branchCount,capital);
        strcpy(bank.name, name.c_str());
        strcpy(bank.address, address.c_str());
       addBank(bank, bankIdCounter, capital, bank.branchCount);
        cout << "Bank successfully created with ID " << bankIdCounter << "." << endl; // виведення ID банку
        break;


case 2:
    amount = getAmountOfBanks();
    if (amount == 0) {
        cout << "There are no established banks." << endl;
        break;
    }
    cout << "Enter id of the bank you want to remove" << endl;
    cin >> id;
    if (id <= 0 || id > amount || getBank(id - 1).name[0] == '\0') {
        cout << "Bank with ID " << id << " does not exist." << endl;
        break;
    }
    deleteBank(id);
    break;

case 5:
    amount = getAmountOfBranches();
    if (amount == 0) {
        cout << "There are no established branches." << endl;
        break;
    }
    cout << "Enter id of the branch you want to remove" << endl;
    cin >> id;
    if (id <= 0 || id > amount || getBranch(id - 1).branchId == -1) {
        cout << "Branch with ID " << id << " does not exist." << endl;
        break;
    }
    deleteBranch(id);
    break;

case 4:
{
    amount = getAmountOfBanks();
    if (amount == 0) {
        cout << "There are no established banks." << endl;
        break;
    }
    int bankId;
    string location, openingDate;
    int employeesCount;
    getBranchInfo(bankId, location, employeesCount, openingDate);
    if (bankId <= 0 || bankId > amount || getBank(bankId - 1).name[0] == '\0') {
        cout << "Bank with ID " << bankId << " does not exist." << endl;
        break;
    }
    Bank bank = getBank(bankId - 1);
    int maxBranches = bank.branchCount;
    int existingBranches = getAmountOfBranchesForBank(bankId);
    if (existingBranches >= maxBranches) {
        cout << "Cannot add more branches. Maximum branches limit reached for bank with ID " << bankId << endl;
        break;
    }

    Branch branch;
    branch.bankId = bankId;
    strcpy(branch.location, location.c_str());
    branch.employeesCount = employeesCount;
    strcpy(branch.openingDate, openingDate.c_str());
    addBranch(branch);

    // Оновлюємо кількість філіалів для банку та записуємо оновлену інформацію в файл
    bank.branchCount++;
    writeBank(bank, bankId - 1);

    cout << "Branch successfully created with ID " << branch.branchId << "." << endl;
    break;
}



case 7:
    amount = getAmountOfBanks();
    if (amount == 0) {
        cout << "There are no established banks." << endl;
    } else {
        bool hasBanks = false;
        cout << "List of all banks:" << endl;
        for (int i = 0; i < amount; ++i) {
            bank = getBank(i);
            if (bank.name[0] != '\0') {
                hasBanks = true;
                cout << "Bank ID: " << i + 1 << endl;
                cout << "Name: " << bank.name << endl;
                cout << "Address: " << bank.address << endl;
                cout << "Number of filials: " << bank.branchCount << endl; 
                cout << "Capital: " << bank.capital << endl<< endl;
            }
        }
        if (!hasBanks) {
            cout << "The list of banks is empty." << endl;
        }
    }
    break;

case 8:
    amount = getAmountOfBranches();
    if (amount == 0) {
        cout << "There are no established branches." << endl;
    } else {
        bool hasBranches = false;
        cout << "List of all branches:" << endl;
        for (int i = 0; i < amount; ++i) {
            branch = getBranch(i);
            if (branch.branchId != -1) {
                hasBranches = true;
                cout << "Filial ID: " << i + 1 << endl;
                cout << "Bank ID: " << branch.bankId << endl;
                cout << "Location: " << branch.location << endl;
                cout << "Number of employees: " << branch.employeesCount << endl << endl;
            }
        }
        if (!hasBranches) {
            cout << "The list of branches is empty." << endl;
        }
    }
    break;

case 3:
    amount = getAmountOfBanks();
    if (amount == 0) {
        cout << "There are no established banks." << endl;
        break;
    }
    cout << "Enter bank ID to edit:" << endl;
    cin >> id;
    if (id <= 0 || id > amount || getBank(id - 1).name[0] == '\0') {
        cout << "Bank with ID " << id << " does not exist." << endl;
        break;
    }
    cout << "Enter new name, address,filial count and capital:" << endl;
    cin >> name >> address >> bank.branchCount >>capital;
    editBank(id, name, address, bank.branchCount,capital);
    break;

case 6:
    amount = getAmountOfBranches();
    if (amount == 0) {
        cout << "There are no established filials." << endl;
        break;
    }
    cout << "Enter filial ID to edit:" << endl;
    cin >> id;
    if (id <= 0 || id > amount || getBranch(id - 1).branchId == -1) {
        cout << "Branch with ID " << id << " does not exist." << endl;
        break;
    }
    cout << "Enter new location, number of employees, and opening date (DD/MM/YYYY):" << endl;
    cin >> location >> employeesCount >> openingDate; 
    editBranch(id, location, employeesCount, openingDate); 
    break;


case 9:
    amount = getAmountOfBanks();
    if (amount == 0) {
        cout << "There are no established banks." << endl;
        break;
    }
    cout << "Enter bank ID to list branches:" << endl;
    cin >> bankId;
    if (bankId <= 0 || bankId > amount || getBank(bankId - 1).name[0] == '\0') {
        cout << "Bank with ID " << bankId << " does not exist." << endl;
        break;
    }
    listBranchesForBank(bankId);
    break;

case 10:
    return 0;

default:
    cout << "Wrong input!" << endl;
    break;
        }
        cout << "What do you want to do next?" << endl;
        cout<< "1" << " ---> Add bank" << endl;
    cout  << "2" << " ---> Remove bank" << endl;
    cout << "3" << " ---> Edit bank" << endl;
    cout << "4" << " ---> Add filial" << endl;
    cout  << "5" << " ---> Remove filial" << endl;
    cout << "6" << " ---> Edit filial" << endl;
    cout  << "7" << " ---> Get list of all banks" << endl;
    cout  << "8" << " ---> Get list of all filials" << endl;
    cout  << "9" << " ---> Get list of all filials for specific bank" << endl;
    cout  << "10" << " ---> Exit" << endl;
    }
}