/* boolean satisfiability problem. basic NP-complete problem. not actually on rosetta code. generates the formulas and figures out if they are satisfiable. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int formula_size = 4;
int variables_count = 3;

enum {
    PAREN,
    OR,
    AND,
    NOT,
    VAR,
};

typedef struct node_t node_t;
struct node_t {
    int type;
    int value;
    int index;
    int len;
    node_t *next;
    node_t *child;
};

void add_child (node_t *node, node_t *node2) {
    if (!node->child) {
        node->child = node2;
        return;
    }
    node_t *node3;
    for (node3 = node->child; node3->next; node3 = node3->next)
        ;
    node3->next = node2;
}

int parse_formula (char *formula, int i, node_t **node);

int parse_paren (char *formula, int i, node_t **node) {
    int j = i;
    if (formula[j++] != '(')
        return 0;
    node_t *node2;
    *node = calloc(1, sizeof (node_t));
    (*node)->type = PAREN;
    (*node)->index = i;
    parse_formula(formula, j, &node2);
    j += node2->len;
    add_child(*node, node2);
    if (formula[j] != ')') {
        printf("error parsing formula at index %d\n", j);
        exit(1);
    }
    j++;
    (*node)->len = j - i;
    return 1;
}

int parse_and (char *formula, int i, node_t **node) {
    node_t *node2;
    *node = calloc(1, sizeof (node_t));
    (*node)->type = AND;
    (*node)->index = i;
    int j;
    for (j = i; formula[j]; j++) {
        if (formula[j] == ' ') {
        }
        else if (parse_paren(formula, j, &node2)) {
            j += node2->len - 1;
            add_child(*node, node2);
        }
        else if (formula[j] == '!' && formula[j + 1] >= 'a' && formula[j + 1] <= 'z') {
            node2 = calloc(1, sizeof (node_t));
            node2->type = NOT;
            node2->value = formula[j + 1] - 'a';
            node2->index = j;
            node2->len = 2;
            add_child(*node, node2);
            j++;
        }
        else if (formula[j] >= 'a' && formula[j] <= 'z') {
            node2 = calloc(1, sizeof (node_t));
            node2->type = VAR;
            node2->value = formula[j] - 'a';
            node2->index = j;
            node2->len = 1;
            add_child(*node, node2);
        }
        else if (formula[j] == '&' && formula[j + 1] == '&') {
            j++;
        }
        else {
            break;
        }
    }
    (*node)->len = j - i;
    if (!(*node)->len) {
        free(*node);
        return 0;
    }
    return 1;
}

int parse_formula (char *formula, int i, node_t **node) {
    node_t *node2;
    *node = calloc(1, sizeof (node_t));
    (*node)->type = OR;
    (*node)->index = i;
    int j;
    for (j = i; formula[j]; j++) {
        if (formula[j] == ' ') {
        }
        else if (parse_and(formula, j, &node2)) {
            j += node2->len - 1;
            add_child(*node, node2);
        }
        else if (formula[j] == '|' && formula[j + 1] == '|') {
            j++;
        }
        else {
            break;
        }
    }
    (*node)->len = j - i;
    return 1;
}

int add_formula_part (char *formula, int formula_len, int formula_index,
                      int operation, int parentheses, int not,
                      int variable, char **formula2) {
    if (!formula_index) {
        return 0;
    }
    int i = 0;
    char formula3[9];
    if (operation == 1) {
        strncpy(formula3, " && ", 4);
        i += 4;
    }
    if (operation == 2) {
        strncpy(formula3, " || ", 4);
        i += 4;
    }
    if (parentheses == 1) {
        formula3[i++] = '(';
    }
    if (not) {
        formula3[i++] = '!';
    }
    formula3[i++] = variable + 'a';
    if (parentheses == 2) {
        formula3[i++] = ')';
    }
    int formula2_len = formula_len + i;
    *formula2 = malloc(formula2_len + i + 1);
    strncpy(*formula2, formula, formula_len);
    strncpy(*formula2 + formula_len, formula3, i);
    (*formula2)[formula2_len] = '\0';
    return formula2_len;
}

void print_node (node_t *node, int depth, char *formula) {
    for (; node; node = node->next) {
        char *type = node->type == PAREN ? "PAREN"
                   : node->type == OR    ? "OR"
                   : node->type == AND   ? "AND"
                   : node->type == NOT   ? "NOT"
                   : node->type == VAR   ? "VAR"
                   :                       "???";

        printf("%*s%s", depth * 4, "", type);
        if (node->type == VAR || node->type == NOT)
            printf(" %c", node->value + 'a');
        printf(" \"%.*s\"", node->len, formula + node->index);
        printf("\n");
        print_node(node->child, depth + 1, formula);
    }
}

void free_node (node_t *node) {
    while (node) {
        node_t *node2 = node->next;
        if (node->child)
            free_node(node->child);
        free(node);
        node = node2;
    }
}

int sat (node_t *node, int *vars) {
    node_t *node2;
    if (node->type == OR) {
        for (node2 = node->child; node2; node2 = node2->next) {
            if (sat(node2, vars)) {
                return 1;
            }
        }
        return 0;
    }
    else if (node->type == AND) {
        for (node2 = node->child; node2; node2 = node2->next) {
            if (!sat(node2, vars)) {
                return 0;
            }
        }
        return 1;
    }
    else if (node->type == PAREN) {
        return sat(node->child, vars);
    }
    else if (node->type == NOT) {
        return !vars[node->value];
    }
    else if (node->type == VAR) {
        return vars[node->value];
    }
    else {
        return 0;
    }
}

void find_sat (char *formula, node_t *node) {
    int i, j, m = 0;
    for (i = 0; formula[i]; i++) {
        if (formula[i] >= 'a' && formula[i] <= 'z') {
            if (m < formula[i] - 'a') {
                m = formula[i] - 'a';
            }
        }
    }
    int *vars = malloc(m + 1);
    for (i = 0; i < 2 << m; i++) {
        for (j = 0; j < m + 1; j++) {
            vars[j] = (i >> (m - j)) & 1;
        }
        if (sat(node, vars)) {
            printf("%s: 1 (", formula);
            for (j = 0; j < m + 1; j++) {
                printf("%s%d", j ? " " : "", vars[j]);
            }
            printf(")\n");
            free(vars);
            return;
        }
    }
    printf("%s: 0\n", formula);
    free(vars);
}

void recurse_formula (char *formula, int formula_len, int formula_index, int open);

void create_formula (char *formula, int formula_len, int formula_index,
                     int operation, int open, int parentheses, int not,
                     int variable) {
    char *formula2 = NULL;
    int formula2_len = add_formula_part(formula, formula_len, formula_index, operation, parentheses, not, variable, &formula2);
    if (formula_index == formula_size) {
        node_t *node;
        parse_formula(formula2, 0, &node);
        find_sat(formula2, node);
        free_node(node);
        free(formula2);
        return;
    }
    recurse_formula(formula2, formula2_len, formula_index, open);
}


void recurse_formula (char *formula, int formula_len, int formula_index, int open) {
    int i, j, k, l, o;
    for (i = 0; i < variables_count; i++) {
        for (j = 0; j < 3; j++) {
            if (formula_index == formula_size - 1 && j == 1)
                continue;
            if (formula_index == 0 && j == 2)
                continue;
            if (open && j == 1)
                continue;
            if (!open && j == 2)
                continue;
            if (open && formula_index == formula_size - 1 && j != 2)
                continue;
            if (open == formula_size - 2 && j != 2)
                continue;
            int o = j == 0 && open ? open + 1 : j == 1 ? 1 : 0;
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 3; l++) {
                    if (l == 0 && formula_index != 0)
                        continue;
                    if (l != 0 && formula_index == 0)
                        continue;
                    create_formula(formula, formula_len, formula_index + 1, l, o, j, k, i);
                }
            }
        }
    }
    free(formula);
}


int main () {
    recurse_formula(NULL, 0, 0, 0);
    return 0;
}

