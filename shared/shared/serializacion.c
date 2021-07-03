/*
 * serializacion.c
 *
 *  Created on: 6 may. 2021
 *      Author: utnso
 */
#include "conexion.h"
#include "TAD_PATOTA.h"
#include "TAD_TRIPULANTE.h"

//==============================INICIAR_PATOTA========================================

//void serializar_iniciar_patota( iniciar_patota* tareaPatota, int socket)
//{
//	// Lo que deberia enviar es el id de la patota, el FILE con las tareas y la cant de tripulantes
//	// Para eso ya creamos el TAD Inicio_patota
//	// POR FAVOR CAMBIALE EL NOMBRE POR ALGO MAS CLARO, HAY 3 QUE SON CASI IGUALES
//	// POR FAVOR CAMBIALE EL NOMBRE POR ALGO MAS CLARO, HAY 3 QUE SON CASI IGUALES
//	// POR FAVOR CAMBIALE EL NOMBRE POR ALGO MAS CLARO, HAY 3 QUE SON CASI IGUALES
//	// POR FAVOR CAMBIALE EL NOMBRE POR ALGO MAS CLARO, HAY 3 QUE SON CASI IGUALES
//	t_buffer* buffer = malloc(sizeof(t_buffer));
//
//		buffer->size = sizeof(uint8_t)  // Id
//					 + sizeof(uint32_t)//longitud de tarea
//		             + sizeof(FILE); // Tarea
//		             // La longitud del string nombre. Le sumamos 1 para enviar tambien el caracter centinela '\0'. Esto se podría obviar, pero entonces deberíamos agregar el centinela en el receptor.
//
//		void* stream = malloc(buffer->size);
//		int offset = 0; // Desplazamiento
//		memcpy(stream+offset,&(tareaPatota->idPatota),sizeof(uint8_t));
//		offset+=sizeof(uint8_t);
//		memcpy(stream+offset,&tareaPatota->cantTripulantes,sizeof(uint8_t));
//		offset+=sizeof(uint32_t);
//		memcpy(stream+offset,tareaPatota->Tareas,sizeof(FILE));
//
//		buffer->stream=stream;
//		t_paquete* paquete = malloc(sizeof(t_paquete));
//
//
//			//CODIGO DE OPERACION 1 = UN TRIUPLANTE
//			paquete->codigo_operacion = INICIAR_PATOTA; // Podemos usar una constante por operación
//			paquete->buffer = buffer; // Nuestro buffer de antes.
//
//			// Armamos el stream a enviar
//			void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
//			offset = 0;
//
//			memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
//
//			send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);
//
//			// No nos olvidamos de liberar la memoria que ya no usaremos
//			free(a_enviar);
//			free(paquete->buffer->stream);
//			free(paquete->buffer);
//			free(paquete);
//
//
//}
//iniciar_patota* deserializar_iniciar_patota(t_buffer* buffer)
//{
//	iniciar_patota* tareas=malloc(sizeof(tareasPatota));
//
//	void* stream=buffer->stream;
//
//	memcpy(&(tareas->idPatota),stream,sizeof(uint8_t));
//	stream+=sizeof(uint8_t);
//	memcpy(&(tareas->cantTripulantes),stream,sizeof(uint8_t));
//	stream+=sizeof(uint8_t);
//	memcpy(&(tareas->Tareas),stream,sizeof(FILE*));
//
//	return tareas;
//
//
//}
void agregar_paquete_iniciar_patota(t_paquete* paquete, t_iniciar_patota* estructura){
	int offset = 0;
	paquete->buffer->size += sizeof(uint32_t) + sizeof(uint8_t)*2 + estructura->tamanio_tareas;

	paquete->buffer->stream = realloc(paquete->buffer->stream,paquete->buffer->size);

	memcpy(paquete->buffer->stream, &(estructura->idPatota), sizeof(uint8_t));		//El entero para el idpatota
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->cantTripulantes), sizeof(uint8_t));		//El entero de la cantidad de tcb
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->tamanio_tareas), sizeof(uint32_t));		//El entero para el tamanio de tareas
	offset += sizeof(uint32_t);
	memcpy(paquete->buffer->stream + offset, estructura->Tareas, estructura->tamanio_tareas);	//el archivo de tareas


}

