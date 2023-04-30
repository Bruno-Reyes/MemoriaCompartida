#include <stdio.h>
#include <stdlib.h>

int main(){
    
    // Identificadores de memoria compartida
    int shmid1, shmid2, i;

    //Estructuras donde se almacenan los vectores y el turno 
    int *arregloA, *turno;

    // Claves 
    key_t llave1, llave2;

    //Identificador del proceso
    pid_t pid;

    // Generamos llaves para cada fila de la matriz
    llave1=ftok("FilaA",'k');
    llave2=ftok("FilaB",'k');
    llave2=ftok("FilaC",'k');
    llave2=ftok("turno",'k');

    //Creando areas de memoria compartida 
    shmid1=shmget(llave1,10*sizeof(int),IPC_CREAT|0777); // Area de 10 enteros
    shmid2=shmget(llave2,sizeof(int),IPC_CREAT|0777);


    arreglo=(int *)shmat(shmid1,0,0);
    turno=(int *)shmat(shmid2,0,0);
    *turno=0;
    for(i=0;i<10;i++)
    {
        arreglo[i]=i+1;
        printf("\nProceso escritor dice: %d", arreglo[i]);
        while(*turno!=1);
        *turno=0;
        sleep(1);
    }

    printf("\n\nEl escritor ha terminado\n\n");
    shmdt(&arreglo);
    shmdt(&turno);
    shmctl(shmid1,IPC_RMID,0);
    shmctl(shmid2,IPC_RMID,0);
    return 0;
}