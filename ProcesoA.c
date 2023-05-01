#include <stdio.h>
#include <sys/shm.h>

int rowSum(const int *matrix, int rows, int columns, int row){
    int total = 0;

    for (int i = 0; i < columns; ++i)
        total += matrix[i*rows + row];

    printf("Sum of row %d = %d\n\n", row, total);

    return total;
}

int main(){
    int rows = 3, columns = 9, row = 0;

    // Matrix Memory
    key_t keyMatrix = ftok(".",'J');
    int shmIdMatrix = shmget(keyMatrix, sizeof(int)*rows*columns, 0777|IPC_CREAT);
    int (*matrix)[columns] = shmat(shmIdMatrix, NULL, 0);

    // Results Memory
    key_t keyResults = ftok(".",'K');
    int shmIdResults = shmget(keyResults, sizeof(int)*rows, 0777|IPC_CREAT);
    int *resultsList= (int *)shmat(shmIdResults,NULL,0);

    resultsList[row] = rowSum((int *) matrix, rows, columns, row);

    return 0;
}