t_iniciar_patota* deserializar_iniciar_patota(t_paquete* paquete){

	t_iniciar_patota* estructura = malloc(sizeof(t_iniciar_patota));
	int offset = 0;

	memcpy(&(estructura->idPatota), paquete->buffer->stream, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(&(estructura->cantTripulantes), paquete->buffer->stream + offset , sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(&(estructura->tamanio_tareas), paquete->buffer->stream + offset , sizeof(uint32_t));
	offset += sizeof(uint32_t);

	estructura->Tareas = (char*) malloc( (int) estructura->tamanio_tareas); //Es necesario  el casteo a (int)??
	memcpy(estructura->Tareas, paquete->buffer->stream + offset , estructura->tamanio_tareas);

	eliminar_paquete(paquete);
	return estructura;

}

void imprimir_paquete_iniciar_patota(t_iniciar_patota* estructura){
	printf("ID PATOTA: %d\n",estructura->idPatota);
	printf("CANTIDAD DE TRIPULANTES: %d\n",estructura->cantTripulantes);
	printf("TAMAÑO DE TAREAS: %d\n",estructura->tamanio_tareas);
	printf("TAREAS: %s\n",estructura->Tareas);
	puts("");
}

void liberar_t_iniciar_patota(t_iniciar_patota* estructura){
	free(estructura->Tareas);
	free(estructura);
}

//==============================TRIPUALNTE========================================

/*	---- BORRAR, lo guardo momentamente por si tenemos que volver a serializar estos campos
	void agregar_paquete_tripulante(t_paquete* paquete, Tripulante* estructura){
	int offset = 0;

	paquete->buffer->size += sizeof(uint32_t)*2 + sizeof(uint8_t)*4 + estructura->estado_length + estructura->Tarea_length + sizeof(pthread_t);
	paquete->buffer->stream = realloc(paquete->buffer->stream,paquete->buffer->size);

	memcpy(paquete->buffer->stream, &(estructura->id), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->idPatota), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->estado_length), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete->buffer->stream + offset, estructura->estado, estructura->estado_length);
	offset += estructura->estado_length;
	memcpy(paquete->buffer->stream + offset, &(estructura->Tarea_length), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete->buffer->stream + offset, estructura->Tarea, estructura->Tarea_length);
	offset += estructura->Tarea_length;
	memcpy(paquete->buffer->stream + offset, &estructura->posicionX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &estructura->posicionY, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->hilo), sizeof(pthread_t));
}

Tripulante* deserializar_tripulante(t_paquete* paquete){
	Tripulante* estructura = malloc(sizeof(Tripulante));
	int offset = 0;

    memcpy(&(estructura->id), paquete->buffer->stream, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->idPatota), paquete->buffer->stream + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->estado_length), paquete->buffer->stream + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    estructura->estado = malloc(estructura->estado_length);
    memcpy(estructura->estado, paquete->buffer->stream + offset, estructura->estado_length);
    offset += estructura->estado_length;

    memcpy(&(estructura->Tarea_length), paquete->buffer->stream + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    estructura->Tarea = malloc(estructura->Tarea_length);
    memcpy(estructura->Tarea, paquete->buffer->stream + offset, estructura->Tarea_length);
    offset += estructura->Tarea_length;

    memcpy(&(estructura->posicionX), paquete->buffer->stream + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->posicionY), paquete->buffer->stream + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&(estructura->hilo),paquete->buffer->stream + offset,sizeof(pthread_t));

	eliminar_paquete(paquete);
	return estructura;



}


 */

//ESTA FUNCION DESPUES TENEMOS QUE LLAMARLA EN NUESTRO MAIN (VER DOCUMENTACION DE SERIALIZACION)

//Tripulante* deserializar_tripulante(t_buffer* buffer) {
//	Tripulante* unTripulante = malloc(sizeof(Tripulante));
//
//    void* stream = buffer->stream;
//    // Deserializamos los campos que tenemos en el buffer
//    memcpy(&(unTripulante->id), stream, sizeof(uint8_t));
//    stream += sizeof(uint8_t);
//    memcpy(&(unTripulante->idPatota), stream, sizeof(uint8_t));
//    stream += sizeof(uint8_t);
//    memcpy(&(unTripulante->posicionX), stream, sizeof(uint8_t));
//    stream += sizeof(uint8_t);
//    memcpy(&(unTripulante->posicionY), stream, sizeof(uint8_t));
//    stream += sizeof(uint8_t);
//    memcpy(&(unTripulante->estado_length), stream, sizeof(uint32_t));
//    stream += sizeof(uint32_t);
//    unTripulante->estado = malloc(unTripulante->estado_length);
//    memcpy(&(unTripulante->estado), stream, unTripulante->estado_length);
//    stream += sizeof(uint32_t);
//    // Por último, para obtener el nombre, primero recibimos el tamaño y luego el texto en sí:
//    memcpy(&(unTripulante->Tarea_length), stream, sizeof(uint32_t));
//    stream += sizeof(uint32_t);
//    unTripulante->Tarea = malloc(unTripulante->Tarea_length);
//    memcpy(unTripulante->Tarea, stream, unTripulante->Tarea_length);
//    stream+=sizeof(uint32_t);
//    memcpy(&(unTripulante->hilo),stream,sizeof(pthread_t));
//
//    return unTripulante;
//}

void agregar_paquete_tripulante(t_paquete* paquete, t_tripulante* estructura){
	int offset = 0;

	paquete->buffer->size += sizeof(uint8_t)*4;
	paquete->buffer->stream = realloc(paquete->buffer->stream,paquete->buffer->size);

	memcpy(paquete->buffer->stream, &(estructura->id_tripulante), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->id_patota), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->posicion_x), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->posicion_y), sizeof(uint8_t));
}

