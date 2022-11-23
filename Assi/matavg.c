#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int i, j, row, column, iteration;
    int pid, np, rows_per_process, n_elements_received;
    int index;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    //Verifying the number of arguments
    if (argc != 4)
        {
            printf("Incorrect number of arguments passed. \n");
            exit(0);
        }
    //Converting each arguments to string
    row = atoi(argv[1]);
    column = atoi(argv[2]);
    iteration = atoi(argv[3]);

    //Number of rows each process should perform on
    rows_per_process = row/np;
    float init_matrix[row][column];
    float copy_matrix[row][column];
    int is_first_iteration = 1; 

    //iteration is the total iteration counter for me
    while (iteration>0)
    {
    if (pid==0)
    {   
        iteration = iteration-1;
        if (is_first_iteration==1)
        {   
            //Initializes random matrix only if it is the first iteration
            printf("Initialization \n");
            for (i=0; i<row; i++)
            {
                for (j=0; j<column; j++)
                {
                    init_matrix[i][j] = rand()%10;
                    printf("%f \t",init_matrix[i][j]);
                }
                printf("\n");
            }
            is_first_iteration = 0;
        }
        else
        {   
            //copying temporary matrix to initial matrix after each iteration
            for (i=0;i<row;i++)
            {
                for (j=0;j<column;j++)
                {
                    init_matrix[i][j] = copy_matrix[i][j];
                    copy_matrix[i][j] = 0;
                }
            }
        }
        
        if (np>1)
        {
            for (i=1; i<np; i++)
            {   
                int localcount;
                //Sending First chunk of the matrix to the first process
                if (i==1) 
                {   
                    localcount = (rows_per_process+1)*column;
                    //Keeping track of total number of elements to send
                    MPI_Send(&localcount, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    //Sending matrix
                    MPI_Send(&(init_matrix[0][0]), localcount, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                else{
                    //sending all the chunk except first and last chunk of the matrix
                    localcount = (rows_per_process+2)*column;
                    MPI_Send(&localcount, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    MPI_Send(&(init_matrix[((i-1)*rows_per_process-1)][0]), localcount, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }

            //calculating last chunk of the matrix
            float lastMatrix[rows_per_process+1][column];
            for (i=0;i<(rows_per_process+1);i++)
            {
                for (j=0; j<column;j++)
                {
                    lastMatrix[i][j] = init_matrix[(rows_per_process*(np-1))+i-1][j];
                }
            }

            //evaluating average matrix for the last chunk from the master process
            float lastAvg[rows_per_process][column];
            int m,n;
            for (m=0; m<rows_per_process; m++)
            {
                for (n=0; n<column; n++)
                {
                    float sum = 0;
                    int count = 0;
                    float avg;
                    if ((n-1)>=0)
                    {
                        sum += lastMatrix[m][n-1];
                        count +=1;
                    }
                    if ((n+1)<column)
                    {
                        sum += lastMatrix[m][n+1];
                        count +=1;
                    }
                    if ((m-1)>=0)
                    {
                        sum += lastMatrix[m-1][n];
                        count +=1;
                    }
                    if (m<rows_per_process)
                    {
                        sum += lastMatrix[m+1][n];
                        count +=1;
                    }
                    avg = sum/count;
                    lastAvg[m][n] = avg;
                }
            }

            //Merging average of last chunk to the main average matrix
            for (m=0; m<rows_per_process; m++)
            {
                for (n=0; n<column; n++)
                {
                    //printf("%f \t",lastAvg[m][n]);
                    copy_matrix[row-rows_per_process+m][n]=lastAvg[m][n];
                }
            }

            //Receiving and merging the averaged chunk from all the processed
            for (i=1; i<np; i++)
            {   
                MPI_Recv(&copy_matrix[(i-1)*(rows_per_process)][0],rows_per_process*column, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                //MPI_Recv(&sample_mat[0][0],rows_per_process*column, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            }

            //Printing the final averaged matrix result in the final iteration
            if (iteration==1)
            {
            printf("\n Final Result \n");
            for (m=0; m<row; m++)
            {
                for (n=0; n<column; n++)
                {
                    printf("%f \t",copy_matrix[m][n]);
                }
                printf("\n");
            }
            int sender = status.MPI_SOURCE;
            }
            
        }
    }
    else
    {   
        int m, n, value_count;
        //Receiving the count of message
        MPI_Recv(&value_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        //printf("%d \n", value_count);

        //Receiving message
        float recvMatrix[value_count/column][column];
        //printf("\n Receiver end \n");
        
        MPI_Recv(&(recvMatrix[0][0]), value_count,MPI_INT, 0 , 0, MPI_COMM_WORLD, &status);
        // for (m=0; m<(value_count/column); m++)
        // {
        //     for (n=0; n<column; n++)
        //     {
        //         printf("%f \t",recvMatrix[m][n]);
        //     }
        //     printf("\n");
        // }


        //Calculating average matrix if its a first chunk
        if ((rows_per_process+1)==(value_count/column))
        {
            float firstAvg[rows_per_process][column];
            
            for (m=0; m<(rows_per_process); m++)
            {
                for (n=0; n<column; n++)
                {
                    float sum = 0;
                    int count = 0;
                    float avg;
                    if ((n-1)>=0)
                    {
                        sum += recvMatrix[m][n-1];
                        count +=1;
                    }
                    if ((n+1)<column)
                    {
                        sum += recvMatrix[m][n+1];
                        count +=1;
                    }
                    if ((m-1)>=0)
                    {
                        sum += recvMatrix[m-1][n];
                        count +=1;
                    }
                    if (m<rows_per_process)
                    {
                        sum += recvMatrix[m+1][n];
                        count +=1;
                    }
                    avg = sum/count;
                    firstAvg[m][n] = avg;
                }
            }
            // printf("\n should be first only \n");
            // for (m=0; m<rows_per_process; m++)
            // {
            //     for (n=0; n<column; n++)
            //     {
            //         printf("%f \t",firstAvg[m][n]);
            //     }
            //     printf("\n");
            // }
            // printf("\n");

            //Sending average chunk back to the master process
            MPI_Send(&(firstAvg[0][0]), rows_per_process*column, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        else if ((rows_per_process+2)==(value_count/column))
        {
            //Calculating average chunk for all the processes other than the first chunk
            float firstAvg[rows_per_process][column];
            for (m=1; m<(rows_per_process+1); m++)
            {
                for (n=0; n<column; n++)
                {
                    float sum = 0;
                    int count = 0;
                    float avg;
                    if ((n-1)>=0)
                    {
                        sum += recvMatrix[m][n-1];
                        count +=1;
                    }
                    if ((n+1)<column)
                    {
                        sum += recvMatrix[m][n+1];
                        count +=1;
                    }
                    if ((m-1)>=0)
                    {
                        sum += recvMatrix[m-1][n];
                        count +=1;
                    }
                    if (m<(rows_per_process+1))
                    {
                        sum += recvMatrix[m+1][n];
                        count +=1;
                    }
                    avg = sum/count;
                    firstAvg[m-1][n] = avg;
                }
            }
            // printf("\n Others \n");
            // for (m=0; m<rows_per_process; m++)
            // {
            //     for (n=0; n<column; n++)
            //     {
            //         printf("%f \t",firstAvg[m][n]);
            //     }
            //     printf("\n");
            // }
            // printf("\n");

            //Sending average chunk back to the master process
            MPI_Send(&(firstAvg[0][0]), rows_per_process*column, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    } 
    }
    MPI_Finalize();
    return 0;
}