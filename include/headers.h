#include <iostream>
#include <string>
#include <cmath>

#include "../src/nodes/Node.h"
#include "../src/nodes/expressions/AssignOpNode.h"
#include "../src/nodes/expressions/BinaryOpNode.h"
#include "../src/nodes/expressions/IdentifierNode.h"
#include "../src/nodes/expressions/LiteralNode.h"
#include "../src/nodes/expressions/UnaryOpNode.h"

#include "../src/nodes/statements/VariableDeclarationNode.h"
#include "../src/nodes/statements/StmtBlockNode.h"
#include "../src/nodes/conditions/IfElseCondNode.h"
#include "../src/nodes/conditions/SwitchCondNode.h"
#include "../src/nodes/conditions/CaseNode.h"
#include "../src/nodes/loops/WhileLoopNode.h"
#include "../src/nodes/loops/DoWhileLoopNode.h"
#include "../src/nodes/loops/ForLoopNode.h"
#include "../src/nodes/loops/BreakNode.h"
#include "../src/nodes/loops/ContinueNode.h"

#include "../src/utils/enums.h"