t_tripulante* deserializar_tripulante(t_paquete* paquete){
	t_tripulante* estructura = malloc(sizeof(Tripulante));
	int offset = 0;

    memcpy(&(estructura->id_tripulante), paquete->buffer->stream, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->id_patota), paquete->buffer->stream + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->posicion_x), paquete->buffer->stream + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->posicion_y), paquete->buffer->stream + offset, sizeof(uint8_t));

	eliminar_paquete(paquete);
	return estructura;


}

void imprimir_paquete_tripulante(t_tripulante* estructura){
	printf("ID TRIPULANTE: %d\n",estructura->id_tripulante);
	printf("ID PATOTA: %d\n",estructura->id_patota);
	printf("POSICION EN X: %d\n",estructura->posicion_x);
	printf("POSICION EN Y: %d\n",estructura->posicion_y);
	puts("");
}

void liberar_t_tripulante(t_tripulante* estructura){
	free(estructura);
}

void serializar_tripulante(Tripulante* unTripulante, int socket){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	unTripulante->estado_length=strlen(unTripulante->estado)+1;
	unTripulante->Tarea_length=strlen(unTripulante->Tarea)+1;
	buffer->size = sizeof(uint8_t) * 3 // Id, posx, posy
	             + strlen(unTripulante->Tarea) + 1 // Tarea
	             + strlen(unTripulante->estado) + 1; // La longitud del string nombre. Le sumamos 1 para enviar tambien el caracter centinela '\0'. Esto se podría obviar, pero entonces deberíamos agregar el centinela en el receptor.

	void* stream = malloc(buffer->size);
	int offset = 0; // Desplazamiento

	memcpy(stream + offset, &unTripulante->id, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream+offset,&(unTripulante->idPatota),sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &unTripulante->posicionX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &unTripulante->posicionY, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &unTripulante->estado_length, sizeof(uint32_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, unTripulante->estado, strlen(unTripulante->estado));
	offset += sizeof(uint32_t);
	// Para la tarea primero mandamos el tamaño y luego el texto en sí:
	memcpy(stream + offset, &unTripulante->Tarea_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, unTripulante->Tarea, strlen(unTripulante->Tarea) + 1);
	offset +=strlen(unTripulante->Tarea) + 1;
	memcpy(stream + offset,&(unTripulante->hilo), sizeof(pthread_t));
	// No tiene sentido seguir calculando el desplazamiento, ya ocupamos el buffer completo

	buffer->stream = stream;

	// Si usamos memoria dinámica para el nombre, y no la precisamos más, ya podemos liberarla:

	//ESTO NO ESTA CHECKEADO SI ALGUIEN ENTIENDE q lo explique xd
	free(unTripulante->Tarea);
	free(unTripulante->estado);
	//LO Q SIGUE SI ESTA CHECKEADO


	t_paquete* paquete = malloc(sizeof(t_paquete));


	//CODIGO DE OPERACION 1 = UN TRIUPLANTE
	paquete->codigo_operacion = TRIPULANTE; // Podemos usar una constante por operación
	paquete->buffer = buffer; // Nuestro buffer de antes.

	// Armamos el stream a enviar
	void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
	offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

	send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

	// No nos olvidamos de liberar la memoria que ya no usaremos
	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);


}

