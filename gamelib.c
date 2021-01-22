#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "gamelib.h"

static struct Giocatore* giocatori;
static struct Stanza* stanza_inizio;
static struct Stanza* lista_stanze;
static unsigned short quest_da_finire = 0;
static int nGiocatori = 0;
static time_t t;
static int scelta = 0;
static int imposta = 0;
static char* tipoRnd;
static char* tipoRnd1;
static char* tipo;
static int tipost = 0;
static int turno_attuale = 1;
static int turni_giocatori[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static int azione;
static int nStanze = 0;
static int uccidi = 0;
static int uccidiboolean = 0;
static int morto[9];
static char cha;
static int ok = 0;

static void stampa();
static void avanza();
static void avanti();
static void destra();
static void sinistra();
static void esegui_quest();
static void chiamata_emergenza();
static void uccidi_astronauta();
static void usa_botola();
static void sabotaggio();
static void verifica();
static void player();
static void libera();

void imposta_gioco(){
  srand((unsigned) time(&t));
  printf("\n+--------------------------------------------------+\n");
  printf("|                    IMPOSTA GIOCO                 |\n");
  printf("+--------------------------------------------------+\n");
  printf("Inserisci il numero di giocatori (da 4 a 10):\n");
  scanf("%d", &nGiocatori);
  while(nGiocatori <4 || nGiocatori>10){
    printf("Inserire un minimo di 4 giocatori a un massimo di 10!\n");
    while (getchar() != '\n');
    scanf("%d", &nGiocatori);
  }
  int c;
  printf("Inserisci il numero di quest da completare:\n");
  scanf("%hd", &quest_da_finire);
  while(quest_da_finire>30 || (c = getchar()) != '\n' && c != EOF){
    printf("Inserire un massimo di 30 quest!\n");
    while(getchar() != '\n');
    scanf("%hd", &quest_da_finire);
  }
  giocatori = malloc(sizeof(struct Giocatore) * nGiocatori);
  stanza_inizio = malloc(sizeof(struct Stanza) * 4);
  stanza_inizio->avanti = NULL;
  stanza_inizio->destra = NULL;
  stanza_inizio->sinistra = NULL;
  stanza_inizio->stanza_precedente = NULL;
  stanza_inizio->tipoStanza = 0;
  stanza_inizio->emergenza_chiamata = false;
  stanza_inizio->stanza_successiva = NULL;
  lista_stanze = stanza_inizio;
    int rnd;
    enum stato_giocatore* stato = malloc(sizeof(enum stato_giocatore) * nGiocatori);
    //enum tipo_stanza* tipo = malloc(sizeof(enum tipo_stanza));
    for (int i = 0; i < 4; i++) {
      rnd = (rand()%100+1);
      if(rnd <= 15){
        stanza_inizio[i].tipoStanza = 2;
      }
      else if(rnd <= 40 && rnd > 15){
        stanza_inizio[i].tipoStanza = 3;
      }

      else if(rnd <= 70 && rnd > 40){
        stanza_inizio[i].tipoStanza = 1;
      }
      else if(rnd <= 100 && rnd > 70){
        stanza_inizio[i].tipoStanza = 0;
      }

    switch (stanza_inizio[i].tipoStanza) {
     case 0: tipoRnd = "vuota";
      break;
     case 1:  tipoRnd = "quest semplice";
      break;
      case 2:  tipoRnd = "quest doppia";
      break;
      case 3:  tipoRnd = "botola";
      break;
    }
  }
  lista_stanze[0].tipoStanza = stanza_inizio[3].tipoStanza;
    enum nome_giocatore colori[] = {rosso, blu, verde, giallo, rosa, viola, bianco, nero, arancione, grigio};
    int n_impostori = (nGiocatori < 6) ? 1 : ((nGiocatori >= 6 && nGiocatori < 9) ? 2 : 3);
    int impostore = 0;
    for (int i = 0; i < nGiocatori; i++) {
      if(++impostore<=n_impostori){
        stato[i] = 1;
      }
      else {
        stato[i] = 0;
      }
    }
    for(int i = (nGiocatori-1); i>=0; i--){
      int rnd = rand()%(i+1);
      int imp_tmp = stato[i];
      stato[i] = stato[rnd];
      stato[rnd] = imp_tmp;
    }
    for(int i = 9; i>=0; i--){
      int rnd = rand()%(i+1);
      int col_tmp = colori[i];
      colori[i] = colori[rnd];
      colori[rnd] = col_tmp;
    }
    for (int i = 0; i < nGiocatori; i++){
      giocatori[i].nome = colori[i];
      giocatori[i].posizione = stanza_inizio;
      giocatori[i].stato = stato[i];

    }
    imposta = 1;
    printf("\n\n1) Vedi informazioni sui giocatori\n");
    printf("2) Gioca\n");
    printf("3) Esci\n");
    scanf("%d", &scelta);
    while (scelta!=1 && scelta!=2 && scelta!=3){
        printf("Scegli una delle 3 opzioni!\n");
        while (getchar() != '\n');
        scanf("%d", &scelta);
    }
    while (scelta == 2 && imposta == 0){
      printf("Impostare il gioco prima di giocare!\n");
      scanf("%d", &scelta);
    }
    switch(scelta) {
      case 1:
        stampa();
        break;
        case 2:
          gioca();
          break;
          case 3:
            termina_gioco();
            break;
    }
return;
}


void stampa() {
char* getColore;
printf("\n+----------------------------------------------------+\n");
  for (int i = 0; i < nGiocatori; i++){
    switch (giocatori[i].nome) {
        case 0: getColore = "rosso";
        break;
        case 1: getColore = "blu";
        break;
        case 2: getColore = "verde";
        break;
        case 3: getColore = "giallo";
        break;
        case 4: getColore = "rosa";
        break;
        case 5: getColore = "viola";
        break;
        case 6: getColore = "bianco";
        break;
        case 7: getColore = "nero";
        break;
        case 8: getColore = "arancione";
        break;
        case 9: getColore = "grigio";
        break;

    }
    if(giocatori[i].stato == 1){
      printf("Giocatore %d: %s %s %s\n", i+1, getColore, "- impostore", "- Stanza iniziale");
    }
    else{
      printf("Giocatore %d: %s %s %s\n", i+1, getColore, "- astronauta", "- Stanza iniziale");
    }
}
printf("+----------------------------------------------------+\n");
fflush(stdin);
printf("1) Gioca\n");
printf("2) Esci\n");
scanf("%d", &scelta);
while (getchar() != '\n');
while (scelta!=1 && scelta!=2){
    printf("Scegli una delle 2 opzioni!\n");
    while (getchar() != '\n');
    scanf("%d", &scelta);
}
switch (scelta) {
  case 1: gioca();
  break;
  case 2: termina_gioco();
  break;
}
  return;
}
void gioca() {
  printf("\n+--------------------------------------------------+\n");
  printf("|                 PARTITA INIZIATA!                |\n");
  printf("|     [vi trovate nella stanza: %p]    |\n", giocatori->posizione);
  printf("+--------------------------------------------------+\n");
  char* turno_giocatore;
  tipo = tipoRnd;
  tipost = stanza_inizio[3].tipoStanza;
  for (int i = 0; i<=nGiocatori; i++) {
    if(i>=nGiocatori){
      i = 0;
    }
    turni_giocatori[i] = giocatori[i].nome;
    switch (giocatori[i].nome) {
      case 0: turno_giocatore = "rosso";
      break;
      case 1: turno_giocatore = "blu";
      break;
      case 2: turno_giocatore = "verde";
      break;
      case 3: turno_giocatore = "giallo";
      break;
      case 4: turno_giocatore = "rosa";
      break;
      case 5: turno_giocatore = "viola";
      break;
      case 6: turno_giocatore = "bianco";
      break;
      case 7: turno_giocatore = "nero";
      break;
      case 8: turno_giocatore = "arancione";
      break;
      case 9: turno_giocatore = "grigio";
      break;
    }
    for(int f = 0; f<nGiocatori; f++){

        morto[f] = giocatori[f].stato;

    }
    printf("+--------------------------------------------------+\n");
      printf("Turno numero: %d\n\n", turno_attuale);
      printf("Tipo della stanza: %s\n\n", tipo);

      printf("Giocatori nella stanza: \n");
      player(i);
      printf("\n");
      printf("Turno del giocatore: %s\n", turno_giocatore);

      if(giocatori[i].stato == 0){
                printf("|-> sei un AUSTRONAUTA\n\n");
                printf("1) Avanza\n\n");
                printf("2) Esegui quest\n\n");
                printf("3) Chiamata di emergenza\n\n");
                printf("4) Resta fermo\n");
                fflush(stdin);
                scanf("%d", &azione);
                while (azione!=1 && azione!=2 && azione!=3 && azione!=4){
                    printf("Scegli una delle 3 opzioni!\n");
                    while (getchar() != '\n');
                    scanf("%d", &azione);
                }
                while(tipost != 1 && tipost != 2 && azione == 2){
                  printf("Non ci sono quest in questa stanza!\n");
                  while (getchar() != '\n');
                  scanf("%d", &azione);
              }
              int check = 0;
              for (int k = 0; k < nGiocatori; k++) {
                if(giocatori[i].posizione == giocatori[k].posizione){
                  if(morto[k] == 2){
                    check = 1;
                    }
                  }
                }
                while (check == 0  && azione == 3) {
                  printf("Non ci sono assassinati qui!\n");
                  scanf("%d", &azione);
                }
                while (lista_stanze[nStanze].emergenza_chiamata == true && azione == 3) {
                printf("L'emergenza è già stata chiamata qui!\n");
                scanf("%d", &azione);
                }

                switch (azione) {
                  case 1: avanza(i);
                  break;
                  case 2:
                    esegui_quest();
                  break;
                  case 3:
                    chiamata_emergenza(i);
                  break;
                  case 4:
                  break;
                }
        }

        else if(giocatori[i].stato == 1){
          printf("|-> sei un IMPOSTORE\n\n");
          printf("1) Avanza\n\n");
          printf("2) Uccidi astronauta\n\n");
          printf("3) Chiamata di emergenza\n\n");
          printf("4) Sabotaggio\n\n");
          printf("5) Usa botola\n\n");
          printf("6) Resta fermo\n");
          scanf("%d", &azione);
          while (azione!=1 && azione!=2 && azione!=3 && azione!=4 && azione!=5 && azione!=6){
              printf("Scegli una delle 3 opzioni!\n");
              while (getchar() != '\n');
              scanf("%d", &azione);
          }
          while(tipost != 1 && tipost != 2 && azione == 4){
          printf("Non ci sono quest da sabotare in questa stanza!\n");
          while (getchar() != '\n');
          scanf("%d", &azione);
          }
          while(tipost != 3 && azione == 5){
          printf("Non ci sono botole da usare in questa stanza!\n");
          while (getchar() != '\n');
          scanf("%d", &azione);
          }
          int check = 0;
          for (int k = 0; k < nGiocatori; k++) {
            if(giocatori[i].posizione == giocatori[k].posizione){
              if(morto[k] == 2){
                check = 1;
                }
              }
            }
            while (check == 0  && azione == 3) {
              printf("Non ci sono assassinati qui!\n");
              scanf("%d", &azione);
            }
            while (lista_stanze[nStanze].emergenza_chiamata == true && azione == 3) {
            printf("L'emergenza è già stata chiamata qui!\n");
            scanf("%d", &azione);
            }
          switch (azione) {
            case 1: avanza(i);
            break;
            case 2: uccidi_astronauta(i);
            break;
            case 3:
              chiamata_emergenza(i);
            break;
            case 4:
            sabotaggio();
            break;
            case 5:
            usa_botola(i);
            break;
            case 6:
            break;
          }
      }
      verifica();
      turno_attuale++;
      fflush(stdin);
      fflush(stdout);
    }
}


void avanza(int i){
  int scelta = 0;
  printf("Scegli in quale direzione andare:\n");
  printf("1) Avanti\n");
  printf("2) Destra\n");
  printf("3) Sinistra\n");
  scanf("%d", &scelta);
  while (scelta!=1 && scelta!=2 && scelta!=3){
      printf("Scegli una delle 3 opzioni!\n");
      while (getchar() != '\n');
      scanf("%d", &scelta);
  }
  switch(scelta){
    case 1:
    avanti(i);
    break;
    case 2:
    destra(i);
    break;
    case 3:
    sinistra(i);
    break;
  }
}

void avanti(int i){
  printf("POSIZIONE PRIMA: %p\n", giocatori[i].posizione);
  lista_stanze[nStanze].stanza_precedente = giocatori[i].posizione;
  struct Stanza *stanza = giocatori[i].posizione;
  if(stanza->avanti == NULL){
    nStanze++;
    struct Stanza *stanzaAvanti = (struct Stanza*)malloc(sizeof(struct Stanza));
    int rnd = 0;
    for(int i =0;i<4;i++){
    rnd = (rand()%100+1);
    if(rnd <= 15){
      stanzaAvanti[i].tipoStanza = 2;
    }
    else if(rnd <= 40 && rnd > 15){
      stanzaAvanti[i].tipoStanza = 3;
    }

    else if(rnd <= 70 && rnd > 40){
      stanzaAvanti[i].tipoStanza = 1;
    }
    else if(rnd <= 100 && rnd > 70){
      stanzaAvanti[i].tipoStanza = 0;
    }

  switch (stanzaAvanti[i].tipoStanza) {
   case 0: tipoRnd1 = "vuota";
    break;
   case 1:  tipoRnd1 = "quest semplice";
    break;
    case 2:  tipoRnd1 = "quest doppia";
    break;
    case 3:  tipoRnd1 = "botola";
    lista_stanze[nStanze].pos = stanza->avanti;
    break;
  }
  }

  tipo = tipoRnd1;
  tipost = stanzaAvanti[3].tipoStanza;
  lista_stanze[nStanze].tipoStanza = tipost;
    stanzaAvanti->avanti = NULL;
    stanzaAvanti->destra = NULL;
    stanzaAvanti->sinistra = NULL;
    stanzaAvanti->stanza_precedente = NULL;
    stanzaAvanti->stanza_successiva = NULL;
    stanza->avanti = stanzaAvanti;
    struct Stanza *stanzaTmp = lista_stanze;
    while(stanzaTmp->stanza_successiva != NULL){  //finchè c'è una stanza successiva, viene salvata in stanzaTmp
      stanzaTmp = stanzaTmp->stanza_successiva;
    }
    stanzaTmp->stanza_successiva = stanzaAvanti; //quando esce dal whie la stanza successiva diventa NULL e viene messa nella stanza successiva quella avanti
    stanzaTmp = stanzaAvanti; //la stanzaTmp diventa quella successiva
    stanzaTmp->stanza_successiva = NULL; //stanza successiva diventa NULL
    lista_stanze->stanza_successiva = stanzaAvanti;
    stanza->emergenza_chiamata = false;

  }
  if(tipost == 3){
      lista_stanze[nStanze].pos = stanza->avanti;
  }
  giocatori[i].posizione = stanza->avanti;
  lista_stanze->avanti = stanza->avanti;
printf("POSIZIONE DOPO: %p\n", giocatori[i].posizione);
}

void destra(int i){
  printf("POSIZIONE PRIMA: %p\n", giocatori[i].posizione);
  lista_stanze[nStanze].stanza_precedente = giocatori[i].posizione;
  struct Stanza *stanza = giocatori[i].posizione;
  if(stanza->destra == NULL){
    nStanze++;
    struct Stanza *stanzaDestra = (struct Stanza*)malloc(sizeof(struct Stanza));
    int rnd = 0;
    for(int i =0;i<4;i++){
    rnd = (rand()%100+1);
    if(rnd <= 15){
      stanzaDestra[i].tipoStanza = 2;
    }
    else if(rnd <= 40 && rnd > 15){
      stanzaDestra[i].tipoStanza = 3;
    }

    else if(rnd <= 70 && rnd > 40){
      stanzaDestra[i].tipoStanza = 1;
    }
    else if(rnd <= 100 && rnd > 70){
      stanzaDestra[i].tipoStanza = 0;
    }

  switch (stanzaDestra[i].tipoStanza) {
   case 0: tipoRnd1 = "vuota";
    break;
   case 1:  tipoRnd1 = "quest semplice";
    break;
    case 2:  tipoRnd1 = "quest doppia";
    break;
    case 3:  tipoRnd1 = "botola";
    lista_stanze[nStanze].pos = stanza->destra;
    break;
  }
  }
  tipo = tipoRnd1;
  tipost = stanzaDestra[3].tipoStanza;
  lista_stanze[nStanze].tipoStanza = tipost;
    stanzaDestra->avanti = NULL;
    stanzaDestra->destra = NULL;
    stanzaDestra->sinistra = NULL;
    stanzaDestra->stanza_precedente = NULL;
    stanzaDestra->stanza_successiva = NULL;
    stanza->destra = stanzaDestra;
    struct Stanza *stanzaTmp = lista_stanze;
    while(stanzaTmp->stanza_successiva != NULL){  //finchè c'è una stanza successiva, viene salvata in stanzaTmp
      stanzaTmp = stanzaTmp->stanza_successiva;
    }
    stanzaTmp->stanza_successiva = stanzaDestra; //quando esce dal whie la stanza successiva diventa NULL e viene messa nella stanza successiva quella avanti
    stanzaTmp = stanzaDestra; //la stanzaTmp diventa quella successiva
    stanzaTmp->stanza_successiva = NULL; //stanza successiva diventa NULL
    lista_stanze->stanza_successiva = stanzaDestra;
    stanza->emergenza_chiamata = false;
  }
  if(tipost == 3){
      lista_stanze[nStanze].pos = stanza->destra;
  }
  giocatori[i].posizione = stanza->destra;
  lista_stanze->destra = stanza->destra;
  printf("POSIZIONE DOPO: %p\n", giocatori[i].posizione);
}

void sinistra(int i){
printf("POSIZIONE PRIMA: %p\n", giocatori[i].posizione);
lista_stanze[nStanze].stanza_precedente = giocatori[i].posizione;
  struct Stanza *stanza = giocatori[i].posizione;
  stanza->stanza_precedente = giocatori[i].posizione;
  if(stanza->sinistra == NULL){
    nStanze++;
    struct Stanza *stanzaSinistra = (struct Stanza*)malloc(sizeof(struct Stanza));
    int rnd = 0;
    for(int i =0;i<4;i++){
    rnd = (rand()%100+1);
    if(rnd <= 15){
      stanzaSinistra[i].tipoStanza = 2;
    }
    else if(rnd <= 40 && rnd > 15){
      stanzaSinistra[i].tipoStanza = 3;
    }

    else if(rnd <= 70 && rnd > 40){
      stanzaSinistra[i].tipoStanza = 1;
    }
    else if(rnd <= 100 && rnd > 70){
      stanzaSinistra[i].tipoStanza = 0;
    }

  switch (stanzaSinistra[i].tipoStanza) {
   case 0: tipoRnd1 = "vuota";
    break;
   case 1:  tipoRnd1 = "quest semplice";
    break;
    case 2:  tipoRnd1 = "quest doppia";
    break;
    case 3:  tipoRnd1 = "botola";
    break;
  }
  }
  tipo = tipoRnd1;
  tipost = stanzaSinistra[3].tipoStanza;
  lista_stanze[nStanze].tipoStanza = tipost;
    stanzaSinistra->avanti = NULL;
    stanzaSinistra->sinistra = NULL;
    stanzaSinistra->sinistra = NULL;
    stanzaSinistra->stanza_precedente = NULL;
    stanzaSinistra->stanza_successiva = NULL;
    stanza->sinistra = stanzaSinistra;
    struct Stanza *stanzaTmp = lista_stanze;
    while(stanzaTmp->stanza_successiva != NULL){  //finchè c'è una stanza successiva, viene salvata in stanzaTmp
      stanzaTmp = stanzaTmp->stanza_successiva;
    }
    stanzaTmp->stanza_successiva = stanzaSinistra; //quando esce dal whie la stanza successiva diventa NULL e viene messa nella stanza successiva quella avanti
    stanzaTmp = stanzaSinistra; //la stanzaTmp diventa quella successiva
    stanzaTmp->stanza_successiva = NULL; //stanza successiva diventa NULL
    lista_stanze->stanza_successiva = stanzaSinistra;
    stanza->emergenza_chiamata = false;

  }
  if(tipost == 3){
      lista_stanze[nStanze].pos = stanza->sinistra;
    }
  giocatori[i].posizione = stanza->sinistra;
  lista_stanze->sinistra = stanza->sinistra;
  printf("POSIZIONE DOPO: %p\n", giocatori[i].posizione);
}

void esegui_quest() {
    if(tipost == 1){
      quest_da_finire--;
    }
    else if(tipost == 2){
      quest_da_finire = quest_da_finire - 2;
    }
  tipost = 0;
  if(tipost == 0){
    tipo = "vuota";
  }
  if(quest_da_finire<=0 || quest_da_finire>31){
    printf("\n+--------------------------------------------------+\n");
    printf("|            ASTRONAUTI.. AVETE VINTO!             |\n");
    printf("|           avete finito tutte le quest!           |\n");
    printf("|            GRAZIE PER AVER GIOCATO!              |\n");
    printf("+--------------------------------------------------+\n");
    libera();
    exit(0);
  }
  printf("Quest mancanti: %d\n", (int)quest_da_finire);
}

void chiamata_emergenza(int j) {
  char* name;
  int nImp = 0;
  int nAst = 0;
  int rnd2 = (rand()%nGiocatori);
  for (int i = 0; i < nGiocatori; i++) {
    if(giocatori[j].posizione == giocatori[i].posizione){
      if(giocatori[i].stato == 0){
        nAst++;
      }
      if(giocatori[i].stato == 1){
        nImp++;
      }
    }

    switch (giocatori[rnd2].nome) {
      case 0: name = "rosso";
      break;
      case 1: name = "blu";
      break;
      case 2: name = "verde";
      break;
      case 3: name = "giallo";
      break;
      case 4: name = "rosa";
      break;
      case 5: name = "viola";
      break;
      case 6: name = "bianco";
      break;
      case 7: name = "nero";
      break;
      case 8: name = "arancione";
      break;
      case 9: name = "grigio";
      break;
    }
    }
    int rnd1 = (rand()%100+1);
    int rnd = (rand()%100+1);
    char ch;

      if(giocatori[rnd2].stato == 0) {
        while (rnd >= 30+(nImp*20)-((nAst-1)*30)) {
          rnd = (rand()%100+1);
        }
        if(rnd <= 30+(nImp*20)-((nAst-1)*30)){
        giocatori[rnd2].stato = 3;
        printf("A seguito della chiamata di emergenza, Il giocatore %s è\n", name);
        printf(" stato defenestrato!\n");
        ch = getchar();
        scanf("%c", &ch);
        fflush(stdin);
        fflush(stdout);
        lista_stanze[nStanze].emergenza_chiamata = true;
        ok = 1;
      }
    }
  if(giocatori[rnd2].stato == 1) {
    while (rnd1 >= 30+(nImp*20)-((nAst-1)*30)) {
      rnd1 = (rand()%100+1);
    }
      if(rnd1 <= 30+(nAst*20)-((nImp-1)*30)){
        giocatori[rnd2].stato = 3;
        printf("A seguito della chiamata di emergenza, Il giocatore %s è\n", name);
        printf("stato defenestrato!\n");
        ch = getchar();
        scanf("%c", &ch);
        fflush(stdin);
        fflush(stdout);
        printf("%d\n", nStanze);
        lista_stanze[nStanze].emergenza_chiamata = true;
        ok=1;
      }
    }



  }


void uccidi_astronauta(int j) {
  char* getColore1;
  char* getColore3;
  int giocatoriNellaStanza = 0;
  printf("Quale giocatore vuoi assassinare?:\n");
  for (int i = 0; i < nGiocatori; i++) {
    switch (giocatori[i].nome) {
      case 0: getColore1 = "rosso";
      break;
      case 1: getColore1 = "blu";
      break;
      case 2: getColore1 = "verde";
      break;
      case 3: getColore1 = "giallo";
      break;
      case 4: getColore1 = "rosa";
      break;
      case 5: getColore1 = "viola";
      break;
      case 6: getColore1 = "bianco";
      break;
      case 7: getColore1 = "nero";
      break;
      case 8: getColore1 = "arancione";
      break;
      case 9: getColore1 = "grigio";
      break;
    }
    if(giocatori[j].posizione == giocatori[i].posizione && giocatori[j].nome != giocatori[i].nome && giocatori[i].stato == 0) {
      printf("%d) %s \n", i, getColore1);
      giocatoriNellaStanza++;
    }
  }
  scanf("%d", &uccidi);
   for (int i = 0; i < nGiocatori; i++) {
if(giocatori[j].posizione == giocatori[i].posizione && giocatori[j].nome != giocatori[i].nome && giocatori[i].stato == 0){
  if(uccidi == giocatori[i].nome){
    uccidiboolean = 1;
  }

  }
}


   for (int i = 0; i < nGiocatori; i++) {
     if(giocatori[j].posizione == giocatori[i].posizione && giocatori[j].nome != giocatori[i].nome && giocatori[i].stato == 0){
       if(uccidi == giocatori[i].nome){
       uccidiboolean = 1;
     }
     }
   }


  switch (giocatori[uccidi].nome) {
    case 0: getColore3 = "rosso";
    break;
    case 1: getColore3 = "blu";
    break;
    case 2: getColore3 = "verde";
    break;
    case 3: getColore3 = "giallo";
    break;
    case 4: getColore3 = "rosa";
    break;
    case 5: getColore3 = "viola";
    break;
    case 6: getColore3 = "bianco";
    break;
    case 7: getColore3 = "nero";
    break;
    case 8: getColore3 = "arancione";
    break;
    case 9: getColore3 = "grigio";
    break;
  }


  switch (uccidi) {
      case 0: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);

      break;
      case 1: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);

      break;
      case 2: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);


      break;
      case 3: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);


      break;
      case 4: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);


      break;
      case 5: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);


      break;
      case 6: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);

      break;
      case 7: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);

      break;
      case 8: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);

      break;
      case 9: giocatori[uccidi].stato = 2;
      printf("* il Giocatore %s è stato assassinato! *\n", getColore3);
      break;
    }
    cha = getchar();
    scanf("%c", &cha);

  int rnd3 = (rand()%100+1);
      if(rnd3 <= (giocatoriNellaStanza*50)){
        giocatori[j].stato = 3;
        printf("Oh no.. Ti hanno scoperto! Sei stato defenestrato! ...\n");
        cha = getchar();
        scanf("%c", &cha);
      }
}


