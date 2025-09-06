

// The parser operates on a stream of tokens to transform into a Tree ds (the AST)

typedef struct {
    int kind;

    union{
        struct AstNode *lhs, *rhs;
    
    };
} AstNode;

struct {
    AstNode* flat_nodes;


} Ast;

AstNode {
    kind
    
    union {
        int_literal_value

        op_node_count;
    }
}

enum {

    ...
    IntLit,

    AddOp,
    ...
}

IsLeaf(x) x <= IntLit
IsList(x) !IsLeaf(x)

Two kind classes:
- LeafNode : 1
- ListNode : N

// Infix: ```int a = (1 + 2) * 3;```
// Infix: ```int b = (1 + 2) * 3;```
//  Mul(Add(1, 2), 3)
//  
//  [MulOp<4> (AddOp<2> (IntLit<1> IntLit<2>) IntLit<3>)]

[
    VarDecl<#> VarName<"a"> MulOp<4> (AddOp<2> (IntLit<1> IntLit<2>) IntLit<3>), 
    VarDecl<#> VarName<"b"> MulOp<4> (AddOp<2> (IntLit<1> IntLit<2>) IntLit<3>) 
]

[
    VarDecl<#>
        VarName<"a">
            MulOp[
                GroupOp[ AddOp[ IntLit<1> IntLit<2> ]]
                IntLit<3>
            ]
]

MulOp[
    GroupOp[
        AddOp[ 
            IntLit<1>, 
            IntLit<2> 
        ]
    ],
    IntLit<3>
]

eval(root_ast)

    while np < max_node_len {
        eval_stmt(env, np)
        np += cur.node_count
    }

def eval_stmt(env, subtree_root)

    switch (subtree_root.kind)
    {
        case IntLit : return subtree_root.int_literal_value
        case AdddOp : {
            Node* lhs = (subtree_root + 1)
            Node* rhs = (subtree_root + IsList(lhs) ? lhs.node_count : 1 )
            
            lhs_val = eval(lhs)
            rhs_val = eval(rhs)
            env.rt_stack.push(lhs_val + rhs_val)
        }
    }
    


flat_nodes[len(flat_nodes) - 1]




// Expression
//  └── Logical (||, &&)
//       └── Comparison (==, !=, <, >, etc.)
//            └── Additive (+, -, |, ^)
//                 └── Multiplicative (*, /, %, <<, >>)
//                      └── Unary (+, -, !, ~, &, *)
//                           └── Primary (literals, identifiers, etc.)