//==============================ELIMINAR_TRIPUALNTE========================================

void serializar_eliminar_tripulante(int idTripulante, int socket)
{
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t);// Id
	void* stream = malloc(buffer->size);
	memcpy(stream,&idTripulante,sizeof(uint8_t));
	buffer->stream=stream;

	t_paquete* paquete = malloc(sizeof(t_paquete));


	//CODIGO DE OPERACION 1 = UN TRIUPLANTE
		paquete->codigo_operacion = ELIMINAR_TRIPULANTE; // Podemos usar una constante por operación
		paquete->buffer = buffer; // Nuestro buffer de antes.

				// Armamos el stream a enviar
	    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
		int offset = 0;

	   memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

	   send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

		// No nos olvidamos de liberar la memoria que ya no usaremos
		free(a_enviar);
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);

}

void agregar_paquete_eliminar_tripulante(t_paquete* paquete, t_eliminar_tripulante* estructura){
	int offset = 0;

	paquete->buffer->size += sizeof(uint8_t);
	paquete->buffer->stream = realloc(paquete->buffer->stream,paquete->buffer->size);

	memcpy(paquete->buffer->stream, &(estructura->id_tripulante), sizeof(uint8_t));

}
t_eliminar_tripulante* deserializar_eliminar_tripulante(t_paquete* paquete){
	t_eliminar_tripulante* estructura = malloc(sizeof(t_eliminar_tripulante));
	int offset = 0;

    memcpy(&(estructura->id_tripulante), paquete->buffer->stream, sizeof(uint8_t));

	eliminar_paquete(paquete);
	return estructura;
}

void imprimir_paquete_eliminar_tripulante(t_eliminar_tripulante* estructura){
	printf("TRIPULANTE A ELIMINAR: %d \n",estructura->id_tripulante);
	puts("");
}
void liberar_t_eliminar_tripulante(t_eliminar_tripulante* estructura){
	free(estructura);
}

//============================== CAMBIO_ESTADO ========================================


