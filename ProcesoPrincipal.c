#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>

void assignMatrixValues(int *matrix, int rows, int columns){
    int f1 = 1, f2 = 2, f3 = 3;

    for(int i=0; i<columns; ++i){
        matrix[i*rows + 0] = f1;
        f1 += 1;

        matrix[i*rows + 1] = f2;
        f2 += 2;

        matrix[i*rows + 2] = f3;
        f3 += 2;
    }

    printf("Successfully created matrix! :)\n");
}

void assignResultsValues(int *resultsList, int rows){
    for(int i = 0; i < rows; ++i)
        resultsList[i] = 0;
    printf("Successfully created results list! :D\n\n");
}

void completedProcesses(const int *resultsList, int rows) {
    int processes = 0;

    while(1){
        for(int i = 0; i < rows; ++i)
            if(resultsList[i] != 0)
                processes += 1;

        printf("Completed processes: %d\n", processes);
        sleep(1);

        if (processes == rows) break;

        processes = 0;
    }
    printf("\n");
}

void printMatrix(int *matrix, int rows, int columns){
    printf("The matrix is: \n");
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            printf("%d ", matrix[j*rows + i]);
        }
        printf("\n");
    }
    printf("\n");
}

void printResults(int *resultsList, int rows){
    printf("The list of results is: \n");
    for(int i = 0; i < rows; ++i){
        printf("%d ", resultsList[i]);
    }
    printf("\n\n");
}

int main(){
    int rows = 3, columns = 9;

    // Matrix Memory
    key_t keyMatrix = ftok(".",'J');
    int shmIdMatrix = shmget(keyMatrix, sizeof(int)*rows*columns, 0777|IPC_CREAT);
    int (*matrix)[columns] = shmat(shmIdMatrix, NULL, 0);

    // Results Memory
    key_t keyResults = ftok(".",'K');
    int shmIdResults = shmget(keyResults, sizeof(int)*rows, 0777|IPC_CREAT);
    int *resultsList= (int *)shmat(shmIdResults,NULL,0);

    assignMatrixValues((int *) matrix, rows, columns);
    assignResultsValues(resultsList, rows);

    printMatrix((int *) matrix, rows, columns);
    printResults(resultsList, rows);

    completedProcesses(resultsList, rows);

    printResults(resultsList, rows);

    // Free Memory
    shmdt(matrix);
    shmdt(resultsList);

    // Delete memory segments
    shmctl(shmIdMatrix,IPC_RMID,NULL);
    shmctl(shmIdResults,IPC_RMID,NULL);

    return 0;
}