void sabotaggio() {
  printf("tipost: %d\n", tipost);
  tipo = "vuota";
  tipost = 0;
  printf("La quest della stanza è stata sabotata!\n");
}


void usa_botola(int j){
  for (int i = 0; i < nStanze; i++) {
    if(lista_stanze[i].tipoStanza == 3){
      giocatori[j].posizione = lista_stanze[i].pos;
    }
    printf("pos giocatore %p\n", giocatori[j].posizione);
    printf("stanza con botola %p\n", lista_stanze[i].pos);
  }
}



void  player(int j) {
  char* getColore1;
  for (int i = 0; i < nGiocatori; i++) {
    switch (giocatori[i].nome) {
      case 0: getColore1 = "rosso";
      break;
      case 1: getColore1 = "blu";
      break;
      case 2: getColore1 = "verde";
      break;
      case 3: getColore1 = "giallo";
      break;
      case 4: getColore1 = "rosa";
      break;
      case 5: getColore1 = "viola";
      break;
      case 6: getColore1 = "bianco";
      break;
      case 7: getColore1 = "nero";
      break;
      case 8: getColore1 = "arancione";
      break;
      case 9: getColore1 = "grigio";
      break;
    }
    if(giocatori[j].posizione == giocatori[i].posizione && giocatori[j].nome != giocatori[i].nome ) {
      if(giocatori[i].stato != 3 && giocatori[i].stato != 2){
        printf("- ");
        printf("%s\n", getColore1);
      }
      if(giocatori[i].stato == 2){
        printf("- ");
        printf("%s *Morto*\n", getColore1);
      }
      if(giocatori[i].stato == 3 && giocatori[i].stato == 1){
        printf("- ");
        printf("%s *Defenestrato* - Impostore.\n", getColore1);
      }
      else if(giocatori[i].stato == 3){
        printf("- ");
        printf("%s *Defenestrato*\n", getColore1);
      }
    }
  }
    printf("\n");
}

