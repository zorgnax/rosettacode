#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Based on this article: https://www.cise.ufl.edu/~sahni/dsaaj/enrich/c16/suffix.htm
//
// There is some efficiency to be gained by using suffix pointers on each of the
// branch nodes, which wasn't explained so clearly in the article, but should make
// sense intuitively if I were to try to implement that myself. Each branch would
// link to another branch that contains it's suffix, so you can ignore comparing up
// to the index1 of that branch node when inserting the next suffix into the tree.

typedef struct node_t node_t;
struct node_t {
    int index1;
    int index2;
    int suffix;
    node_t *child;
    node_t *next;
};

char *S;
int n;

// Each node in the tree stores a reference to the first child, then each child
// stores a reference to their next sibling
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
}

void print_tree (node_t *node, int depth, int highlight) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    if (!node->child) {
        if (node->suffix == highlight) {
            printf("\e[1;36m");
        }
        else {
            printf("\e[1;32m");
        }
    }
    int len = node->index2 - node->index1;
    printf("%.*s", len, S + node->index1);
    printf(" %d %d %d\n", node->index1, node->index2, node->suffix);
    if (!node->child) {
        printf("\e[0m");
    }
    for (node_t *child = node->child; child; child = child->next) {
        print_tree(child, depth + 1, highlight);
    }
    if (depth == 0) {
        printf("\n");
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
    printf("\n");
}

int find_mismatched_node (node_t *node, int index, node_t **mismatched, node_t **parent) {
    int i;
    for (i = 0; i < node->index2 - node->index1 && i < n - index; i++) {
        if (S[index + i] != S[node->index1 + i]) {
            *mismatched = node;
            return i;
        }
    }
    if (i == n - index) {
        *mismatched = node;
        return i;
    }
    *parent = node;
    for (node_t *child = node->child; child; child = child->next) {
        int offset = find_mismatched_node(child, index + i, mismatched, parent);
        if (offset) {
            return offset;
        }
    }
    *mismatched = node;
    return i;
}

int main (int argc, char **argv) {
    printf("hello world!\n");

    S = "ababbabbaabbabb";
    S = "the quick brown fox jumps over the lazy dog.";
    S = "zuzzy zazz";
    S = "peeper";
    S = "ababab";
    S = "aaaaaa";
    S = "abcab";
    if (argc > 1) {
        S = argv[1];
    }
    n = strlen(S);

    printf("S is %s n is %d\n", S, n);

    node_t *node;
    node_t *root = calloc(1, sizeof(node_t));
    root->index1 = 0;
    root->index2 = 0;

    for (int index = 0; index < n; index++) {
        printf("suffix %d is %.*s\e[1;36m%.*s\e[0m\n", index, index, S, n - index, S + index);

        node_t *mismatched, *parent;
        int offset = find_mismatched_node(root, index, &mismatched, &parent);

        node_t *new_node = calloc(1, sizeof(node_t));
        new_node->index1 = mismatched->index1 + offset - mismatched->suffix + index;
        new_node->index2 = n;
        new_node->suffix = index;

        if (mismatched->index1 + offset < mismatched->index2) {
            node_t *branch = calloc(1, sizeof(node_t));

            if (parent->child == mismatched) {
                parent->child = branch;
            }
            else {
                for (node = parent->child; node; node = node->next) {
                    if (node->next == mismatched) {
                        node->next = branch;
                        break;
                    }
                }
            }

            branch->index1 = mismatched->index1;
            branch->index2 = mismatched->index1 + offset;
            branch->child = mismatched;
            branch->next = mismatched->next;
            branch->suffix = mismatched->suffix;
            mismatched->index1 += offset;
            mismatched->next = new_node;
        }
        else {
            add_child(mismatched, new_node);
        }

        print_tree(root, 0, index);
    }
    return 0;
}

