#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "../utils/enums.h"

using namespace std;

struct EntryInfo
{
    DataType type;
    EntryType entryType;
    vector<DataType> paramsTypes;
    EntryInfo() {}
    EntryInfo(DataType type, EntryType entryType = TYPE_VAR, const vector<DataType> &paramsTypes = {})
    {
        this->type = type;
        this->entryType = entryType;
        this->paramsTypes = paramsTypes;
    }
};

class SymbolTable
{
private:
    unordered_map<string, EntryInfo> table;
    static SymbolTable *symbolTable;

    SymbolTable() {}

public:
    bool insert(const string &name, DataType type, EntryType entryType = TYPE_VAR, const vector<DataType> &paramsTypes = {});
    bool update();
    bool lookup(const string &name, EntryInfo &info);
    static SymbolTable *getTable();
};

#endif