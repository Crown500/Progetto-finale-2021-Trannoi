#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


enum stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum nome_giocatore {rosso , blu, verde, giallo, rosa, viola, bianco, nero, arancione, grigio};
//enum nome_giocatore {0 ,    1,    2,     3,      4,     5,     6,     7,       8,       9};
enum tipo_stanza {vuota, quest_semplice, quest_doppia, botola};
struct Giocatore{
      struct Stanza* posizione;
      enum stato_giocatore stato;
      enum nome_giocatore nome;
};


struct Stanza{
  struct Stanza* pos;
  struct Stanza* avanti;
  struct Stanza* sinistra;
  struct Stanza* destra;
  struct Stanza* stanza_precedente;
  struct Stanza* stanza_successiva;
  enum tipo_stanza tipoStanza;
  bool emergenza_chiamata;
};


void imposta_gioco(void);
void gioca(void);
void termina_gioco(void);

