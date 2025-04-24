#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *path = "compteur.txt";
    int compteur = 0;

    FILE *fichier = fopen(path, "r");
    if (fichier != NULL) {
        fscanf(fichier, "%d", &compteur);
        fclose(fichier);
    }

    compteur++;

    // on ouvre le fichier en ecrasant l'ancien
    fichier = fopen(path, "w");
    if (fichier == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier, "%d\n", compteur); 
    fclose(fichier);

    printf("Programme lancer %d fois\n", compteur);

    return 0;
}