#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    // Definimos las dimensiones de la matriz
    int filas = 3, columnas = 9;

    // Identificadores de memoria compartida (midMatriz = id memoria matriz & midResultados = id memoria resultados & midProcesos == id procesos terminados)
    int midMatriz, midResultados, midProcesos;

    // Definimos el apuntador de la matriz, de los resultados y de los procesos terminados 
    int *matriz, *listaResultados, *procesosTerminados;

    // Claves
    key_t llaveMatriz; 
    key_t llaveResultados; 
    key_t llaveProcesos;

    //Identificador del proceso
    pid_t pid;

    // Generamos las llaves para identificar las regiones de la memoria compartida
    llaveMatriz = ftok("Matriz",'k');
    llaveResultados = ftok("Resultados",'k');
    llaveProcesos = ftok("Procesos",'k');

    // Crea el segmento de memorias compartidas
    midMatriz = shmget(llaveMatriz, sizeof(int)*filas*columnas, IPC_CREAT|0777); 
    midResultados = shmget(llaveResultados, sizeof(int)*filas, IPC_CREAT|0777);
    midProcesos = shmget(llaveProcesos, sizeof(int), IPC_CREAT|0777);

    // Asignando direcciones de inicios de segmentos de las memorias
    matriz = (int *)shmat(midMatriz, 0, 0);
    listaResultados= (int *)shmat(midResultados,0,0);
    procesosTerminados= (int *)shmat(midProcesos,0,0);

    int fila = 2;
    // Fila 3
    int total= 0;
    for(int i=0; i<9; i++){
        total += matriz[i*filas + fila]; // Equivalente a matrix[0][i] 
        printf("%d ", matriz[i*filas + fila]);
    }

    printf("Suma de fila %d = %d :) \n", fila, total);
    listaResultados[fila] = total;

    // Para comprobar la sincronia
    sleep(1);

    // Aumentamos en 1 el valor de los procesos terminados
    *procesosTerminados = *procesosTerminados + 1;


    return 0;
}