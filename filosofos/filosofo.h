#ifndef FILOSOFO_INCLUDED
#define FILOSOFO_INCLUDED

#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

enum ESTADO {
	PENSANDO,
	COMIENDO
};

typedef struct {
	enum ESTADO estado;
	int prioridad;
	int numero;
	int tenedorIzquierdo;
	int tenedorDerecho;
} filosofo_t;

extern pthread_mutex_t *tenedores;
extern void actualizarEstado(filosofo_t *);
const extern int DIFF_T_INTERVALOS, NUM_FILOSOFOS, MAX_T_COMER, MIN_T_COMER, MAX_T_PENSAR, MIN_T_PENSAR;

void* filosofar(void *);
void dest_filosofo(filosofo_t *);
filosofo_t* filosofo(int);

#endif
