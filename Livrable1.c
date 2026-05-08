#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * LIVRABLE 1 : Code Source Industriel
 * Projet : Système de Collision pour Essaim Autonome (UAV)
 * Contrainte : Arithmétique des pointeurs pure
 * (Interdiction des crochets [])
 */

/* Structure représentant un drone */
struct Drone {
    int id;
    float x;
    float y;
    float z;
};

/* Fonction de comparaison pour le tri sur l’axe X */
int comparerX(const void* a, const void* b) {

    const struct Drone* d1 = (const struct Drone*)a;
    const struct Drone* d2 = (const struct Drone*)b;

    if (d1->x < d2->x)
        return -1;

    if (d1->x > d2->x)
        return 1;

    return 0;
}

/* Calcul de la distance euclidienne 3D */
float calculerDistance(struct Drone* d1, struct Drone* d2) {

    float dx = d1->x - d2->x;
    float dy = d1->y - d2->y;
    float dz = d1->z - d2->z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

int main() {

    int N = 10000;

    srand(time(NULL));

    /* Allocation dynamique de l’essaim */
    struct Drone* essaim =
        (struct Drone*)malloc(N * sizeof(struct Drone));

    if (essaim == NULL) {

        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return 1;
    }

    /* Initialisation des drones */
    for (int i = 0; i < N; i++) {

        struct Drone* actuel = (essaim + i);

        actuel->id = i + 1;

        actuel->x =
            ((float)rand() / RAND_MAX) * 1000.0f;

        actuel->y =
            ((float)rand() / RAND_MAX) * 1000.0f;

        actuel->z =
            ((float)rand() / RAND_MAX) * 1000.0f;
    }

    /* Tri selon l’axe X */
    qsort(essaim,
          N,
          sizeof(struct Drone),
          comparerX);

    float distanceMinimale = 1e9;

    struct Drone* droneA = NULL;
    struct Drone* droneB = NULL;

    /* Recherche des deux drones les plus proches */
    for (int i = 0; i < N; i++) {

        for (int j = i + 1; j < N; j++) {

            struct Drone* p1 = (essaim + i);
            struct Drone* p2 = (essaim + j);

            /* Optimisation spatiale */
            if ((p2->x - p1->x) >= distanceMinimale) {
                break;
            }

            float d = calculerDistance(p1, p2);

            if (d < distanceMinimale) {

                distanceMinimale = d;

                droneA = p1;
                droneB = p2;
            }
        }
    }

    /* Affichage des résultats */
    if (droneA != NULL && droneB != NULL) {

        printf("=== ALERTE COLLISION IDENTIFIEE ===\n");

        printf(
            "Drone 1 ID : %d "
            "(Coords : %.2f, %.2f, %.2f)\n",
            droneA->id,
            droneA->x,
            droneA->y,
            droneA->z
        );

        printf(
            "Drone 2 ID : %d "
            "(Coords : %.2f, %.2f, %.2f)\n",
            droneB->id,
            droneB->x,
            droneB->y,
            droneB->z
        );

        printf(
            "Distance minimale : %.4f unités\n",
            distanceMinimale
        );

        printf(
            "Action : Déclenchement "
            "de la manoeuvre d'évitement.\n"
        );
    }

    /* Libération de la mémoire */
    free(essaim);

    return 0;
}