void verifica(){
  int imp = 0;
  int ast = 0;
  for (int i = 0; i < nGiocatori; i++) {
    if(giocatori[i].stato == 0){
      ast++;
    }
    if(giocatori[i].stato == 1){
      imp++;
    }
  }
  if(imp <= 0){
    free(giocatori);
    free(stanza_inizio);
    printf("\n+--------------------------------------------------+\n");
    printf("|            ASTRONAUTI.. AVETE VINTO!             |\n");
    printf("|           non ci sono più impostori!             |\n");
    printf("|            GRAZIE PER AVER GIOCATO!              |\n");
    printf("+--------------------------------------------------+\n");
    libera();
    exit(0);
  }
  if(ast <= 1){
    printf("\n+--------------------------------------------------+\n");
    printf("|            IMPOSTORI.. AVETE VINTO!              |\n");
    printf("|           non ci sono più impostori!             |\n");
    printf("|            GRAZIE PER AVER GIOCATO!              |\n");
    printf("+--------------------------------------------------+\n");
    libera();
    exit(0);
  }

}

void termina_gioco() {
  printf("\n+--------------------------------------------------+\n");
  printf("|            GRAZIE PER AVER GIOCATO!              |\n");
  printf("+--------------------------------------------------+\n");
  libera();
  exit(0);
}

void libera() {
  free(giocatori);
  free(stanza_inizio);
  free(lista_stanze);
}

