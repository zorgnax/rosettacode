#include <stdio.h>
#include <stdlib.h>

enum {
    RED, BLACK
};

typedef struct node_t node_t;
struct node_t {
    node_t *parent;
    node_t *left;
    node_t *right;
    char color;
    int value;
    int pos;
};

typedef struct queue_t queue_t;
struct queue_t {
    node_t *node;
    queue_t *next;
    queue_t *prev;
};

queue_t *push (queue_t *q, node_t *n) {
    queue_t *r = malloc(sizeof (queue_t));
    r->node = n;
    if (q) {
        r->next = q;
        r->prev = q->prev;
        r->prev->next = r;
        r->next->prev = r;
    }
    else {
        r->next = r;
        r->prev = r;
        q = r;
    }
    return q;
}

queue_t *shift (queue_t *q, node_t **n) {
    if (!q) {
        *n = NULL;
        return NULL;
    }
    *n = q->node;
    if (q->prev == q) {
        free(q);
        return NULL;
    }
    q->next->prev = q->prev;
    q->prev->next = q->next;
    queue_t *r = q;
    q = q->next;
    free(r);
    return q;
}

int height (node_t *n) {
    if (!n) {
        return 0;
    }
    int h1 = 1 + height(n->left);
    int h2 = 1 + height(n->right);
    return h1 > h2 ? h1 : h2;
}

int pow_ (int x, int n) {
    int i;
    int r = 1;
    for (i = 0; i < n; i++) {
        r *= x;
    }
    return r;
}

void print_tree2 (node_t *n) {
    if (!n) {
        printf("(empty)\n");
        return;
    }
    queue_t *q = NULL;
    int row1 = 1;
    int row2 = 0;
    int h = height(n);
    int pos = 0;
    int i;
    n->pos = 0;
    for (i = 1; i < h; i++) {
        n->pos += pow_(2, i);
    }
    while (n) {
        pos += printf("%*s", !pos || n->pos > pos ? n->pos - pos : 1, "");
        printf("%s", n->color == RED ? "\e[1;31m" : "");
        pos += printf("%d", n->value);
        printf("%s", n->color == RED ? "\e[0m" : "");
        if (n->left) {
            n->left->pos = n->pos - pow_(2, h - 1);
            q = push(q, n->left);
            row2++;
        }
        if (n->right) {
            n->right->pos = n->pos + pow_(2, h - 1);
            q = push(q, n->right);
            row2++;
        }
        row1--;
        if (!row1) {
            printf("\n");
            row1 = row2;
            row2 = 0;
            h--;
            pos = 0;
        }
        q = shift(q, &n);
    }
}

void print_tree (node_t *n, int depth) {
    if (!n) {
        if (!depth) {
            printf("%*s(empty)\n", depth * 4, "");
        }
        return;
    }
    printf("%*s%s%d%s\n",
        depth * 4, "",
        n->color == RED ? "\e[1;31m": "",
        n->value,
        n->color == RED ? "\e[0m" : "");
    print_tree(n->left, depth + 1);
    print_tree(n->right, depth + 1);
}

node_t *uncle (node_t *n) {
    if (!n || !n->parent || !n->parent->parent) {
        return NULL;
    }
    node_t *g = n->parent->parent;
    if (g->left == n->parent) {
        return g->right;
    }
    else {
        return g->left;
    }
}

node_t *insert_case1 (node_t *t, node_t *n);

node_t *rotate_left (node_t *t, node_t *n) {
    node_t *p = n->parent;
    node_t *c = n->right;
    if (p) {
        if (n == p->left) {
            p->left = c;
        }
        else {
            p->right = c;
        }
    }
    else {
        t = c;
    }
    c->parent = p;
    n->right = c->left;
    if (n->right) {
        n->right->parent = n;
    }
    c->left = n;
    n->parent = c;
    return t;
}

node_t *rotate_right (node_t *t, node_t *n) {
    node_t *p = n->parent;
    node_t *c = n->left;
    if (p) {
        if (n == p->left) {
            p->left = c;
        }
        else {
            p->right = c;
        }
    }
    else {
        t = c;
    }
    c->parent = p;
    n->left = c->right;
    if (n->left) {
        n->left->parent = n;
    }
    c->right = n;
    n->parent = c;
    return t;
}

node_t *insert_case5 (node_t *t, node_t *n) {
    node_t *g = n->parent->parent;
    node_t *p = n->parent;
    g->color = RED;
    p->color = BLACK;
    if (n == p->left) {
        printf("case 5 - rotate %d right\n", g->value);
        t = rotate_right(t, g);
    }
    else {
        printf("case 5 - rotate %d left\n", g->value);
        t = rotate_left(t, g);
    }
    return t;
}

node_t *insert_case4 (node_t *t, node_t *n) {
    node_t *g = n->parent->parent;
    node_t *p = n->parent;
    if (n == p->right && p == g->left) {
        printf("case 4 - rotate %d left\n", p->value);
        t = rotate_left(t, p);
        t = insert_case5(t, p);
    }
    else if (n == p->left && p == g->right) {
        printf("case 4 - rotate %d right\n", p->value);
        t = rotate_right(t, p);
        t = insert_case5(t, p);
    }
    else {
        t = insert_case5(t, n);
    }
    return t;
}

node_t *insert_case3 (node_t *t, node_t *n) {
    node_t *u = uncle(n);
    if (u && u->color == RED) {
        printf("case 3 - swap color upwards\n");
        n->parent->color = BLACK;
        u->color = BLACK;
        node_t *g = n->parent->parent;
        g->color = RED;
        t = insert_case1(t, g);
    }
    else {
        t = insert_case4(t, n);
    }
    return t;
}

node_t *insert_case2 (node_t *t, node_t *n) {
    if (n->parent->color == BLACK) {
        printf("case 2 - ok\n");
    }
    else {
        t = insert_case3(t, n);
    }
    return t;
}

node_t *insert_case1 (node_t *t, node_t *n) {
    if (!n->parent) {
        if (n->color == RED) {
            printf("case 1 - root is black\n");
            n->color = BLACK;
        }
        else {
            printf("case 1 - ok\n");
        }
    }
    else {
        t = insert_case2(t, n);
    }
    return t;
}

node_t *insert_normal (node_t *t, node_t *n) {
    if (!t) {
        return n;
    }
    node_t *p = t;
    while (1) {
        if (n->value < p->value) {
            if (p->left) {
                p = p->left;
            }
            else {
                p->left = n;
                n->parent = p;
                break;
            }
        }
        else {
            if (p->right) {
                p = p->right;
            }
            else {
                p->right = n;
                n->parent = p;
                break;
            }
        }
    }
    return t;
}

node_t *insert (node_t *t, int value) {
    node_t *n = calloc(1, sizeof (node_t));
    n->value = value;
    n->color = RED;
    t = insert_normal(t, n);
    print_tree2(t);
    t = insert_case1(t, n);
    print_tree2(t);
    printf("===================================\n");
    return t;
}

int main () {
    printf("red black tree\n");
    node_t *t = NULL;
    t = insert(t, 17);
    t = insert(t, 11);
    t = insert(t, 22);
    t = insert(t, 15);
    t = insert(t, 13);
    t = insert(t, 1);
    t = insert(t, 8);
    t = insert(t, 6);
    t = insert(t, 25);
    t = insert(t, 27);
    return 0;
}

