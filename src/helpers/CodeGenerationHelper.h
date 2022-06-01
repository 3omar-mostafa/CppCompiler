#ifndef GENERATION_HELPER
#define GENERATION_HELPER

#include <stack>
#include <string>
using namespace std;

class CodeGenerationHelper
{
    int labelNum;
    stack<string> continueLabelStack;
    stack<string> breakLabelStack;

public:
    CodeGenerationHelper()
    {
        labelNum = 1;
    }
    string getNewLabel()
    {
        return std::to_string(labelNum++);
    }

    void addContinueLabel(string label)
    {
        continueLabelStack.push(label);
    }
    void addBreakLabel(string label)
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
    string getContinueLabel()
    {
        return continueLabelStack.top();
    }
    string getBreakLabel()
    {
        return breakLabelStack.top();
    }
};

#endif