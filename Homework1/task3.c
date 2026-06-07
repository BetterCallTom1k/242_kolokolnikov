#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    int **mat = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(m * sizeof(int));
        for (int j = 0; j < m; j++)
            scanf("%d", &mat[i][j]);
    }

    int **tr = malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
        tr[i] = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            tr[j][i] = mat[i][j];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (j) printf(" ");
            printf("%d", tr[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) free(mat[i]);
    free(mat);
    for (int i = 0; i < m; i++) free(tr[i]);
    free(tr);
    return 0;
}
