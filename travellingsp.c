#include <stdio.h>
#include <stdlib.h>

char *cities[] = {
    "Moscow",
    "St. Petersburg",
    "Novosibirsk",
    "Yekaterinburg",
    "Nizhny Novgorod",
    "Samara",
    "Omsk",
    "Kazan",
    "Chelyabinsk",
    "Rostov-on-Don",
    "Ufa",
    "Volgograd",
};

int graph[][12] = {
    0,    395,  1748, 879,  246,  534,  1385, 443,  933,  597,  724,  566,
    395,  0,    1932, 1108, 702,  1100, 1967, 950,  1188, 957,  1015, 959,
    1748, 1932, 0,    869,  1823, 1321, 406,  1577, 973,  2315, 1241, 2021,
    879,  1108, 869,  0,    828,  484,  585,  588,  132,  1391, 317,  1097,
    246,  702,  1823, 828,  0,    331,  1142, 204,  686,  650,  482,  527,
    534,  1100, 1321, 484,  331,  0,    939,  188,  475,  624,  254,  401,
    1385, 1967, 406,  585,  1142, 939,  0,    950,  472,  1538, 687,  1293,
    443,  950,  1577, 588,  204,  188,  950,  0,    486,  715,  280,  525,
    933,  1188, 973,  132,  686,  475,  472,  486,  0,    1085, 222,  848,
    597,  957,  2315, 1391, 650,  624,  1538, 715,  1085, 0,    878,  247,
    724,  1015, 1241, 317,  482,  254,  687,  280,  222,  878,  0,    644,
    566,  959,  2021, 1097, 527,  401,  1293, 525,  848,  247,  644,  0,
};

int n = sizeof (cities) / sizeof (char *);
int *path;
int *set;
int *best;
int best_dist = 0;

void travellingsp (int i, int dist) {
    int j, k;
    if (i == n) {
        dist += graph[path[i - 1]][0];
        if (!best_dist || best_dist > dist) {
            best_dist = dist;
            for (j = 0; j < n; j++) {
                best[j] = path[j];
            }
        }
        return;
    }
    for (j = 0; j < n; j++) {
        if (!set[j]) {
            set[j] = 1;
            path[i] = j;
            travellingsp(i + 1, dist + graph[path[i - 1]][j]);
            set[j] = 0;
        }
    }
}

int main () {
    path = calloc(n, sizeof (int));
    set = calloc(n, sizeof (int));
    best = calloc(n, sizeof (int));
    path[0] = 0;
    set[0] = 1;
    travellingsp(1, 0);
    int i;
    for (i = 0; i < n; i++) {
        printf("%s%s", cities[best[i]], i == n - 1 ? "\n" : ", ");
    }
    printf("best distance: %d\n", best_dist);
}

