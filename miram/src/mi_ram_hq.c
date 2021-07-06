/*
 ============================================================================
 Name        : mi_ram_hq.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "mi_ram_hq.h"


#define ASSERT_CREATE(nivel, id, err)

NIVEL* nivel;
t_config* config;

int tamMemoria;
char* esquemaMemoria;
int tamPagina;
int tamSwap;
char* path_swap;
char* alg_remplazo;
char* crit_seleccion;
char* puerto;
int memoria;

int main(void) {
	int socketCliente, socketServer;
	bool terminar;

	config = config_create("/home/utnso/Escritorio/Conexiones/miram/mi_ram_hq.config");
	tamMemoria = config_get_int_value(config, "TAMANIO_MEMORIA");
	esquemaMemoria = config_get_string_value(config, "ESQUEMA_MEMORIA");
	tamPagina = config_get_int_value(config, "TAMANIO_PAGINA");
	tamSwap = config_get_int_value(config, "TAMANIO_SWAP");
	path_swap = config_get_string_value(config, "PATH_SWAP");
	alg_remplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	crit_seleccion = config_get_string_value(config, "CRITERIO_SELECCION");
	puerto = config_get_string_value(config, "PUERTO");

	memoria = malloc(tamMemoria);

//	nivel = crear_mapa();

	socketServer = crear_server(puerto);
	while (1) {
		socketCliente = esperar_cliente(socketServer, 5);
		if (socketCliente == -1)
			continue;
//			pthread_t hiloCliente;
//			pthread_create(&hiloCliente,NULL,(void*)administrar_cliente,socketCliente);
//			pthread_join(hiloCliente,NULL);

//--------------- MOMENTANEO ------------
		int respuesta;
		t_paquete* paquete_recibido = recibir_paquete(socketCliente, &respuesta);

		if (paquete_recibido->codigo_operacion == -1 || respuesta == ERROR) {
			puts("No se pudo recibir correctamente el paquete");
			liberar_conexion(socketCliente);
			eliminar_paquete(paquete_recibido);
			continue;
		}

		printf("PAQUETE DE TIPO %d RECIBIDO\n",paquete_recibido->codigo_operacion);

		switch (paquete_recibido->codigo_operacion) {
		case INICIAR_PATOTA:;
			t_iniciar_patota* estructura_iniciar_patota = deserializar_iniciar_patota(paquete_recibido);
			imprimir_paquete_iniciar_patota(estructura_iniciar_patota);

//			liberar_iniciar_patota(estructura_iniciar_patota);
			liberar_conexion(socketCliente);
			break;

		case TRIPULANTE:;
			t_tripulante* estructura_tripulante = deserializar_tripulante(paquete_recibido);
			imprimir_paquete_tripulante(estructura_tripulante);

//			liberar_tripulante(estructura_tripulante);
			liberar_conexion(socketCliente);
			break;
		case ELIMINAR_TRIPULANTE:;
			t_eliminar_tripulante* tripulante_a_eliminar = deserializar_eliminar_tripulante(paquete_recibido);
			imprimir_paquete_eliminar_tripulante(tripulante_a_eliminar);

			break;

		case ACTUALIZAR_POS:;
			t_tripulante* tripulante_a_mover = deserializar_tripulante(paquete_recibido);
			imprimir_paquete_tripulante(tripulante_a_mover);

			break;

		case ACTUALIZAR_ESTADO:;
			t_cambio_estado* tripulante_a_actualizar = deserializar_cambio_estado(paquete_recibido);
			imprimir_paquete_cambio_estado(tripulante_a_actualizar);
			break;

		case PEDIR_TAREA:;
			t_tripulante* tripulante_solicitud = deserializar_tripulante(paquete_recibido);
			imprimir_paquete_tripulante(tripulante_solicitud);
//			char* tarea = buscar_tarea_correspondiente()
			char* tarea = "tarea de prueba";
			int tamanio_tarea = strlen(tarea)+1;
			send(socketCliente, &tamanio_tarea,sizeof(uint32_t),0);
			send(socketCliente, tarea,tamanio_tarea,0);

			liberar_conexion(socketCliente);
			break;

		default:;
			printf("No se especifico el caso para recibir el paquete tipo: %d\n",paquete_recibido->codigo_operacion);
			break;
		}
	}


	liberar_conexion(socketServer);
	//crear_personajes(nivel, patota);
	return EXIT_SUCCESS;
	}



void dibujarTripulante(Tripulante* tripulante){
	int err;
	//char* id[3] = '0';
	char id = intAChar(tripulante->id);
	err = personaje_crear(nivel, id, 10, 10);
	ASSERT_CREATE(nivel, id, err);

	if(err) {
		printf("Error: %s\n", nivel_gui_string_error(err));
	}

	//free (id);
}

int crear_mapa(){
		NIVEL* nivel;

		int cols, rows;
		int err;

		nivel_gui_inicializar();

		nivel_gui_get_area_nivel(&cols, &rows);

		nivel = nivel_crear("Test Chamber 04");
		nivel_gui_dibujar(nivel);
		printf("Ya Dibuje \n");
		return nivel;
}



int procesar_archivo(FILE* archivoTareas){
	archivoTareas = fopen("Tareas.txt","r");

	if(archivoTareas == NULL){
		printf("Error! No se pudo abrir el archivo\n");
		exit(-1);
	}

	char tarea[30];
	while (fgets(tarea, 30, archivoTareas) != NULL){
		//Guardar en memoria
	}
	fclose(archivoTareas);
	//return posicionDeMemoriaDePrimerTarea
}

//void administrar_cliente(int socketCliente){
//	printf("El hilo fue creado correctamente\n");
//
//	t_paquete* paquete = malloc(sizeof(t_paquete));
//	paquete->buffer = malloc(sizeof(t_buffer));
//
//	// Primero recibimos el codigo de operacion
//	recv(socketCliente, &(paquete->codigo_operacion), sizeof(uint8_t), 0);
//
//	// Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
//	recv(socketCliente, &(paquete->buffer->size), sizeof(uint32_t), 0);
//	printf("Size a allocar: %d \n", paquete->buffer->size);
//
//
//	paquete->buffer->stream = malloc(paquete->buffer->size);
//	recv(socketCliente, paquete->buffer->stream, paquete->buffer->size, 0);
//
//	// Ahora en función del código recibido procedemos a deserializar el resto
//	printf("Recibi todo bien, codigo de operacion: %d\n", paquete->codigo_operacion);
//
//	switch(paquete->codigo_operacion) {
//		case INICIAR_PATOTA:;
//			printf("Inicio una patota /n");
//			iniciar_patota* nuevaPatota;
//			nuevaPatota = deserializar_iniciar_patota(paquete->buffer);
//	    	printf("ID de la patota: %d \n",nuevaPatota -> idPatota);
//			//PCB* pcb;
//	    	//pcb ->	pid = nuevaPatota -> idPatota;
//	    	/*uint32_t* tareas = procesar_archivo(nuevaPatota -> Tareas);
//	    	pcb -> tareas = tareas;
//	    	//Guardar PCB en memoria
//	    	uint32_t* punteroPCB;
//	    	for(int i = 0;i < nuevaPatota -> cantTripulantes;i++){
//	    		recv(socketCliente, &(paquete->codigo_operacion), sizeof(uint8_t), 0);
//	    		recv(socketCliente, &(paquete->buffer->size), sizeof(uint32_t), 0);
//	    		paquete->buffer->stream = malloc(paquete->buffer->size);
//	    		recv(socketCliente, paquete->buffer->stream, paquete->buffer->size, 0);
//	    		Tripulante* tripulante;
//	    		tripulante = deserializar_tripulante(paquete->buffer);
//	    		iniciar_tripulante(tripulante, pcb);
//	    	}*/
//	    	break;
//		case ACTUALIZAR_POS:;
//			printf("Me pidio moverlo en el mapa \n");
//			/*id_and_pos* nuevaPos;
//			nuevaPos = deserializar_id_and_pos(paquete->buffer);
//			actualizarPosicion(nuevaPos, nivel);*/
//			break;
//		case PEDIRTAREA:
//			printf("Me pidio una tarea \n");
//			break;
//		case ELIMINAR_TRIPULANTE:
//			printf("Eliminar tripulante \n");
//			break;
//		case ACTUALIZAR_ESTADO:
//			printf("Me pidio actualizar estado \n");
//			break;
//		case TRIPULANTE:
//			printf("Me mando un tripulante nuevo \n");
//			break;
//		default:
//			printf("No tengo registrado ese case \n");
//			break;
//
//	}
//}

char intAChar(int numero){
	return numero + '0';
}

void actualizarPosicion(id_and_pos* nuevaPos, NIVEL* nivel){
	char idAMover = intAChar(nuevaPos->idTripulante);
	item_mover(nivel, idAMover, nuevaPos-> posX,nuevaPos->posY);
}



void iniciar_tripulante(Tripulante* tripulante, uint32_t* pcb){
	TCB* tcb = crearTCB(tripulante,pcb);
	dibujarTripulante(tripulante);
}

TCB* crearTCB(Tripulante* tripulante, uint32_t* pcb){
	TCB* tcb;
	tcb -> tid = tripulante -> id;
	tcb -> posx = tripulante -> posicionX;
	tcb -> posy = tripulante -> posicionY;
	tcb -> estado = tripulante -> estado;
	tcb ->puntero_pcb = pcb;

	return tcb;
}
