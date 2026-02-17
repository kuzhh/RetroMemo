#include "archive.h"

int guardarRegistro(tPlayer *playerReg)
{
    FILE *arch = fopen("stats.dat", "ab");
    if(!arch)
    {
        printf("Error al crear el archivo\n");
        return ERR;
    }

    if (fwrite(playerReg, sizeof(tPlayer), 1, arch) != 1) {
        printf("Error al escribir el registro\n");
        fclose(arch);
        return ERR;
    }

    fclose(arch);
    return OK;
}

static int betterThan(const tPlayer *a, const tPlayer *b)
{
    if (a->score != b->score) return a->score > b->score;
    return a->moves < b->moves;
}

static void top5Insert(tPlayer top[5], int *n, const tPlayer *p)
{
    if (*n < 5) {
        top[*n] = *p;
        (*n)++;
    } else {
        if (!betterThan(p, &top[4])) return; // no entra al top
        top[4] = *p;
    }

    // ordenar top[0..n-1] (insertion sort, es chico)
    for (int i = 1; i < *n; i++) {
        tPlayer key = top[i];
        int j = i - 1;
        while (j >= 0 && betterThan(&key, &top[j])) {
            top[j + 1] = top[j];
            j--;
        }
        top[j + 1] = key;
    }
}

int mostrarTop(const char *ruta, tPlayer top[5])
{
    FILE *arch = fopen(ruta, "rb");
    if (!arch) {
        printf("Error al leer el archivo\n");
        return ERR;
    }

    int n = 0;

    tPlayer p;
    while (fread(&p, sizeof(tPlayer), 1, arch) == 1) {
        top5Insert(top, &n, &p);
    }

    fclose(arch);

    printf("===== TOP %d =====\n", n);
    for (int i = 0; i < n; i++) {
        printf("#%d %s | Score: %d | Moves: %d | Pares: %d | Racha: %d\n",
               i + 1,
               top[i].namePlayer,
               top[i].score,
               top[i].moves,
               top[i].pairsFound,
               top[i].streak);
    }

    return n;
}