#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void asignarValores(int *matriz,int filas, int columnas){
    int i = 0; // Indicador auxiliar para llenar la matriz
    // Fila 1
    int f1= 1;
    for(i=0; i<columnas; i++){
        matriz[i*filas + 0] = f1; // Equivalente a matriz[0][i] 
        f1 += 1;
    }

    // Fila 2
    int f2= 1;
    for(i=0; i<columnas; i++){
        matriz[i*filas + 1] = f2; // Equivalente a matriz[1][i] 
        f2 += 2;
    }

    // Fila 3
    int f3= 2;
    for(i=0; i<columnas; i++){
        matriz[i*filas + 2] = f3; // Equivalente a matriz[2][i] 
        f3 += 2;
    }

    printf("Matriz creada correctamente! :)\n");
}

void leerMatriz(int *matriz, int filas, int columnas){
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            printf("%d ", matriz[j*filas + i]); // Equivalente a matrix[][] 
        }
        printf("\n");
    }
}

void iniciarResultados(int *listaResultados, int filas){
    printf("Resultados inicializados en: ");
    for(int i=0;i<filas;i++){
        listaResultados[i]=0;
        printf(" %d ", listaResultados[i]);
    }
    printf("\n");
}

void mostrarResultados(int *listaResultados, int filas){
    printf("Los resultados son: ");
    for(int i=0;i<filas;i++){
        printf(" %d ", listaResultados[i]);
    }
    printf("\n");
}

int main(){

    // Definimos las dimensiones de la matriz
    int filas = 3, columnas = 9;

    // Identificadores de memoria compartida 
    // (midMatriz = id memoria matriz & midResultados = id memoria resultados & midProcesos == id procesos terminados)
    int midMatriz, midResultados;

    // Definimos el apuntador de la matriz, de los resultados y de los procesos terminados 
    int *matriz, *listaResultados, procesos;

    // Claves
    key_t llaveMatriz; 
    key_t llaveResultados; 

    // Generamos las llaves para identificar las regiones de la memoria compartida
    llaveMatriz = ftok("Matriz",'k');
    llaveResultados = ftok("Resultados",'k');

     // Crea el segmento de memorias compartidas
    midMatriz = shmget(llaveMatriz, sizeof(int)*filas*columnas, IPC_CREAT|0777); 
    midResultados = shmget(llaveResultados, sizeof(int)*filas, IPC_CREAT|0777);

    // Asignando direcciones de inicios de segmentos de las memorias
    matriz = (int *)shmat(midMatriz, NULL, 0);
    listaResultados= (int *)shmat(midResultados,NULL,0);


    // Asignando valor a los procesos terminados
    procesos=0;

    // Asignación de valores a la matriz utilizando una función
    asignarValores(matriz, filas, columnas);

    // Lectura de la matriz
    leerMatriz(matriz, filas, columnas);

    // Iniciamos los valores de los resultados 
    iniciarResultados(listaResultados, filas);


    int procesosTerminados = 0;
    while(procesos == 0){
        printf("Procesos terminados: %d\n", procesosTerminados);
        procesosTerminados = 0;
        for(int i=0; i<filas;i++){
            if(listaResultados[i]!=0){
                procesosTerminados += 1;
            }
        }
        
        if(procesosTerminados==3){
            procesos = 1;
        }
        
        sleep(2);
    }   

    mostrarResultados(listaResultados, filas);

    // Liberar memoria
    shmdt(matriz);
    shmdt(listaResultados);

    // Eliminar segmentos de memoria
    shmctl(midMatriz,IPC_RMID,0);
    shmctl(midResultados,IPC_RMID,0);

    return 0;
}