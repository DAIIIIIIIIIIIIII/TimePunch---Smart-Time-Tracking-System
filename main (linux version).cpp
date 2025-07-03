#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <iomanip>
#include <time.h>
#include <ctype.h>
//#include <direct.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//Dichiarazioni delle funzioni
void crea_nuovo_utente();
void elimina_utente();
void visualizza_lista_utenti();
void esci_dalle_impostazioni();
void settings();
void invia_messaggio(char *nome);
void checkppl(char codice[], bool *nice, char *nome);
void validification(char codice[], bool *nice, char *nome);
void crea_nuovo_utente();
void formatta_nome(char *nome);
void inserimento_orari (char *nome);


void scrivi_log(const char *formato, ...) {

    FILE *log_file = fopen("programma.log", "a");       //Apri in modalità append

    if (log_file == NULL) {

        printf("Errore apertura file di log");
        return;
    }

    // Ottieni l'orario attuale
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    // Scrivi il timestamp nel log
    fprintf(log_file, "[%s] ", buffer);

    //Scrivi il messaggio di log
    va_list args;
    va_start(args, formato);
    vfprintf(log_file, formato, args);
    va_end(args);

    fprintf(log_file, "\n");                            //Aggiungi una nuova riga
    fclose(log_file);                                   //Chiudi il file
}

