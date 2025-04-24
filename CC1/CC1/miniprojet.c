/* Compilation : gcc miniprojet.c -lmingw32 -lSDLmain -lSDL -lSDL_gfx */
/*executer le jeu commande c'est ./a.exe*/
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

// Configuration
typedef struct {
    int largeur;
    int hauteur;
    int nb_asteroides;
    int nb_ennemis;
    double vie_vaisseau;
    double vie_adversaire;
    double vitesse;
} Config;

Config config = {800, 600, 5, 10, 100, 10, 5.0};

SDL_Surface *ecran = NULL;

// Structures
typedef struct {
    double x, y;
} Vec2;

typedef struct {
    Vec2 position;
    double vie;
    int actif;
} Asteroide;

typedef struct {
    Vec2 position;
    Vec2 vitesse;
    double direction;
    double vie;
    int compteur_rafale;
} Joueur;

typedef struct {
    Vec2 position;
    Vec2 direction;
    int actif;
} Tir;

Joueur joueur;
Asteroide *asteroides;
#define MAX_TIRS 1000
Tir tirs[MAX_TIRS];

void charger_config(const char *fichier) {
    FILE *f = fopen(fichier, "r");
    if (!f) return;
    char ligne[100];
    while (fgets(ligne, 100, f)) {
        char cle[50];
        int valeur;
        if (sscanf(ligne, "%[^=]=%d", cle, &valeur) == 2) {
            if (strcmp(cle, "largeur") == 0) config.largeur = valeur;
            else if (strcmp(cle, "hauteur") == 0) config.hauteur = valeur;
            else if (strcmp(cle, "nombre_asteroides") == 0) config.nb_asteroides = valeur;
            else if (strcmp(cle, "nombre_ennemis") == 0) config.nb_ennemis = valeur;
            else if (strcmp(cle, "vie_vaisseau") == 0) config.vie_vaisseau = valeur;
            else if (strcmp(cle, "vie_adversaire") == 0) config.vie_adversaire = valeur;
            else if (strcmp(cle, "vitesse") == 0) config.vitesse = valeur;
        }
    }
    fclose(f);
}

void init() {
    joueur.position = (Vec2){config.largeur/2, config.hauteur/2};
    joueur.vitesse = (Vec2){0, 0};
    joueur.direction = 0;
    joueur.vie = config.vie_vaisseau;

    asteroides = malloc(sizeof(Asteroide) * config.nb_asteroides);
    for (int i = 0; i < config.nb_asteroides; i++) {
        asteroides[i].position.x = rand() % config.largeur;
        asteroides[i].position.y = rand() % config.hauteur;
        asteroides[i].vie = 100;
        asteroides[i].actif = 1;
    }

    for (int i = 0; i < MAX_TIRS; i++) {
        tirs[i].actif = 0;
    }
}

void tirer() {
    static Uint32 dernier_tir = 0;
    Uint32 maintenant = SDL_GetTicks();
    if (maintenant - dernier_tir < 200) return;
    dernier_tir = maintenant;

    for (int i = 0; i < MAX_TIRS; i++) {
        if (!tirs[i].actif) {
            tirs[i].position = joueur.position;
            tirs[i].direction.x = cos(joueur.direction) * 10;
            tirs[i].direction.y = sin(joueur.direction) * 10;
            tirs[i].actif = 1;
            break;
        }
    }
}

void mise_a_jour_tirs() {
    for (int i = 0; i < MAX_TIRS; i++) {
        if (tirs[i].actif) {
            tirs[i].position.x += tirs[i].direction.x;
            tirs[i].position.y += tirs[i].direction.y;
            if (tirs[i].position.x < 0 || tirs[i].position.x > config.largeur ||
                tirs[i].position.y < 0 || tirs[i].position.y > config.hauteur) {
                tirs[i].actif = 0;
            }
        }
    }
}

void verifier_collisions() {
    for (int i = 0; i < config.nb_asteroides; i++) {
        if (!asteroides[i].actif) continue;

        double dx = joueur.position.x - asteroides[i].position.x;
        double dy = joueur.position.y - asteroides[i].position.y;
        double distance = sqrt(dx*dx + dy*dy);
        if (distance < 25) joueur.vie -= 1;

        for (int j = 0; j < MAX_TIRS; j++) {
            if (tirs[j].actif) {
                double tx = tirs[j].position.x - asteroides[i].position.x;
                double ty = tirs[j].position.y - asteroides[i].position.y;
                double dist_tir = sqrt(tx*tx + ty*ty);
                if (dist_tir < 15) {
                    asteroides[i].vie -= 50;
                    tirs[j].actif = 0;
                    if (asteroides[i].vie <= 0) asteroides[i].actif = 0;
                }
            }
        }

        for (int j = i + 1; j < config.nb_asteroides; j++) {
            if (!asteroides[j].actif) continue;

            double dx2 = asteroides[i].position.x - asteroides[j].position.x;
            double dy2 = asteroides[i].position.y - asteroides[j].position.y;
            double distance2 = sqrt(dx2*dx2 + dy2*dy2);
            if (distance2 < 30) {
                double midX = (asteroides[i].position.x + asteroides[j].position.x) / 2;
                double midY = (asteroides[i].position.y + asteroides[j].position.y) / 2;
                asteroides[i].position.x = midX + rand()%20 - 10;
                asteroides[i].position.y = midY + rand()%20 - 10;
                asteroides[j].position.x = midX + rand()%20 - 10;
                asteroides[j].position.y = midY + rand()%20 - 10;
            }
        }
    }
}

