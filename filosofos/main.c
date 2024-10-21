#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include "filosofo.h"

const int NUM_FILOSOFOS = 5;
const int MIN_T_COMER = 1000;
const int MAX_T_COMER = 3000;
const int MIN_T_PENSAR = 1300;
const int MAX_T_PENSAR = 2000;
const int DIFF_T_INTERVALOS = 850;

filosofo_t **filosofos;
pthread_mutex_t *tenedores;
pthread_mutex_t screen_mutex;

void actualizarEstado(filosofo_t *info) {
	pthread_mutex_lock(&screen_mutex);

	if (info->estado == COMIENDO) mvaddwstr(info->numero, 13, L"󱅠");
	else mvaddwstr(info->numero, 13, L"🤔");

	char prioridad[8];
	sprintf(prioridad, "%d", info->prioridad);
	mvaddstr(info->numero, 29, prioridad);

	if (info->tenedorDerecho != -1)
		mvaddch(info->numero, 21, info->tenedorDerecho + 48);
	else
		mvaddch(info->numero, 21, ' ');

	if (info->tenedorIzquierdo != -1)
		mvaddch(info->numero, 19, info->tenedorIzquierdo + 48);
	else
		mvaddch(info->numero, 19, ' ');

	refresh();
	pthread_mutex_unlock(&screen_mutex);
}

void mostrarFilosofos() {
	pthread_mutex_lock(&screen_mutex);

	char filosofoPalabra[35];
	filosofo_t *f;
	for (int i = 0; i < NUM_FILOSOFOS; i++) {
		f = filosofos[i];
		sprintf(filosofoPalabra, "Filosofo %d: (  ), ( , ), PL:  ", i);
		int palabraLen = strlen(filosofoPalabra);

		mvaddstr(i, 0, filosofoPalabra);
		mvaddwstr(i, 13, L"🤔");
		refresh();
	}

	pthread_mutex_unlock(&screen_mutex);
}

int main() {
	filosofos = alloca(sizeof(filosofo_t *) * NUM_FILOSOFOS);
	tenedores = alloca(sizeof(pthread_mutex_t) * NUM_FILOSOFOS);
	pthread_t hilos_filosofos[NUM_FILOSOFOS];

	srand(time(NULL));

	initscr();
	cbreak();
	noecho();
	clear();
	setlocale(LC_ALL, "");
	curs_set(FALSE);


	pthread_mutex_init(&screen_mutex, NULL);
	for (int i = 0; i < NUM_FILOSOFOS; i++)
		pthread_mutex_init(&tenedores[i], NULL);

	for (int i = 0; i < NUM_FILOSOFOS; i++) {
		filosofos[i] = filosofo(i);
		pthread_create(&hilos_filosofos[i], NULL, filosofar, filosofos[i]);
	}
	mostrarFilosofos();

	for (int i = 0; i < NUM_FILOSOFOS; i++)
		pthread_join(hilos_filosofos[i], NULL);

	for (int i = 0; i < NUM_FILOSOFOS; i++)
		pthread_mutex_destroy(&tenedores[i]);
	pthread_mutex_destroy(&screen_mutex);
	
	for (int i = 0; i < NUM_FILOSOFOS; i++)
		dest_filosofo(filosofos[i]);

	endwin();

	return 0;
}
