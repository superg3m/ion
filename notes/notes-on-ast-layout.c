

// Example infix
```
(bar + 1) * (foo - 1)
```
// Example AST (Postfix)
[
//Mul-#
  //Add-#
    bar
    1
  Add
  //Sub-#
    foo
    1
  Sub
Mul
]
// Notes:
// - if we made commented out nodes into actual nodes we could navigate in both directions
// - (bw and fw), doubly linked list style, our "default" postfix only allows backward navigation
// - Similarly, if tree was Prefix, we'd only have the commented out nodes, meaning only forward would be possible
// In both cases, we distinguish nodes in:
// | Leaf(value, No-Children/Descendant hypothetical subtree_size=1)
// | NonLeaf(subtree_size: inclusive of header node)


//
// Documenting/Clarifying AST (Postfix-only) traversal API
//

u32 ast_node_get_subtree_size(Ast* ast, AstNode* node)
{
    DEBUG_ASSERT(ast && node && node >= &ast->nodes[0] && node < &ast->nodes[ast->node_cap]);
    return ast_node_kind_is_leaf(node->kind) ? 1 : node->subtree_size;
}

AstNode* ast_node_get_last_child(Ast* ast, AstNode* node)
{
    DEBUG_ASSERT(ast && node && node > &ast->nodes[0] && node < &ast->nodes[ast->node_cap]);
    DEBUG_ASSERT(!ast_node_kind_is_leaf(node.kind) && node.subtree_size > 1);
    
    return (node - 1);
}

Nullable(AstNode*) ast_node_get_prev_sibling_or_null(Ast* ast, AstNode* parent, AstNode* child)
{
    DEBUG_ASSERT(ast && parent && !ast_node_kind_is_leaf(parent) && child && child < parent && child > &ast->nodes[0] && parent < &ast->nodes[ast->node_cap]);

    u32 child_subtree_size = ast_node_get_subtree_size(node);
    AstNode* hyp_prev_sibl = (node - subtree_size);
    if (hyp_prev_sibl < (parent - parent.subtree_size)) {
        return NULL;
    }

    DEBUG_ASSERT(prev_sibl >= &ast->nodes[0]);
    return hyp_prev_sibl;
}


// Example API usage
// say we want to iter on `node`'s children (and we have the `ast`)
// (of course node must be non-leaf, i.e. it must have children)
DEBUG_ASSERT(!ast_node_kind_is_leaf(node->kind));

for (
    AstNode* child = ast_node_get_last_child(ast, node);
    child != NULL;
    child = ast_node_get_prev_sibling_or_null(ast, node, child);
) {
    // ... do things with each child
}


// Note that Prefix-only API would be symmetric:
// - `get_first_child`
// - `get_next_sibling_or_null`
