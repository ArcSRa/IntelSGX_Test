//SEULEMENT POUR TEST ET DEBUG POUR LIRE ET ECRIRE DANS ENCLAVE -> FICHIER SCELLE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SEALED_PATH "data/encrypted/counter.bin"

//ficheir  scelle
int load_counter() {
    int fd = open(SEALED_PATH, O_RDONLY);
    if (fd < 0)
        return 0;
    int count = 0;
    read(fd, &count, sizeof(count));
    close(fd);
    return count;
}

// Sauvegarde le compteur dans fichier (MRENCLAVE ->  manifest)
void save_counter(int count) {
    int fd = open(SEALED_PATH, O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if (fd < 0) {
        perror("Erreur sealing");
        return;
    }
    write(fd, &count, sizeof(count));
    close(fd);
}

int main(int argc, char *argv[]) {
setvbuf(stdout, NULL, _IONBF, 0); 
setvbuf(stderr, NULL, _IONBF, 0); 

    int counter = load_counter();
    printf("Compteur initial (reprise) : %d\n", counter);
  
    while (1) {
        counter++;
        printf("Compteur: %d\n", counter);
        save_counter(counter);
        sleep(1);


        if (access("/stop", F_OK) == 0) {
            printf("Fichier /stop detecete , arret...\n");

            break;
        }
    }
    
    save_counter(counter);
    printf("Fin, compteur sauvegarde : %d\n", counter);
    return 0;
}
