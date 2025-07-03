#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

// Funzione per ottenere l'orario e la data attuali
void ora_corrente(char* buffer, size_t buffer_size) {

    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&now);

    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Funzione per determinare se oggi è il penultimo giorno del mese
int penultimo_giorno() {
    time_t now;
    struct tm tm_info;
    time_t end_of_month;
    struct tm *end_info;
    int ultimo_giorno, penultimo_giorno;

    time(&now);
    tm_info = *localtime(&now);

    // Trova l'ultimo giorno del mese
    tm_info.tm_mday = 1;      // Imposta il giorno al primo del mese
    tm_info.tm_mon++;        // Passa al mese successivo
    tm_info.tm_year += tm_info.tm_mon / 12; // Gestisce l'anno
    tm_info.tm_mon %= 12;    // Normalizza il mese
    tm_info.tm_mday = 0;     // Imposta il giorno al 0 del mese successivo (ultimo giorno del mese precedente)
    mktime(&tm_info);        // Normalizza la struttura tm

    ultimo_giorno = tm_info.tm_mday; // L'ultimo giorno del mese
    penultimo_giorno = ultimo_giorno - 1; // Il penultimo giorno

    // Verifica se oggi è il penultimo giorno del mese
    return (localtime(&now)->tm_mday == penultimo_giorno);
}


// Funzione per eseguire le azioni desiderate
void ogni_mm() {
    
    if (penultimo_giorno()) {
        char current_time[20];
        char path[1024] = "orari/"; // Directory principale
        char file_name[100];
        char full_path[1024];

        struct dirent *entry;
        DIR *dp;
        struct stat statbuf;
        time_t now;
        struct tm *tm_info;

        ora_corrente(current_time, sizeof(current_time));
        printf("Oggi è il penultimo giorno del mese! Esegui azioni alle: %s\n", current_time);

        // Calcola il mese e l'anno successivi
        time(&now);
        tm_info = localtime(&now);
        tm_info->tm_mon += 1; // Passa al mese successivo
        if (tm_info->tm_mon > 11) { // Se dicembre, passare all'anno successivo
            tm_info->tm_mon = 0;
            tm_info->tm_year += 1;
        }
        strftime(file_name, sizeof(file_name), "%m-%y.csv", tm_info);

        // Naviga nella directory 'orari'
        dp = opendir(path);

        // Itera su tutte le sottocartelle
        while ((entry = readdir(dp))) {


            // Ignora '.' e '..'
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {

                continue;
            }
            
            // Costruisci il percorso completo per la sottocartella
            snprintf(full_path, sizeof(full_path), "%s%s", path, entry->d_name);
            

            // Verifica se è una directory
            if (stat(full_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
                // Costruisci il percorso completo per il file
                snprintf(full_path, sizeof(full_path), "%s/%s/%s", path, entry->d_name, file_name);
                
                // Crea il file nella sottocartella
                FILE *file = fopen(full_path, "w");
            }
            
        }

        closedir(dp);
    }
}


int main() {
    // Variabili per intervalli di tempo
    int interval_giorno = 24 * 60 * 60; // 24 ore in secondi
    int interval_ora = 60 * 60; // 1 ora in secondi

    // Variabili per tenere traccia dell'ultimo tempo di esecuzione
    time_t last_run_giornaliera = 0;
    time_t last_run_ore = 0;
    time_t now;

    while (1) {
        time(&now);

        // Esegui funzione giornaliera ogni giorno
        if (difftime(now, last_run_giornaliera) >= interval_giorno) {
            ogni_mm();
            last_run_giornaliera = now;
        }

        // Esegui funzioni ogni ora
        if (difftime(now, last_run_ore) >= interval_ora) {
            /*
            funzione_ore1();
            funzione_ore2();
            funzione_ore3();
            funzione_ore4();
            */
            last_run_ore = now;
        }

        // Pausa per un intervallo breve per evitare l'uso eccessivo della CPU
        sleep(60); // Pausa di 60 secondi
    }

    return 0;
}