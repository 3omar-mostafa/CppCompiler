#ifndef WHILE_LOOP_NODE
#define WHILE_LOOP_NODE
#include "../Node.h"

class WhileLoopNode : public Node
{
    ExpressionNode *cond;
    Node *body;

public:
    WhileLoopNode(yy::location loc, ExpressionNode *cond, Node *body) : Node(loc)
    {
        this->cond = cond;
        this->body = body;
    }

    bool analyzeSemantic() override
    {
        // TODO::add scopes

        if (!(cond->analyzeSemantic() && body->analyzeSemantic()))
            return false;

        if (cond->type == DTYPE_VOID)
            return false;

        return true;
    }

    string generateCode() override
    {
        string quad;
        string l1 = "1";
        string l2 = "2";
        quad = "L" + l1 + ":\n";
        quad += cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " L" + l2 + "\n";
        quad += body->generateCode();
        quad += Utils::opToQuad(OPR_JMP, cond->type) + " L" + l1 + "\n";
        quad += "L" + l2 + ":\n";

        return quad;
    }
};
#endif