#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_character(const char *filename, const char *name, int vie, int atk, int def, int vit) {
    FILE *fichier = fopen(filename, "wb");
    if (fichier == NULL) {
        perror("Erreur ouverture fichier pour écriture");
        exit(1);
    }

    int len = strlen(name);
    fwrite(&len, sizeof(int), 1, fichier);      // Écrit la longueur du nom
    fwrite(name, sizeof(char), len, fichier);   // Écrit les caractères du nom
    fwrite(&vie, sizeof(int), 1, fichier);
    fwrite(&atk, sizeof(int), 1, fichier);
    fwrite(&def, sizeof(int), 1, fichier);
    fwrite(&vit, sizeof(int), 1, fichier);

    fclose(fichier);
    printf("Personnage créé et sauvegardé dans '%s'.\n", filename);
}

void read_character(const char *filename) {
    FILE *fichier = fopen(filename, "rb");
    if (fichier == NULL) {
        perror("Erreur ouverture fichier pour lecture");
        exit(1);
    }

    int len;
    fread(&len, sizeof(int), 1, fichier); // Lit la longueur du nom

    char *name = malloc(len + 1); // +1 pour le '\0' en gros
    fread(name, sizeof(char), len, fichier);
    name[len] = '\0'; // ajoute le caractere a la fin de chaîne

    int vie, atk, def, vit;
    fread(&vie, sizeof(int), 1, fichier);
    fread(&atk, sizeof(int), 1, fichier);
    fread(&def, sizeof(int), 1, fichier);
    fread(&vit, sizeof(int), 1, fichier);

    fclose(fichier);

    printf("Personnage : {\n");
    printf("Nom du guerrier de Oblivion     : %s\n", name);
    printf("Vie     : %d\n", vie);
    printf("Attaque : %d\n", atk);
    printf("Défense : %d\n", def);
    printf("Vitesse : %d\n", vit);
    printf("}\n");

    free(name);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Utilisation :\n");
        printf("  %s -create [FICHIER] [NOM] [VIE] [ATK] [DEF] [VIT]\n", argv[0]);
        printf("  %s -read [FICHIER]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-create") == 0 && argc == 8) {
        const char *fichier = argv[2];
        const char *nom = argv[3];
        int vie = atoi(argv[4]);
        int atk = atoi(argv[5]);
        int def = atoi(argv[6]);
        int vit = atoi(argv[7]);
        create_character(fichier, nom, vie, atk, def, vit);
    }
    else if (strcmp(argv[1], "-read") == 0 && argc == 3) {
        const char *fichier = argv[2];
        read_character(fichier);
    }
    else {
        printf("Arguments invalides.\n");
        return 1;
    }

    return 0;
}

/* commande executer
./prog -create chouette.perso 
*/
