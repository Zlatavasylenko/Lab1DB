#ifndef BANK_FUNCTIONS_H
#define BANK_FUNCTIONS_H
#include <string>
using namespace std;

void getBankInfo(string& name, string& address, int& branchCount, float& capital);
void getBranchInfo(int& bankId, string& location, int& employeesCount, string& openingDate);
void getOptions();
void editBank(int id, string newName, string newAddress, int newBranchCount, float capital);
void editBranch(int id, string newLocation, int newEmployeesCount,  string openingDate);
void listBranchesForBank(int bankId);

#endif //BANK_FUNCTIONS_H
