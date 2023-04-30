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
    int midMatriz, midResultados;

    // Definimos el apuntador de la matriz, de los resultados y de los procesos terminados 
    int *matriz, *listaResultados;

    // Claves
    key_t llaveMatriz; 
    key_t llaveResultados; 

    //Identificador del proceso
    pid_t pid;

    // Generamos las llaves para identificar las regiones de la memoria compartida
    llaveMatriz = ftok("Matriz",'k');
    llaveResultados = ftok("Resultados",'k');

    // Crea el segmento de memorias compartidas
    midMatriz = shmget(llaveMatriz, sizeof(int)*filas*columnas, 0777); 
    midResultados = shmget(llaveResultados, sizeof(int)*filas, 0777);


    // Asignando direcciones de inicios de segmentos de las memorias
    matriz = (int *)shmat(midMatriz, NULL, 0);
    listaResultados= (int *)shmat(midResultados,NULL,0);

    int fila = 0;

    // Fila 1
    int total= 0;
    for(int i=0; i<9; i++){
        total += matriz[i*filas + fila]; // Equivalente a matrix[0][i] 
        printf("%d ", matriz[i*filas + fila]);
    }

    printf("Suma de fila %d = %d :) \n", fila, total);

    listaResultados[fila] = total;


    return 0;
}