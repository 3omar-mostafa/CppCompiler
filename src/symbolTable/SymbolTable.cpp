#include "SymbolTable.h"

// SymbolTable *SymbolTable::symbolTable = nullptr;

bool SymbolTable::insert(const std::string &name, DataType type, EntryType entryType, const std::vector<DataType> &paramsTypes)
{
    if (table.find(name) != table.end())
        return false;
    table[name] = EntryInfo(type, entryType, paramsTypes);
    return true;
}

bool SymbolTable::lookup(const std::string &name, EntryInfo &info)
{
    if (table.find(name) == table.end())
        return false;
    info = table[name];
    return true;
}

// SymbolTable *SymbolTable::getInstance()
// {
//     if (symbolTable == nullptr)
//     {
//         symbolTable = new SymbolTable();
//     }
//     return symbolTable;
// }
