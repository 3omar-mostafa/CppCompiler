#include "SymbolTable.h"

SymbolTable *SymbolTable::symbolTable = nullptr;

bool SymbolTable::insert(const string &name, DataType type, EntryType entryType, const vector<DataType> &paramsTypes)
{
    if (table.find(name) != table.end())
        return false;
    table[name] = EntryInfo(type, entryType, paramsTypes);
    return true;
}

bool SymbolTable::lookup(const string &name, EntryInfo &info)
{
    if (table.find(name) == table.end())
        return false;
    info = table[name];
    return true;
}

SymbolTable *SymbolTable::getTable()
{
    if (symbolTable == nullptr)
    {
        symbolTable = new SymbolTable();
    }
    return symbolTable;
}
