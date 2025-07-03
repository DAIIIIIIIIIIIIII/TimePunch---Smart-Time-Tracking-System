# Riepilogo Completo Idee per l'Applicazione di Rilevamento Presenze

  ## 1. Piattaforma e Linguaggio
  - Sviluppo iniziale: C su Windows
  - Obiettivo finale: Eseguibile per Raspberry Pi 4 (Linux ARM)

  ## 2. Interfaccia Utente
  - Applicazione sempre attiva in primo piano
✓  - Messaggio principale: "Inserisci il tuo codice utente"      
  - Utilizzo di un tastierino numerico per l'inserimento del codice
✓  - Messaggi di benvenuto/arrivederci dopo l'inserimento del codice 
  - Futura implementazione: Aggiunta di uno schermo per confermare l'inserimento del codice e fornire messaggi di stato

  ## 3. Gestione Utenti
✓  - Codici utente generati automaticamente (001, 002, 003, ...)
✓  - File per salvare informazioni utenti      

  ### Funzioni Amministrative (Codice: 999)
✓  1. Crea nuovo utente
✓  2. Elimina utente
✓  3. Visualizza lista utenti
✓  5. Esci dal menu amministratore

  ## 4. Registrazione e Gestione Orari
✓  - Una cartella per ogni utente
✓  - Una sottocartella per ogni mese
✓  - Un file CSV per ogni giorno del mese
✓  - Formato base: YYYY-MM-DD,HH:MM:SS
✓  - Formato dettagliato CSV:
✓    - Colonne: ora di entrata, ora di uscita, calcolo ore primo turno, calcolo ore secondo turno, ore totali del giorno
✓    - Spazi tra i giorni per migliorare la leggibilità
    - Eventuale uso del colore per evidenziare informazioni importanti (considerato per futuro)

  ## 5. Calcolo Ore Lavorate
✓  - Due turni per giornata (4 orari: 2 entrate, 2 uscite)
  - Calcolo ore per turno e totale giornaliero

  ## 6. Gestione Registrazioni Mancanti e Validazione Dati
  - Segnalazione automatica di registrazioni incomplete
  - Interfaccia di correzione per amministratore
  - Possibilità di registrazione retroattiva per l'utente
  - Logica di inferenza per stimare orari mancanti
  - Flag per registrazioni incomplete
  - Logging delle modifiche manuali
✓  - Verifica che l'orario di uscita sia dopo l'orario di entrata
✓  - Controllo degli intervalli di tempo accettabili per prevenire errori evidenti
  - Controllo degli orari per identificare eventuali anomalie

✓  ## 7. Struttura File
  ```
  users/
    001/
      2024-09/
        2024-09-01.csv
        2024-09-02.csv
        ...
      2024-10/
        ...
    002/
      ...
  users.txt (elenco utenti e codici)
  error_log.txt
  ```

  ## 8. Funzionalità Aggiuntive e Futuri Miglioramenti
✓  - Generazione report (giornalieri, settimanali, mensili)
  - Sistema di notifiche per registrazioni mancanti
  - Politica aziendale per gestione eccezioni
✓  - Integrazione con API: Collegamento dei dati a Excel, Google Sheets o LibreOffice Calc per visualizzazione e analisi avanzata
  - Funzione di correzione automatica con richiesta di conferma in caso di errori
  - Gestione automatica delle anomalie comuni

  ## 9. Sicurezza e Integrità Dati
  - Protezione accesso funzioni amministrative
  - Backup regolare dei dati
✓  - Validazione input utente

  ## 10. Strumenti per la Correzione e Analisi
  - Editor di CSV integrato o modulo di correzione per gestire errori manualmente
  - Log degli errori: Registrazione delle anomalie e degli errori riscontrati