void serializar_cambio_estado(Tripulante* estado, int socket)
{

	estado->estado_length=strlen(estado->estado)+1;
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t) + sizeof(uint32_t) + strlen(estado->estado) + 1; //Id y estado nuevo
	void* stream = malloc(buffer->size);
	int offset = 0; // Desplazamiento
	memcpy(stream+offset,&estado->id,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(stream+offset,&estado->estado_length,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(stream+offset,&estado->estado,strlen(estado->estado)+1);
	buffer->stream=stream;

	t_paquete* paquete = malloc(sizeof(t_paquete));


	//CODIGO DE OPERACION 1 = UN TRIUPLANTE
	paquete->codigo_operacion = ACTUALIZAR_ESTADO; // Podemos usar una constante por operación
	paquete->buffer = buffer; // Nuestro buffer de antes.

	// Armamos el stream a enviar
	void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
				offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

	send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

				// No nos olvidamos de liberar la memoria que ya no usaremos
	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}


void agregar_paquete_cambio_estado(t_paquete* paquete, t_cambio_estado* estructura){
	int offset = 0;

	paquete->buffer->size += sizeof(uint8_t) * 2 + sizeof(uint32_t) + estructura->tamanio_estado;
	paquete->buffer->stream = realloc(paquete->buffer->stream,paquete->buffer->size);

	memcpy(paquete->buffer->stream, &(estructura->id_tripulante), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream + offset, &(estructura->id_patota), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(paquete->buffer->stream+offset, &(estructura->tamanio_estado), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete->buffer->stream+offset, estructura->estado, estructura->tamanio_estado);

}

t_cambio_estado* deserializar_cambio_estado(t_paquete* paquete){
	t_cambio_estado* estructura = malloc(sizeof(t_cambio_estado));
	int offset = 0;

    memcpy(&(estructura->id_tripulante), paquete->buffer->stream, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->id_patota), paquete->buffer->stream+offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(estructura->tamanio_estado), paquete->buffer->stream+offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    printf("tamaño a reservar para el char estado: %d \n",estructura->tamanio_estado);
    estructura->estado = (char* ) malloc( (int) estructura->tamanio_estado);
    memcpy(estructura->estado, paquete->buffer->stream + offset, estructura->tamanio_estado );

	eliminar_paquete(paquete);
	return estructura;

}
void imprimir_paquete_cambio_estado(t_cambio_estado* estructura){
	printf("ID TRIPULANTE: %d\n",estructura->id_tripulante);
	printf("ID PATOTA: %d\n",estructura->id_patota);
	printf("TAMAÑO DE ESTADO: %d\n",estructura->tamanio_estado);
	printf("ESTADO: %s\n",estructura->estado);
	puts("");
}
void liberar_t_cambio_estado(t_cambio_estado* estructura){
	free(estructura->estado);
	free(estructura);
}



//============================== CAMBIO_ESTADO ========================================

void serializar_patota( Patota* unaPatota, int socket)
{
	t_buffer* buffer=malloc(sizeof(t_buffer));
	buffer->size=sizeof(uint8_t) //id patota
			    +sizeof(Tripulante)*10 //array de tripulantes VER ESTO
				+strlen(unaPatota->tareas)+1;
	void* stream=malloc(buffer->size);
	int offset=0;
	memcpy(stream+offset,&(unaPatota->id),sizeof(uint8_t));
	offset+=sizeof(uint8_t);
	memcpy(stream+offset,&(unaPatota->tripulacion),sizeof(Tripulante)*10);
	offset+=sizeof(Tripulante)*10;
	memcpy(stream+offset,&(unaPatota->tareas_length), sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(stream+offset,&(unaPatota->tareas),strlen(unaPatota->tareas)+1);

	buffer->stream=stream;
	free(unaPatota->tareas);//esto?? no me borraria las tareas para siempre?? lol

	//bueno pa vamo mandar el paquete
	t_paquete* paquete = malloc(sizeof(t_paquete));

	//Definimos codigo de operacion PATOTA
	paquete->codigo_operacion= PATOTA;
	paquete->buffer=buffer;

	//armamos el stream a enviar
	void* a_enviar=malloc(buffer->size+sizeof(uint8_t)+sizeof(uint32_t));
    offset=0;
	memcpy(a_enviar+offset,&(paquete->codigo_operacion),sizeof(uint8_t));
	offset+=sizeof(uint8_t);
	memcpy(a_enviar+offset,&(paquete->buffer->size),sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(a_enviar+offset,&(paquete->buffer->stream),paquete->buffer->size);

	//ahora si enviamos todo
	send(socket,a_enviar, buffer->size+sizeof(uint8_t)+sizeof(uint32_t),0);

	//LIBERAR MEMOREA EVITAR MEMORY LEAK
	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free (paquete);

}
Patota* deserializarPatota(t_buffer* buffer)
{
	Patota* patota=malloc(sizeof(Patota));
	void*stream=buffer->stream;
	//Deserealizamos campo por campo mviendonos en el buffer

	memcpy(&(patota->id),stream,sizeof(uint8_t));
	stream+=sizeof(uint8_t);
	memcpy(&(patota->tripulacion),stream,sizeof(Tripulante)*10);
	stream+=sizeof(Tripulante)*10;
	memcpy(&(patota->tareas_length),stream,sizeof(uint32_t));
	stream+=sizeof(uint32_t);
	patota->tareas=malloc(patota->tareas_length);
	memcpy(&(patota->tareas),stream,patota->tareas_length);

	return patota;
}

void serializar_tarea_tripulante( Tripulante* tareaTrip, int socket)
{
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint8_t)*2;  // Id

	void* stream = malloc(buffer->size);
	int offset = 0; // Desplazamiento
	memcpy(stream + offset, &tareaTrip->id, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream+offset,&tareaTrip->idPatota, sizeof(uint8_t));

	buffer->stream=stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));


			//CODIGO DE OPERACION 1 = UN TRIUPLANTE
			paquete->codigo_operacion = PEDIRTAREA; // Podemos usar una constante por operación
			paquete->buffer = buffer; // Nuestro buffer de antes.

			// Armamos el stream a enviar
			void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
			offset = 0;

			memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

			send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

			// No nos olvidamos de liberar la memoria que ya no usaremos
			free(a_enviar);
			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);



}

