#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fin = NULL;
    FILE *fout = NULL;

    char noms[100][100];   // tableau de 100 noms
    int numeros[100];      // tableau de 100 numéros
    int total = 0;

    // Lire les fichiers d'entrée et sortie
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            fin = fopen(argv[i+1], "r");
            i++;
        } else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            fout = fopen(argv[i+1], "w");
            i++;
        }
    }

    // Charger depuis un fichier s'il existe
    if (fin != NULL) {
        while (fscanf(fin, "%s %d", noms[total], &numeros[total]) == 2) {
            total++;
        }
        fclose(fin);
    }

    // Ajout des nouveaux entrées
    while (1) {
        char nom[100];
        int numero;

        printf("Nom (None pour arrêter) : ");
        scanf("%s", nom);
        if (strcmp(nom, "None") == 0) {
            break;
        }

        printf("Numéro : ");
        scanf("%d", &numero);

        strcpy(noms[total], nom);
        numeros[total] = numero;
        total++;
    }

    // Sauvegarder dans le fichier
    if (fout != NULL) {
        for (int i = 0; i < total; i++) {
            fprintf(fout, "%s %d\n", noms[i], numeros[i]);
        }
        fclose(fout);
    }

    // Rechercher par nom
    while (1) {
        char recherche[100];
        printf("Nom à rechercher (None pour arrêter) :\n>>> ");
        scanf("%s", recherche);
        if (strcmp(recherche, "None") == 0) {
            break;
        }

        int trouvé = 0;
        for (int i = 0; i < total; i++) {
            if (strcmp(noms[i], recherche) == 0) {
                printf("Le numéro de \"%s\" est %d\n", noms[i], numeros[i]);
                trouvé = 1;
            }
        }

        if (!trouvé) {
            printf("Nom introuvable.\n");
        }
    }

    return 0;
}
