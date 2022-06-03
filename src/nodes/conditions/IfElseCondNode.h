#ifndef IF_COND_NODE
#define IF_COND_NODE
#include "../Node.h"

class IfElseCondNode : public Node
{
    ExpressionNode *cond;
    Node *ifBody;
    Node *elseBody;

public:
    IfElseCondNode(yy::location loc, ExpressionNode *cond, Node *ifBody, Node *elseBody = nullptr) : Node(loc)
    {
        this->cond = cond;
        this->ifBody = ifBody;
        this->elseBody = elseBody;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (scopeHelper->isInsideGlobalScope())
        {
            Utils::log("if statement is not allowed in global scope", loc, "error");
            return false;
        }

        bool check = true;
        scopeHelper->pushScope(SCOPE_IF, this);

        if (!(cond->analyzeSemantic(true) && ifBody->analyzeSemantic()))
            check &= false;

        if (cond->type == DTYPE_VOID)
        {
            Utils::log("invalid conversion from '" + Utils::typeToQuad(cond->type) + "' to 'bool'", cond->loc, "error");
            check &= false;
        }

        if (elseBody != nullptr)
        {
            if (!elseBody->analyzeSemantic())
                check &= false;
        }

        scopeHelper->popScope();

        return check;
    }

    string generateCode() override
    {
        /*
        -------------COND------------
        JMPZ L1
        -------------BODY------------

        L1:
        ------COND-NOT-SATISFIED-----

        =============================
        --------------OR-------------
        =============================

        -------------COND------------
        JMPZ L1
        -------------BODY------------
        JMP L2

        L1: (ELSE)
        ------COND-NOT-SATISFIED-----

        L2:
        ------COND-SATISFIED-EXIT----

        */

        auto genHelper = CodeGenerationHelper::getInstance();

        string quad;
        string l1 = genHelper->getNewLabel();
        quad = cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " " + l1 + "\n";
        quad += ifBody->generateCode();

        if (elseBody != nullptr)
        {
            string l2 = genHelper->getNewLabel();
            quad += Utils::opToQuad(OPR_JMP, cond->type) + " " + l2 + "\n";
            quad += l1 + ":\n";
            quad += elseBody->generateCode();
            quad += l2 + ":\n";
        }
        else
        {
            quad += l1 + ":\n";
        }

        return quad;
    }

    ~IfElseCondNode() override
    {
        delete cond;
        delete ifBody;
        delete elseBody;
        cond = nullptr;
        ifBody = nullptr;
        elseBody = nullptr;
    }
};
#endif