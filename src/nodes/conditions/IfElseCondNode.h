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

    bool analyzeSemantic() override
    {
        // TODO::add scopes

        if (!(cond->analyzeSemantic() && ifBody->analyzeSemantic()))
            return false;

        if (cond->type == DTYPE_VOID)
            return false;

        if (elseBody != nullptr)
        {
            if (!elseBody->analyzeSemantic())
                return false;
        }

        return true;
    }

    string generateCode() override
    {
        string quad;
        string l1 = "1";
        quad = cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " L" + l1 + "\n";
        quad += ifBody->generateCode();

        if (elseBody != nullptr)
        {
            string l2 = "2";
            quad += Utils::opToQuad(OPR_JMP, cond->type) + " L" + l2 + "\n";
            quad += "L" + l1 + ":\n";
            quad += elseBody->generateCode();
            quad += "L" + l2 + ":\n";
        }
        else
        {
            quad += "L" + l1 + ":\n";
        }

        return quad;
    }
};
#endif