void dessiner() {
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 25, 25, 53));
    boxRGBA(ecran, 10, 10, 10 + joueur.vie * 2, 25, 255, 0, 0, 255);
    // Faucon Millenium 
    int cx = joueur.position.x;
    int cy = joueur.position.y;
    filledCircleRGBA(ecran, cx, cy, 12, 180, 180, 180, 255);
    int offsetX = cos(joueur.direction) * 15;
    int offsetY = sin(joueur.direction) * 15;
    boxRGBA(ecran, cx + offsetX - 4, cy + offsetY - 4, cx + offsetX + 4, cy + offsetY + 4, 100, 100, 255, 255); 
    filledCircleRGBA(ecran, cx + cos(joueur.direction + 0.5) * 10, cy + sin(joueur.direction + 0.5) * 10, 4, 160, 160, 160, 255); 
    filledCircleRGBA(ecran, cx + cos(joueur.direction - 0.5) * 10, cy + sin(joueur.direction - 0.5) * 10, 4, 160, 160, 160, 255); 
    for (int i = 0; i < config.nb_asteroides; i++) {
        if (asteroides[i].actif) {
            filledCircleRGBA(ecran, asteroides[i].position.x, asteroides[i].position.y, 15, 100, 100, 100, 255);
        }
    }
    for (int i = 0; i < MAX_TIRS; i++) {
        if (tirs[i].actif) {
            filledCircleRGBA(ecran, tirs[i].position.x, tirs[i].position.y, 3, 255, 0, 0, 255);
        }
    }
}

void afficher_texte(SDL_Surface *ecran, const char *texte, int x, int y) {
    stringRGBA(ecran, x, y, texte, 255, 255, 255, 255);
}

void afficher_menu() {
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    afficher_texte(ecran, "ESPACE pour commencer", config.largeur/2 - 80, config.hauteur/2 - 10);
    SDL_Flip(ecran);
}

void afficher_pause() {
    afficher_texte(ecran, "PAUSE - Appuyez sur P pour reprendre", config.largeur/2 - 100, config.hauteur/2);
}

void afficher_gameover(int victoire) {
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    if (victoire)
        afficher_texte(ecran, "GG Mr Trancho ;) !", config.largeur/2 - 40, config.hauteur/2);
    else
        afficher_texte(ecran, "GAME OVER", config.largeur/2 - 40, config.hauteur/2);
    SDL_Flip(ecran);
    SDL_Delay(3000);
}

void mise_a_jour(const Uint8 *touches) {
    if (touches[SDLK_LEFT]) joueur.direction -= 0.1;
    if (touches[SDLK_RIGHT]) joueur.direction += 0.1;

    if (touches[SDLK_UP]) {
        joueur.position.x += cos(joueur.direction) * config.vitesse;
        joueur.position.y += sin(joueur.direction) * config.vitesse;
    }
    if (touches[SDLK_DOWN]) {
        joueur.position.x -= cos(joueur.direction) * config.vitesse;
        joueur.position.y -= sin(joueur.direction) * config.vitesse;
    }

    if (touches[SDLK_SPACE]) tirer();

    if (joueur.position.x < 10) joueur.position.x = 10;
    if (joueur.position.x > config.largeur - 10) joueur.position.x = config.largeur - 10;
    if (joueur.position.y < 10) joueur.position.y = 10;
    if (joueur.position.y > config.hauteur - 10) joueur.position.y = config.hauteur - 10;

    mise_a_jour_tirs();
    verifier_collisions();
}

int main(int argc, char *argv[]) {
    if (argc > 1) charger_config(argv[1]);

    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(config.largeur, config.hauteur, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("miniprojet", NULL);

    int actif = 1, en_pause = 0, jeu_commence = 0;

    afficher_menu();

    while (!jeu_commence) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE)
                jeu_commence = 1;
        }
    }

    init();

    SDL_Event event;
    const Uint8 *touches = SDL_GetKeyState(NULL);

    while (actif && joueur.vie > 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) actif = 0;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) en_pause = !en_pause;
        }

        touches = SDL_GetKeyState(NULL);

        if (!en_pause) {
            mise_a_jour(touches);
            dessiner();
        } else {
            afficher_pause();
        }

        int tous_morts = 1;
        for (int i = 0; i < config.nb_asteroides; i++) {
            if (asteroides[i].actif) {
                tous_morts = 0;
                break;
            }
        }
        if (tous_morts) {
            afficher_gameover(1);
            break;
        }

        if (joueur.vie <= 0) {
            afficher_gameover(0);
            break;
        }

        SDL_Flip(ecran);
        SDL_Delay(1000 / 60);
    }

    free(asteroides);
    SDL_Quit();
    return 0;
}
