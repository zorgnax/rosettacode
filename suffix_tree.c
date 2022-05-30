#include <stdio.h>
#include <stdlib.h>

// based on this article: https://www.cise.ufl.edu/~sahni/dsaaj/enrich/c16/suffix.htm

typedef struct node_t node_t;
struct node_t {
    int index1;
    int index2;
    node_t *parent;
    node_t *child;
    node_t *next;
};

char S[] = "ababbabbaabbabb#";
int n = sizeof(S) - 1;

// Each node in the tree stores a reference to the first child, then each child
// stores a reference to their next sibling, each child also stores a reference
// to their parent
void add_child (node_t *parent, node_t *child) {
    node_t *node;
    if (parent->child) {
        for (node = parent->child; node->next; node = node->next) {
            // ok
        }
        node->next = child;
    }
    else {
        parent->child = child;
    }
    child->parent = parent;
}

void swap_child (node_t *parent, node_t *child1, node_t *child2) {
    if (parent->child == child1) {
        parent->child = child2;
    }
    else {
        for (node_t *node = parent->child; node; node = node->next) {
            if (node->next == child1) {
                node->next = child2;
                break;
            }
        }
    }
}

void print_tree (node_t *node, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    int len = node->index2 - node->index1;
    if (len) {
        printf("%.*s", len, S + node->index1);
    }
    else {
        printf("-");
    }
    printf(" %d %d\n", node->index1, node->index2);
    for (node_t *child = node->child; child; child = child->next) {
        print_tree(child, depth + 1);
    }
}

void print_node (node_t *node) {
    printf("node [%d, %d]", node->index1, node->index2);
    if (node->child) {
        printf(" has child");
    }
    if (node->next) {
        printf(" has sibling");
    }
    if (node->parent) {
        printf(" has parent");
    }
    printf("\n");
}

int find_mismatched_node (node_t *node, int index, node_t **mismatched_node, node_t **parent) {
    int i;
    for (i = 0; i < node->index2 - node->index1 && i < n - index; i++) {
        if (S[index + i] != S[node->index1 + i]) {
            *mismatched_node = node;
            return i;
        }
    }
    if (i == n - index) {
        *mismatched_node = node;
        return i;
    }
    *parent = node;
    for (node_t *child = node->child; child; child = child->next) {
        int offset = find_mismatched_node(child, index + i, mismatched_node, parent);
        if (offset) {
            return offset;
        }
    }
    *mismatched_node = node;
    return i;
}

int main (int argc, char **argv) {
    printf("hello world!\n");
    printf("S is %s n is %d\n", S, n);

    node_t *root = calloc(1, sizeof(node_t));
    root->index1 = 0;
    root->index2 = 0;

    for (int index = 0; index < n; index++) {
        printf("index is %d\n", index);

        node_t *mismatched_node, *parent;
        int offset = find_mismatched_node(root, index, &mismatched_node, &parent);

        if (mismatched_node->index1 + offset < mismatched_node->index2) {
            node_t *branch = calloc(1, sizeof(node_t));
            branch->index1 = mismatched_node->index1;
            branch->index2 = mismatched_node->index1 + offset;
            branch->parent = mismatched_node->parent;
            branch->child = mismatched_node;
            branch->next = mismatched_node->next;

            node_t *new_node = calloc(1, sizeof(node_t));
            new_node->index1 = index + offset;
            new_node->index2 = n;
            new_node->parent = branch;

            mismatched_node->index1 += offset;
            mismatched_node->parent = branch;
            mismatched_node->next = new_node;

            swap_child(branch->parent, mismatched_node, branch);
        }
        else {
            node_t *new_node = calloc(1, sizeof(node_t));
            new_node->index1 = index + offset;
            new_node->index2 = n;
            add_child(mismatched_node, new_node);
        }

        print_tree(root, 0);
        if (index == 2) {
            break;
        }
    }
    return 0;
}

