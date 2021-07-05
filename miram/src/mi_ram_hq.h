/*
 * mi_ram_hq.h
 *
 *  Created on: 29 may. 2021
 *      Author: utnso
 */

#ifndef MI_RAM_HQ_H_
#define MI_RAM_HQ_H_

#include <nivel-gui/nivel-gui.h>
#include <nivel-gui/tad_nivel.h>
#include <curses.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <shared/conexion.h>
#include <commons/config.h>

#define PUERTOPREDETERMINADO 6667

pthread_mutex_t mutex_lista_restaurantes;


typedef struct
{
   uint32_t pid;
   uint32_t* tareas ;
}PCB;

typedef struct
{
   uint32_t tid;
   char* estado;
   uint32_t posx;
   uint32_t posy;
   uint32_t* prox_tarea;
   uint32_t* puntero_pcb;
}TCB;

void dibujarTripulante(Tripulante*);
int crear_mapa();
//void administrar_cliente(id_and_pos,NIVEL);
TCB* crearTCB(Tripulante*, uint32_t*);
void administrar_cliente(int);
char intAChar(int);



#endif /* MI_RAM_HQ_H_ */