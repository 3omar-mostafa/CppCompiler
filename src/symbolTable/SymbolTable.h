#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "../utils/enums.h"
#include "location.hpp"

struct EntryInfo
{
    DataType type;
    EntryType entryType;
    std::vector<DataType> paramsTypes; // params of function
    yy::location loc;
    int used;
    bool initialized;

    EntryInfo() = default;

    explicit EntryInfo(yy::location loc, DataType type, EntryType entryType = TYPE_VAR, const std::vector<DataType> &paramsTypes = {}, int used = 0, bool initialized = false)
    {
        this->loc = loc;
        this->type = type;
        this->entryType = entryType;
        this->paramsTypes = paramsTypes;
        this->used = used;
        this->initialized = initialized;
    }
};

class SymbolTable
{
private:
    std::unordered_map<std::string, EntryInfo> table;
    // static SymbolTable *symbolTable;

public:
    // static SymbolTable *getInstance();
    SymbolTable(){};

    bool insert(yy::location loc, const std::string &name, DataType type, EntryType entryType = TYPE_VAR,
                const std::vector<DataType> &paramsTypes = {}, int used = 0, bool initialized = false);

    std::unordered_map<std::string, EntryInfo> getTable();

    bool lookup(const std::string &name, EntryInfo *&info);
};

#endif //__SYMBOL_TABLE__