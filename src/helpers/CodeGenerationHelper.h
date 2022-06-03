#ifndef GENERATION_HELPER
#define GENERATION_HELPER

#include <stack>
#include <string>

class CodeGenerationHelper
{
    int labelNum = 1;
    std::stack<std::string> continueLabelStack;
    std::stack<std::string> breakLabelStack;

    CodeGenerationHelper() = default;

public:

    static CodeGenerationHelper* getInstance()
    {
        static CodeGenerationHelper instance;
        return &instance;
    }

    std::string getNewLabel()
    {
        return "L" + std::to_string(labelNum++);
    }

    void addContinueLabel(const std::string& label)
    {
        continueLabelStack.push(label);
    }

    void addBreakLabel(const std::string& label)
    {
        breakLabelStack.push(label);
    }

    void removeContinueLabel()
    {
        continueLabelStack.pop();
    }

    void removeBreakLabel()
    {
        breakLabelStack.pop();
    }

    std::string getContinueLabel()
    {
        return continueLabelStack.top();
    }

    std::string getBreakLabel()
    {
        return breakLabelStack.top();
    }
};

#endif