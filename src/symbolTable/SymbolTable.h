#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "../utils/enums.h"
#include "../utils/utils.h"
#include "location.hpp"
#include <sstream>
#include <iomanip>


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

    std::string to_string() const
    {
        std::string kind = (entryType == TYPE_VAR) ? "Variable" : (entryType == TYPE_CONST) ? "Constant" : "Function";

        std::stringstream ss;
        ss << std::left;
        ss << "datatype: " << std::setw(5) << Utils::typeToQuad(type) << ", ";
        ss << "kind: " << std::setw(8) << kind << ", ";
        ss << "is_used: " << std::setw(5) << (used ? "true" : "false") << ", ";
        ss << "is_initialized: " << std::setw(5) << (initialized ? "true" : "false") << ", ";
        ss << "location: " << loc;
        return ss.str();
    }
};

class SymbolTable
{
private:
    std::unordered_map<std::string, EntryInfo> table;

public:
    SymbolTable() = default;

    EntryInfo* insert(yy::location loc, const std::string& name, DataType type, EntryType entryType = TYPE_VAR,
                      const std::vector<DataType>& paramsTypes = {}, int used = 0, bool initialized = false);

    std::unordered_map<std::string, EntryInfo> getTable();

    EntryInfo* lookup(const std::string& name);
};

#endif //__SYMBOL_TABLE__