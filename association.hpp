// /*
// 23/2/25
// */

// #include "ast.hpp"

// // Flattening nested operators, ((((2 + 2) + 3) * 4 ) * 4 ) -> ((2 + 2 + 3) * 4 * 4)

// void associativeTransformAST(ExpressionNodeode)  {
//     // check for + or *, because associativity is only a property of these 
//     if (node.getKind() == InfixKind::PLUS || node.getKind() == InfixKind::MULTIPLY) { 
//         // start with operands of current node
//         std::vector<std::unique_ptr<ExpressionNode>> flattenedOperands;

//         // Flatten the left operand
//         flattenOperands(node.Left, node.getKind(), flattenedOperands);

//         // Flatten the right operand
//         flattenOperands(node.Right, node.getKind(), flattenedOperands);

//         // Similar nested operators so we go ahead with flattening
//         if (!flattenedOperands.empty()) {
//             // replace infix/binary with nary node 
//             // node = NaryExpressionNode(node.Tok, node.Operator, node.getKind(), std::move(flattenedOperands));
//         }
//     }

// }

// void flattenOperands(std::unique_ptr<ExpressionNode>& node, InfixKind kind, std::vector<std::unique_ptr<ExpressionNode>>& flattenedOperands) {
//     if (!node) return;
    
//     // If the node is an infix node and has the same operator as the parent node, recurse
//     if (node->getKind() == kind) {
//         auto infixNode = dynamic_cast<InfixExpressionNode*>(node.get());
//         if (infixNode) {
//             // Recursively flatten the left operand
//             flattenOperands(infixNode->Left, kind, flattenedOperands);
            
//             //Recursively flatten the right operand
//             flattenOperands(infixNode->Right, kind, flattenedOperands);
//             return;
//         }
//     } 

//     // if the operator is different, append it as an opperand
//     flattenedOperands.push_back(std::move(node));
    
// }