void inserimento_orari (char *nome) {

    char percorso[256];
    char nome_file[10];
    FILE *file;
    char buffer[512] = {0};
    int data_trovata = 0;
    char riga_array[50] = {0};
    char riga_temp[50] = {0};

    //Data attuale
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char data_odierna[25];
    snprintf(data_odierna, sizeof(data_odierna), "%02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year % 100);

    //Crea il nome del file nel formato MM-YY.csv
    snprintf(nome_file, sizeof(nome_file), "%02d-%02d.csv", tm.tm_mon + 1, tm.tm_year % 100);

    //Percorso completo della cartella "orari/nome/MM-YY.csv"
    snprintf(percorso, sizeof(percorso), "orari/%s/%s", nome, nome_file);

    if (access(percorso, F_OK) == -1) {
        
        //Crea il file in scrittura
        file = fopen(percorso, "a");

        if (file == NULL) {

            scrivi_log("ERRORE NELLA CREAZIONE DEL FILE: %s", percorso);
            printf("Errore nella creazione del file: %s\n", percorso);
            return;

        } else {

            fprintf(file,   "Data,Ora Ingresso Mattina,Ora Uscita Mattina,Ore Lavorate Mattina,"
                            "Ora Ingresso Pomeriggio,Ora Uscita Pomeriggio,Ore Lavorate Pomeriggio,Ore Lavorate Giornata,\n");

            //fprintf(file, "\n");


            fprintf(file, "%02d-%02d-%02d,,,,,,,,\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year % 100);

            fclose(file);  
        }
    }

    file = fopen(percorso, "r+");

    if (file == NULL) {

        scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", percorso);
        printf("Errore nell'apertura del file: %s\n", percorso);
        return;

    } else {

        //Variabile per memorizzare la posizione della riga trovata
        long posizione_riga = 0;

        //Cerca la riga con la data odierna
        while (fgets(buffer, sizeof(buffer), file)) {

            if (strstr(buffer, data_odierna)) {

                data_trovata = 1;
                strcpy(riga_array, buffer);  //Copia la riga trovata nell'array
                scrivi_log("Riga trovata nell'array: %s", riga_array);
                posizione_riga = ftell(file) - strlen(buffer);  //Salva la posizione della riga
                break;
            }
        }

        if (data_trovata == 0) {

            //Vai alla fine del file
            fseek(file, 0, SEEK_END);

            //Scrivi la nuova riga con la data odierna e gli slot vuoti
            fprintf(file, "%s,,,,,,,,\n", data_odierna);
            scrivi_log("Data non presente nel file...");
            scrivi_log("Scrittura della nuova riga: %s", data_odierna);
            fclose (file);

        }

        file = fopen(percorso, "r");

        if (file == NULL) {

            scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", percorso);
            printf("Errore nell'apertura del file: %s\n", percorso);
            return;

        } else {
        
            //Cerca la riga con la data odierna

            while (fgets(buffer, sizeof(buffer), file)) {

                if (strstr(buffer, data_odierna)) {

                    strcpy(riga_array, buffer);  //Copia la riga trovata nell'array
                    scrivi_log("Riga con data odierna trovata: %s", riga_array);
                    posizione_riga = ftell(file) - strlen(buffer);  //Salva la posizione della riga
                    break;

                }
            }

            fclose (file);

            file = fopen(percorso, "r+");

            if (file == NULL) {

                scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", percorso);
                printf("Errore nell'apertura del file: %s\n", percorso);
                return;

            } else {

                char ora_attuale[6];
                snprintf(ora_attuale, sizeof(ora_attuale), "%02d.%02d", tm.tm_hour, tm.tm_min); //Formatta l'ora come "HH.MM"

                //Trova il posto corretto per inserire l'ora corrente
                if (tm.tm_hour >= 13) {

                    //Dopo le 13: cerca lo slot 5 o 6
                    char *slot = strchr(riga_array, ','); //Vai alla colonna 1

                    for (int i = 0; i < 3; i++) {

                        slot = strchr(slot + 1, ',');  //Vai avanti fino a colonna 5
                    }

                    if (*(slot + 1) == ',') {

                        //Slot 5 vuoto, inserisci l'ora corrente
                        snprintf(riga_temp, sizeof(riga_temp), "%.*s%s%s", (int)(slot - riga_array + 1), riga_array, ora_attuale, slot + 1);
                        scrivi_log("Prima colonna libera, inserimento orario: %s", ora_attuale);

                    } else {

                        //Slot 5 pieno, inserisci l'ora in slot 6
                        slot = strchr(slot + 1, ',');  //Vai alla colonna 6

                        // Trova la successiva virgola per delimitare il contenuto da sovrascrivere
                        char *fine_slot = strchr(slot + 1, ',');
                        snprintf(riga_temp, sizeof(riga_temp), "%.*s%s%s", (int)(slot - riga_array + 1), riga_array, ora_attuale, fine_slot);
                        scrivi_log("Prima colonna occupata, inserimento orario nella seconda colonna: %s", ora_attuale);
                    }

                } else {

                    //Prima delle 13: cerca lo slot 2 o 3
                    char *slot = strchr(riga_array, ','); //Vai alla colonna 1

                    if (*(slot + 1) == ',') {

                        //Slot 2 vuoto, inserisci l'ora corrente
                        snprintf(riga_temp, sizeof(riga_temp), "%.*s%s%s", (int)(slot - riga_array + 1), riga_array, ora_attuale, slot + 1);
                        scrivi_log("Primo colonna libera, inserimento orario: %s", ora_attuale);

                    } else {

                        // Slot 2 pieno, sostituisci il contenuto e riscrivi l'ora
                        slot = strchr(slot + 1, ',');  //Vai alla colonna 3
                
                        //Trova la successiva virgola per delimitare il contenuto da sovrascrivere
                        char *fine_slot = strchr(slot + 1, ',');

                        //Sovrascrivi il contenuto del secondo slot con l'ora corrente, eliminando il precedente
                        snprintf(riga_temp, sizeof(riga_temp), "%.*s%s%s", (int)(slot - riga_array + 1), riga_array, ora_attuale, fine_slot);
                        scrivi_log("Prima colonna occupata, inserimento orario nella seconda colonna: %s", ora_attuale);
                    }
                }
            }            
            //Vai alla posizione della riga trovata e sovrascrivi la riga
            fseek(file, posizione_riga, SEEK_SET);
            fprintf(file, "%s", riga_temp);
            scrivi_log("Riga del file riscritta: %s", riga_temp);

            fclose(file);
        }
    }

}

void formatta_nome(char *nome) {

    // Prima lettera maiuscola, il resto minuscolo
    nome[0] = toupper(nome[0]);

    for (int i = 1; nome[i] != '\0'; i++) {

        nome[i] = tolower(nome[i]);
    }
}

void crea_nuovo_utente() {

    char nome[100];
    char ultima_linea[100];
    int ultimo_codice = 0;
    FILE *file = fopen("utenti.txt", "r");

    if (file == NULL) {

        scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", "utenti.txt");
        printf("Errore nell'apertura del file: %s\n", "utenti.txt"); // Aggiungi \n per la formattazione
        return;

    } else {

        while (fgets(ultima_linea, sizeof(ultima_linea), file)) {                       //Leggi l'ultima riga del file per ottenere l'ultimo codice

            // Estrai l'ultimo codice utente dalla linea
            int codice_temp;

            if (sscanf(ultima_linea, "%*s %d,", &codice_temp) == 1) {

                ultimo_codice = codice_temp;
            }
        }
    }

    fclose(file);

    ultimo_codice++;

    printf("\nInserisci il nome del nuovo utente: ");
    scanf("%s", nome);

    formatta_nome(nome);

    // Riapri il file in modalità append per aggiungere il nuovo utente
    file = fopen("utenti.txt", "a");

    if (file == NULL) {

        scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", "utenti.txt");
        printf("Errore nell'apertura del file: %s\n", "utenti.txt"); // Aggiungi \n per la formattazione

    } else {

        fprintf(file, "%s %03d,\n", nome, ultimo_codice);                               //Scrivi il nuovo utente nel file con il codice incrementato
        
        fclose(file);

        //mkdir(strcat(strcpy(ultima_linea, "orari/"), nome));                            //Creazione della cartella con nome utente        //Windows version
        mkdir(strcat(strcpy(ultima_linea, "orari/"), nome), 0755);                                                                          //Linux version
        
        printf("\nNuovo utente %s aggiunto con codice %03d.\n\n", nome, ultimo_codice);
        scrivi_log("Nuovo utente registrato correttamente");
    }
}

void elimina_utente() {
    
    FILE *file = fopen("utenti.txt", "r");
    char linea[100] = {0};                                                          //Buffer per contenere le righe del file
    char utenti[100][100] = {0};
    int scelta = 0;
    int count = 0;
    char conferma;

    if (file == NULL) {
        
        scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", "utenti.txt");
        printf("Errore nell'apertura del file: %s", "utenti.txt");
        fclose (file);
        return;

    }

    printf("\nLista utenti:\n\n");

    // Leggi ogni riga del file e salva solo il nome nell'array `utenti`
    while (fgets(linea, sizeof(linea), file)) {

        char nome[100];

        // Estrae solo il nome dalla riga
        if (sscanf(linea, "%s", nome) == 1) {

            strcpy(utenti[count], nome);
            printf("%d. %s\n", count + 1, nome);                                    //Stampa solo il nome
            count++;
        }
    }

    fclose(file);

    printf("\n\nQuale utente vuoi eliminare (Inserisci il numero): ");
    scanf("%d", &scelta);
    printf("\n");
        
    scrivi_log("Valore inserito dall'utente: %d", scelta);

    // Verifica che la scelta sia valida
    if (scelta < 1 || scelta > count) {

        printf("Utente non presente nel file\n");
        scrivi_log("Valore inserito dall'utente inesistente nel file");
        settings ();
        return;
    }


    printf("Sei sicuro di voler eliminare l'utente \"%s\"? (Y/N): ", utenti[scelta - 1]);   //conferma prima di eliminare
    scanf(" %c", &conferma, 1);
    printf ("\n");

    if (conferma == 'Y' || conferma == 'y') {

        file = fopen("utenti.txt", "r");
        FILE *tempFile = fopen("temp.txt", "w");

        if (file == NULL && tempFile == NULL) {

            scrivi_log("ERRORE NELL'APERTURA DI UNO DEI PRESENTI FILE: %s, %s", "utenti.txt", "temp.txt");
            printf("Errore nell'apertura di uno dei presenti file: %s, %s\n", "utenti.txt", "temp.txt");

        }

        int lineCount = 0;

        // Leggi ogni riga e scrivi nel file temporaneo se non è la riga da eliminare
        while (fgets(linea, sizeof(linea), file)) {

            lineCount++;

            if (lineCount != scelta) {

                fputs(linea, tempFile);
            }
        }

        fclose(file);
        fclose(tempFile);

        remove("utenti.txt");
        rename("temp.txt", "utenti.txt");

        scrivi_log("Utente \"%s\" eliminato con successo.", utenti[scelta - 1]);
        printf("Utente \"%s\" eliminato con successo.\n", utenti[scelta - 1]);


    } else if (conferma == 'N' || conferma == 'n') {

        printf("Eliminazione annullata. Torno alle impostazioni.\n");
        scrivi_log("Eliminazione annullata");
        settings();

    } else {

        printf("Scelta non valida. Eliminazione annullata.\n");
        scrivi_log("Scelta errata, eliminazione annullata");
        settings();
    }
        
}

void visualizza_lista_utenti() {
    
    FILE *file = fopen("utenti.txt", "r");                                  //Apri il file in modalità lettura
    scrivi_log("Apertura file %s", "utenti.txt");


    char linea[100];                                                        //Buffer per contenere le righe del file
    printf("\nLista utenti:\n");
    printf("\n");

    if (file == NULL) {
        
        scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", "utenti.txt");
        printf("Errore nell'apertura del file: %s", "utenti.txt");

    } else {

        // Leggi ogni riga del file e stampala a schermo
        while (fgets(linea, sizeof(linea), file)) {

            printf("%s", linea);
        }

        printf("\n\n");
        fclose(file);
    }
}

void esci_dalle_impostazioni() {

    printf("\n");
}

void settings() {
    int scelta;

    // Stampa delle opzioni del menu
    printf("\n1. Crea nuovo utente\n");
    printf("2. Elimina utente\n");
    printf("3. Visualizza lista utenti\n");
    printf("5. Esci dalle impostazioni\n\n");

    // Acquisizione della scelta dell'utente
    printf("Inserisci la tua scelta (1-5): ");
    scanf("%d", &scelta);

    scrivi_log("Apertura impostazioni...");
    scrivi_log("Valore impostazioni immesso dall'utente: %d", scelta);



    // Controllo con switch-case in base alla scelta dell'utente
    switch (scelta) {

        case 1:
            crea_nuovo_utente();
            break;

        case 2:
            elimina_utente();
            break;

        case 3:
            visualizza_lista_utenti();
            break;

        case 5:
            esci_dalle_impostazioni();
            scrivi_log("Chiusura impostazioni...");
            break;

        default:
            printf("\nNon sai contare da 1 a 5?\n");
            break;
    }
}

void invia_messaggio(char *nome) {                                          //Messaggi e probabilità

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);                                          //stabilisce l'ora
    const char* fileMessaggi;

    // Determina il file da aprire in base all'ora del giorno
    if (tm.tm_hour >= 0 && tm.tm_hour < 7) {

        fileMessaggi = "messaggi/messaggi_mattino_presto.txt";

    } else if (tm.tm_hour >= 7 && tm.tm_hour < 10) {

        fileMessaggi = "messaggi/messaggi_mattino.txt";

    } else if (tm.tm_hour >= 10 && tm.tm_hour < 13) {

        fileMessaggi = "messaggi/messaggi_pranzo.txt";

    } else if (tm.tm_hour >= 13 && tm.tm_hour < 16) {

        fileMessaggi = "messaggi/messaggi_pomeriggio.txt";

    } else {

        fileMessaggi = "messaggi/messaggi_sera.txt";
    }

    FILE* file = fopen(fileMessaggi, "r");                                  //Apre il file dei messaggi

    if (file == NULL) {

        printf("Errore nell'apertura\n");                                   //file non esite
        printf("Database messaggi non esistente\n");
        printf("Chiama Ivan\n");

        exit(1);
    }

    
    char linea[256];                                                        //Buffer per leggere ogni riga dal file
    char messaggi[100][256];                                                //Array per memorizzare fino a 100 messaggi, ciascuno di massimo 256 caratteri
    int probabilita[100];                                                   //rray per memorizzare la probabilità associata a ciascun messaggio
    int count = 0;                                                          //Contatore per il numero di messaggi letti dal file
    int totaleProbabilita = 0;

    // Legge i messaggi e le probabilità dal file
    while (fgets(linea, sizeof(linea), file)) {
        
        char messaggio[256];                                                //Buffer temporaneo per estrarre il messaggio dalla linea
        int prob;                                                           //Variabile per memorizzare la probabilità estratta dalla linea
        
        // Estrae il messaggio e la probabilità, separati da una virgola
        if (sscanf(linea, "%[^;]; %d", messaggio, &prob) == 2) {

            strcpy(messaggi[count], messaggio);
            probabilita[count] = prob;
            totaleProbabilita += prob;
            count++;

        }
    }
    fclose(file);


    // Genera un numero casuale tra 1 e la somma totale delle probabilità
    int scelta = rand() % totaleProbabilita + 1;
    int cumulativa = 0;

    // Seleziona un messaggio basato sulla probabilità
    for (int i = 0; i < count; i++) {

        cumulativa += probabilita[i];

        if (scelta <= cumulativa) {

            char messaggioFinale[256];
            
            snprintf(messaggioFinale, sizeof(messaggioFinale), messaggi[i], nome);
            printf("\n%s\n\n", messaggioFinale);
            return;
        }
    }

}

void checkppl (char codice [], bool *nice, char *nome) {

    FILE *utenti = fopen("utenti.txt", "r");                                //nome file database utente
    char linea [25];
    char codice_utente [4];

    if (utenti == NULL) {

        printf("Errore nell'apertura\n");                                   //file non esite
        printf("Database utenti non esistente\n");
        printf("Chiama Ivan\n");
        scrivi_log("ERRORE NELL'APERTURA DEL FILE: %s", "utenti.txt");

        exit (1);

    } else {

        while (fgets(linea, sizeof(linea), utenti)) {

            if (sscanf(linea, "%s %3s", nome, codice_utente) == 2) {        //Estrae il nome e il codice dalla riga e li assegna a nome e codice utente

                if (strcmp(codice, codice_utente) == 0) {                   //Confronta il codice del file con quello inserito dall'utente

                    *nice = true;

                    fclose(utenti);
                    scrivi_log("Utente presente nel database");
                    return;
                }
            }
        }
    
    }
    
    fclose(utenti); 
    *nice = false;                                                        //Chiude il file anche se il codice non viene trovato

}

void validification(char codice[], bool *nice, char *nome) {

    if ((strlen(codice) == 0) || (codice[0] == '\0')) {

        printf("Errore: codice non valido, riprova.\n");                    //cod. utente non valido se vuoto o, == 0
        scrivi_log("Codice inserito dall'utente non valido");
        *nice = false;

    } else {

        checkppl (codice, nice, nome);                                      //controlla che il cod. utente esista nel database

        if (!*nice) {

            printf ("\nUtente non trovato\n\n");
            scrivi_log("Utente non presente nel file");
        }
    }

    
}


int main() {

    system("clear");

    while (true) {

        char codice [4];                                                    //array di 3 cifre + terminatore '\0'
        bool nice = false;                                                  //validatore codice utente
        char nome [50] = {0};
        
        printf("Codice utente: ");
        scanf("%3s", codice, (unsigned)sizeof(codice));                   //evita l'overflow a massimo 3 valori
        scrivi_log("Codice utente inserito: %s", codice);


        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);                      //pulizia del buffer se il valore inserito è più grande

        if (strcmp (codice, "999") == 0) {

            settings();

        } else {

            validification (codice, &nice, nome);                           //controllo del codice utente
            
            if (nice) {

                inserimento_orari (nome);                                   //inserisce l'orario nel file apposito
            
                invia_messaggio (nome);                                     // invia un messaggio casuale dal file dei messaggi  
            }
        }

        sleep(3);
        system("clear");
    }

    return 0;
}