#include "SymbolTable.h"


EntryInfo* SymbolTable::insert(yy::location loc, const std::string &name, DataType type, EntryType entryType, const std::vector<DataType> &paramsTypes, int used, bool initialized)
{
    if (table.find(name) != table.end())
        return nullptr;
    table[name] = EntryInfo(loc, type, entryType, paramsTypes, used, initialized);
    return &(table[name]);
}

EntryInfo* SymbolTable::lookup(const std::string& name)
{
    if (table.find(name) == table.end())
        return nullptr;
    return &(table[name]);
}

std::unordered_map<std::string, EntryInfo> SymbolTable::getTable()
{
    return table;
}
