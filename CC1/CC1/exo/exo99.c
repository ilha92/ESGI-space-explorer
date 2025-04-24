#include <stdio.h>
#include <stdlib.h>

int main() {
    const char * path = "exemple.txt";
    FILE *fichier = fopen(path, "r+");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char caractere;
    long position;

    while ((caractere = fgetc(fichier)) != EOF) {
        if (caractere >= 'a' && caractere <= 'z') {
            // je converti en majuscule via l'ascii
            caractere = caractere - 32;  
            position = ftell(fichier);
            fseek(fichier, position - 1, SEEK_SET);
            fputc(caractere, fichier);
        }
    }

    fclose(fichier);
    return 0;
}