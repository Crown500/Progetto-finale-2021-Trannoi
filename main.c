#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "gamelib.h"

int imposta = 0;
int main() {
  int scelta = 0;
  printf("                        |-- TRANNOI --|\n");
  printf("Un gruppo di astronauti si trova in viaggio sull’astronave Skelt, il loro \nobiettivo è riuscire a completare tutte le attività previste (le quest) per \nil mantenimento della nave, arrivando così a destinazione.Tra di loro si celano\nperò anche degli impostori, il cui scopo è eliminare di nascosto gli\nastronauti senza farsi scoprire da essi. Riusciranno ad arrivare a\ndestinazione prima di essere decimati?\nTrannoi è liberamente ispirato ad un gioco esistente.\n\n");
  printf("|---------------------------------------------------------------|\n");
  printf("premi ENTER per continuare:");
  getchar();
  printf("\n\n\n");
  printf("|----------------------------MENU'----------------------------|\n");
  printf("1) - Imposta gioco\n");
  printf("2) - Gioca\n");
  printf("3) - Esci dal gioco\n");
  scanf("%d[^\n]", &scelta);
  while (scelta != 1 && scelta != 2 && scelta != 3) {
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
      imposta_gioco();
      break;
      case 2:
        gioca();
        break;
        case 3:
          termina_gioco();
          break;

  }

  return 0;
}

