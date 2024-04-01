#ifndef UTIL_H
#define UTIL_H

#include "aeroporto.h"
#include "companhia_aerea.h"

void mostrar_comandos();

char Comando_Selecionado(int argc, char *argv[]);

void Listar_Rotas_Diretas(Airline *listaAirlines, char *iata_origem, char *iata_destino);

void Listar_Rotas_Com_Uma_Escala(Airline *listaAirlines, char *iata_origem, char *iata_destino);

void Listar_Rotas_Com_Duas_Escalas(Airline *listaAirlines, char *iata_origem, char *iata_destino);

#endif /* UTIL_H */

