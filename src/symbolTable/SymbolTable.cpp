#include "SymbolTable.h"

// SymbolTable *SymbolTable::symbolTable = nullptr;

bool SymbolTable::insert(yy::location loc, const std::string &name, DataType type, EntryType entryType, const std::vector<DataType> &paramsTypes, int used, bool initialized)
{
    if (table.find(name) != table.end())
        return false;
    table[name] = EntryInfo(loc, type, entryType, paramsTypes, used, initialized);
    return true;
}

bool SymbolTable::lookup(const std::string &name, EntryInfo *&info)
{
    if (table.find(name) == table.end())
        return false;
    info = &(table[name]);
    return true;
}

std::unordered_map<std::string, EntryInfo> SymbolTable::getTable()
{
    return table;
}
// SymbolTable *SymbolTable::getInstance()
// {
//     if (symbolTable == nullptr)
//     {
//         symbolTable = new SymbolTable();
//     }
//     return symbolTable;
// }
