#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

typedef struct BTreeNode {
    int keys;
    int count;
    struct BTreeNode *children[MAX + 1];
} Node;

Node *root = NULL;

// Create a new node
Node *createNode(int key, Node *child) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->keys[1] = key;
    newNode->count = 1;
    newNode->children[0] = root;
    newNode->children[1] = child;
    return newNode;
}

// Add a key to the node
void addToNode(Node *node, int key, int pos, Node *child) {
    int i = node->count;
    while (i > pos) {
        node->keys[i + 1] = node->keys[i];
        node->children[i + 1] = node->children[i];
        i--;
    }
    node->keys[pos + 1] = key;
    node->children[pos + 1] = child;
    node->count++;
}

// Split a full node
void splitNode(int key, int *pKey, int pos, Node *node, Node *child, Node **newNode) {
    int mid = (pos > MIN) ? MIN + 1 : MIN;
    *newNode = (Node *)malloc(sizeof(Node));

    int i = mid + 1, j = 1;
    while (i <= MAX) {
        (*newNode)->keys[j] = node->keys[i];
        (*newNode)->children[j - 1] = node->children[i - 1];
        j++, i++;
    }
    (*newNode)->children[j - 1] = node->children[i - 1];

    node->count = mid;
    (*newNode)->count = MAX - mid;

    if (pos <= MIN)
        addToNode(node, key, pos, child);
    else
        addToNode(*newNode, key, pos - mid, child);

    *pKey = node->keys[node->count];
    (*newNode)->children[0] = node->children[node->count];
    node->count--;
}

// Insert recursively
int insertInternal(int key, int *pKey, Node *node, Node **child) {
    int pos;
    if (node == NULL) {
        *pKey = key;
        *child = NULL;
        return 1;
    }

    for (pos = node->count; pos >= 1 && key < node->keys[pos]; pos--);
    if (key == node->keys[pos]) {
        printf("Duplicate key: %d not allowed\n", key);
        return 0;
    }

    if (insertInternal(key, pKey, node->children[pos], child)) {
        if (node->count < MAX) {
            addToNode(node, *pKey, pos, *child);
            return 0;
        } else {
            splitNode(*pKey, pKey, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Public insert function
void insert(int key) {
    int pKey;
    Node *child;
    if (insertInternal(key, &pKey, root, &child)) {
        root = createNode(pKey, child);
    }
}

// In-order traversal
void inorder(Node *node) {
    if (node != NULL) {
        for (int i = 0; i < node->count; i++) {
            inorder(node->children[i]);
            printf("%d ", node->keys[i + 1]);
        }
        inorder(node->children[node->count]);
    }
}

// Search function
void search(Node *node, int key) {
    if (node == NULL) {
        printf("%d Not Found\n", key);
        return;
    }

    int pos;
    for (pos = 1; pos <= node->count && key > node->keys[pos]; pos++);

    if (pos <= node->count && key == node->keys[pos]) {
        printf("%d Found\n", key);
        return;
    }

    search(node->children[pos - 1], key);
}

// Utility to find successor
int getSuccessor(Node *node) {
    while (node->children[0] != NULL)
        node = node->children[0];
    return node->keys[1];
}

// Remove from leaf/internal node
int deleteInternal(Node *node, int key) {
    if (!node) return 0;

    int pos;
    for (pos = 1; pos <= node->count && key > node->keys[pos]; pos++);

    if (pos <= node->count && key == node->keys[pos]) {
        if (node->children[pos - 1]) {
            int succ = getSuccessor(node->children[pos]);
            node->keys[pos] = succ;
            deleteInternal(node->children[pos], succ);
        } else {
            for (int i = pos; i < node->count; i++) {
                node->keys[i] = node->keys[i + 1];
                node->children[i] = node->children[i + 1];
            }
            node->count--;
        }
        return 1;
    } else {
        return deleteInternal(node->children[pos - 1], key);
    }
}

// Public delete function
void deleteNode(int key) {
    if (!deleteInternal(root, key)) {
        printf("Key %d not found\n", key);
    }
    if (root && root->count == 0) {
        Node *oldRoot = root;
        root = root->children[0];
        free(oldRoot);
    }
}

int main() {
    root = NULL;

    insert(10);
    insert(20);
    insert(5);
    insert(6);
    insert(12);
    insert(30);
    insert(7);
    insert(17);

    printf("Inorder traversal: ");
    inorder(root);
    printf("\n");

    search(root, 6);
    search(root, 12);

    deleteNode(6);
	search(root, 6);
	
    printf("Inorder after deletion: ");
    inorder(root);
    printf("\n");

    return 0;
}

