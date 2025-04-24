#include <stdio.h>
#include <stdlib.h>

char vigenere(char c, char k, int decode) {
    if (c < 'A' || c > 'Z') return c; // ignore ce qui est pasmajuscules

    int pc = c - 'A'; // position dans l'alphabet
    int kc = k - 'A'; // décalage depuis la clé

    if (decode) {
        return 'A' + (26 + pc - kc) % 26;
    } else {
        return 'A' + (pc + kc) % 26;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Utilisation : %s FICHIER CLE [decode]\n", argv[0]);
        return 1;
    }

    const char *fichier_path = argv[1];
    const char *cle = argv[2];
    int decode = (argc > 3 && argv[3][0] == 'd'); 

    FILE *fichier = fopen(fichier_path, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", fichier_path);
        return 1;
    }

    int i = 0; // position dans la clé
    int c;
    while ((c = fgetc(fichier)) != EOF) {
        char lettre = (char)c;

        if (lettre >= 'A' && lettre <= 'Z') {
            char k = cle[i % strlen(cle)];
            char chiffré = vigenere(lettre, k, decode);
            putchar(chiffré);
            i++;
        } else {
            putchar(lettre);
        }
    }

    fclose(fichier);
    return 0;
}