tareaTripulante* deserializar_tarea_tripulante(t_buffer* buffer)
{
	tareaTripulante* tarea=malloc(sizeof(tareaTripulante));

	void* stream= buffer->stream;
	memcpy(&(tarea->idTripulante), stream, sizeof(uint8_t));
	stream+=sizeof(uint8_t);
	memcpy(&(tarea->idPatota), stream, sizeof(uint8_t));
	return tarea;
}

void serializar_tarea(char* tarea, int socket)
{
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)+strlen(tarea)+1;
	uint32_t tamano= strlen(tarea)+1;

	void* stream = malloc(buffer->size);
	int offset = 0; // Desplazamiento
	memcpy(stream+offset,&tamano,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(stream+offset,tarea,strlen(tarea)+1);
	buffer->stream=stream;
	free(tarea);
	t_paquete* paquete = malloc(sizeof(t_paquete));


				//CODIGO DE OPERACION 1 = UN TRIUPLANTE
	paquete->codigo_operacion = ENVIOTAREA; // Podemos usar una constante por operación
	paquete->buffer = buffer; // Nuestro buffer de antes.

				// Armamos el stream a enviar
	void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
	offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

	send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

				// No nos olvidamos de liberar la memoria que ya no usaremos
	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
				free(paquete);

}

char* deserializar_tarea(t_buffer* buffer)
{
	uint32_t* tamano=malloc(sizeof(uint32_t));
	void* stream= buffer->stream;

	memcpy(&(tamano),stream,sizeof(uint32_t));
	stream+=sizeof(uint32_t);
	char* tarea=malloc((int)tamano);
	memcpy(tarea,stream,(int)tamano);
	return tarea;

}

void serializar_id_and_pos(Tripulante* pos, int socket)
{
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint8_t) * 3 ;// Id, posx, posy
	void* stream = malloc(buffer->size);
	int offset = 0; // Desplazamiento
	memcpy(stream+offset,&pos->id,sizeof(uint8_t));
	offset+=sizeof(uint8_t);
	memcpy(stream+offset,&pos->posicionX,sizeof(uint8_t));
	offset+=sizeof(uint8_t);
	memcpy(stream+offset,&pos->posicionY,sizeof(uint8_t));
	buffer->stream=stream;

	t_paquete* paquete = malloc(sizeof(t_paquete));


	//CODIGO DE OPERACION 1 = UN TRIUPLANTE
		paquete->codigo_operacion = ACTUALIZAR_POS; // Podemos usar una constante por operación
		paquete->buffer = buffer; // Nuestro buffer de antes.

				// Armamos el stream a enviar
	    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
				offset = 0;

	   memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

	   send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

				// No nos olvidamos de liberar la memoria que ya no usaremos
				free(a_enviar);
				free(paquete->buffer->stream);
				free(paquete->buffer);
				free(paquete);

}

