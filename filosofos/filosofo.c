#include "filosofo.h"

filosofo_t* filosofo(int num) {
	filosofo_t *filosofo = malloc(sizeof(filosofo_t));

	filosofo->numero = num;
	filosofo->prioridad = rand() % NUM_FILOSOFOS;
	filosofo->tenedorIzquierdo = -1;
	filosofo->tenedorDerecho = -1;

	return filosofo;
}

void dest_filosofo(filosofo_t *f) {
	free(f);
}

int calcularTiempoEspera(int prioridad) {
	int minimoTiempoEspera = prioridad * DIFF_T_INTERVALOS;
	return rand() % DIFF_T_INTERVALOS + minimoTiempoEspera;
}

int* agarrarTenedores(int tenedorIzquierdo, int tenedorDerecho) {
	int agarroDerecho, agarroIzquierdo;

	if (rand() % 2 == 0) {
		agarroIzquierdo = pthread_mutex_trylock(&tenedores[tenedorIzquierdo]) == 0;
		agarroDerecho = pthread_mutex_trylock(&tenedores[tenedorDerecho]) == 0;
	} else {
		agarroDerecho = pthread_mutex_trylock(&tenedores[tenedorDerecho]) == 0;
		agarroIzquierdo = pthread_mutex_trylock(&tenedores[tenedorIzquierdo]) == 0;
	}

	int *info = malloc(sizeof(int) * 2);
	info[0] = agarroIzquierdo;
	info[1] = agarroDerecho;

	return info;
}

void soltarTenedorDerecho(filosofo_t *f) {
	pthread_mutex_unlock(&tenedores[f->tenedorDerecho]);
	f->tenedorDerecho = -1;
}

void soltarTenedorIzquierdo(filosofo_t *f) {
	pthread_mutex_unlock(&tenedores[f->tenedorIzquierdo]);
	f->tenedorIzquierdo = -1;
}

void* filosofar(void *info) {
	filosofo_t *filosofo = (filosofo_t *) info;

	int tenedorIzquierdo = (filosofo->numero + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS;
	int tenedorDerecho = filosofo->numero % NUM_FILOSOFOS;

	while (1 == 1) {
		int tiempoPensar = (rand() % (MAX_T_PENSAR - MIN_T_PENSAR)) + MIN_T_PENSAR;
		filosofo->estado = PENSANDO;
		actualizarEstado(filosofo);
		usleep(tiempoPensar * 1000);
	
		int *informacion = agarrarTenedores(tenedorIzquierdo, tenedorDerecho);
		int agarroIzquierdo = informacion[0];
		int agarroDerecho = informacion[1];
		free(informacion);

		if (agarroDerecho) filosofo->tenedorDerecho = tenedorDerecho;
		if (agarroIzquierdo) filosofo->tenedorIzquierdo = tenedorIzquierdo;
		actualizarEstado(filosofo);
		usleep(150 * 1000);

		if (!agarroDerecho || !agarroIzquierdo) {
			if (agarroIzquierdo && !agarroDerecho) soltarTenedorIzquierdo(filosofo);
			else if (agarroDerecho && !agarroIzquierdo) soltarTenedorDerecho(filosofo);

			int nuevaPrioridad = filosofo->prioridad - 1;
			filosofo->prioridad = nuevaPrioridad < 0 ? 0 : nuevaPrioridad;

			int tiempoEsperar = calcularTiempoEspera(filosofo->prioridad);
			actualizarEstado(filosofo);
			usleep(tiempoEsperar * 1000);
		} else {
			int tiempoComer = (rand() % (MAX_T_COMER - MIN_T_COMER)) + MIN_T_COMER;
			filosofo->estado = COMIENDO;
			actualizarEstado(filosofo);
			usleep(tiempoComer * 1000);

			if (rand() % 2 == 0) {
				soltarTenedorIzquierdo(filosofo);
				soltarTenedorDerecho(filosofo);
			} else {
				soltarTenedorDerecho(filosofo);
				soltarTenedorIzquierdo(filosofo);
			}

			filosofo->prioridad = NUM_FILOSOFOS - 1;
		}
	}

	return NULL;
}

