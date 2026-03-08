tree *findNode(tree *root, int key) {
    if (root == NULL) return NULL;
    if (key == root->key) return root;
    if (key < root->key) return findNode(root->left, key);
    return findNode(root->right, key);
}

tree *findBrother(tree *root, int key) {
    tree *node = findNode(root, key);

    if (node == NULL) return NULL;
    if (node->parent == NULL) return NULL;

    tree *p = node->parent;
    if (p->left == node) return p->right;
    if (p->right == node) return p->left;
    return NULL;
}