id_and_pos* deserializar_id_and_pos(t_buffer* buffer)
{
	id_and_pos* id_and_pos = malloc(sizeof(id_and_pos));
	void*stream=buffer->stream;
	//Deserealizamos campo por campo mviendonos en el buffer

	memcpy(&(id_and_pos -> idTripulante),stream,sizeof(uint8_t));
	stream+=sizeof(uint8_t);
	memcpy(&(id_and_pos->posX),stream,sizeof(uint8_t));
	stream+=sizeof(uint8_t);
	memcpy(&(id_and_pos->posY),stream,sizeof(uint8_t));

	return id_and_pos;
}


void serializar_sabotaje(char* sabotaje, int socket)
{
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)+strlen(sabotaje)+1;
	uint32_t tamano= strlen(sabotaje)+1;

	void* stream = malloc(buffer->size);
	int offset = 0; // Desplazamiento
	memcpy(stream+offset,&tamano,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(stream+offset,sabotaje,strlen(sabotaje)+1);
	buffer->stream=stream;
	free(sabotaje);
	t_paquete* paquete = malloc(sizeof(t_paquete));


				//CODIGO DE OPERACION 1 = UN TRIUPLANTE
	paquete->codigo_operacion = SABOTAJE; // Podemos usar una constante por operación
	paquete->buffer = buffer; // Nuestro buffer de antes.

				// Armamos el stream a enviar
	void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
	offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));

	send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

				// No nos olvidamos de liberar la memoria que ya no usaremos
	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
				free(paquete);

}

char* deserializar_sabotaje(t_buffer* buffer)
{
	uint32_t* tamano=malloc(sizeof(uint32_t));
	void* stream= buffer->stream;

	memcpy(&(tamano),stream,sizeof(uint32_t));
	stream+=sizeof(uint32_t);
	char* sabotaje=malloc((int)tamano);
	memcpy(sabotaje,stream,(int)tamano);
	return sabotaje;

}
/*

void* serializar_generar_recurso(Tripulante* trip, int socket)
{

}

void* serializar_consumir_recurso(Tripulante* trip, int socket)
{

}
void* actualizar_bitacora(Tripulante*trip, int socket)
{

}
*/


/*
inicio_patota deserializar_inicio_patota(t_buffer* buffer){

}*/
// FUNCION PARA EL MAIN PARA RECIBIR UN PAQUETE adaptarla cuando se necesite!!!
/*
 * t_paquete* paquete = malloc(sizeof(t_paquete));
paquete->buffer = malloc(sizeof(t_buffer));

// Primero recibimos el codigo de operacion
recv(unSocket, &(paquete->codigo_operacion), sizeof(uint8_t), 0);

// Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
recv(unSocket, &(paquete->buffer->size), sizeof(uint32_t), 0);
paquete->buffer->stream = malloc(paquete->buffer->size);
recv(unSocket, paquete->buffer->stream, paquete->buffer->size, 0);

// Ahora en función del código recibido procedemos a deserializar el resto
switch(paquete->codigo_operacion) {
    case PERSONA:
        t_persona* persona = deserializar_persona(paquete->buffer);
        ...
        // Hacemos lo que necesitemos con esta info
        // Y eventualmente liberamos memoria
        free(persona);
        ...
        break;
    ... // Evaluamos los demás casos según corresponda
}

// Liberamos memoria
free(paquete->buffer->stream);
free(paquete->buffer);
free(paquete);
*/
