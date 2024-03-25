#include <iostream>
#include <iomanip> 
#include "functions.h"
using namespace std;

void getBankInfo(string& name, string& address, int& branchCount, float& capital) {
    cout << "Enter bank name:" << endl;
    cin >> name;
    cout << "Enter bank address:" << endl;
    cin >> address;
    cout << "Enter number of filials:" << endl;
    cin >> branchCount;
    cout << "Enter bank capital:" << endl; 
    cin >> capital;
}

void getBranchInfo(int& bankId, string& location, int& employeesCount, string& openingDate) {
    cout << "Enter bank ID: ";
    cin >> bankId;
    cout << "Enter location: ";
    cin >> location;
    cout << "Enter number of employees: ";
    cin >> employeesCount;
    cout << "Enter opening date (DD/MM/YYYY): ";
    cin >> openingDate;
}

void getOptions() {
    cout << "Vasylenko Zlata K-24" << endl;
    cout << "Select an option:" << endl;
    cout << setw(3) << "1" << " ---> Add bank" << endl;
    cout << setw(3) << "2" << " ---> Remove bank" << endl;
    cout << setw(3) << "3" << " ---> Edit bank" << endl;
    cout << setw(3) << "4" << " ---> Add filial" << endl;
    cout << setw(3) << "5" << " ---> Remove filial" << endl;
    cout << setw(3) << "6" << " ---> Edit filial" << endl;
    cout << setw(3) << "7" << " ---> Get list of all banks" << endl;
    cout << setw(3) << "8" << " ---> Get list of all filials" << endl;
    cout << setw(3) << "9" << " ---> Get list of all filials for specific bank" << endl;
    cout << setw(3) << "10" << " ---> Exit" << endl;
}
