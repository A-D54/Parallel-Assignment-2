

/* 
	Done on: March 14th, 2024
	Parallel Code For BucketSort Using OpenMP
	Note that the mergesort function i used was one i wrote back in cp1
*/




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NBUCKETS 10
#define NELEMENTS 1000

void merge(int A[], int l, int m, int r) {


    int firstl = 1+m-l;
    int secondl = r-m;
    int Left_arr[firstl], Right_arr[secondl];


    for (int i = 0; i < firstl; i++){
        Left_arr[i] = A[l + i];
    }
    
    
    for (int j = 0; j < secondl; j++){
   
        Right_arr[j] = A[m+j+1];
    }
    
    
    int i = 0;
    int j = 0;
    int k = l;
    while (i < firstl && j < secondl) {
    
        if (Left_arr[i] > Right_arr[j]) {

            A[k] = Right_arr[j];
            j++;
            
        }else {
        
            A[k] = Left_arr[i];
            i++;
            
        }
        
        k++;
    }
    

    while (firstl>i) {
        A[k] = Left_arr[i];
        i++;
        k++;
    }

    while (secondl>j) {
        A[k] = Right_arr[j];
        j++;
        k++;
    }
}


//------------------------------------------------------------------------------//



void mergesort(int A[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergesort(A, l, m);
        mergesort(A, m + 1, r);
        merge(A, l, m, r);
    }
    
    
}



//------------------------------------------------------------------------------//




void bucketmerging(int buckets[][NELEMENTS], int A[]) {
    
    int pos = 0;
    
    for (int i = 0; i < NBUCKETS; i++) {
    
        for (int j = 0; j < NELEMENTS; j++) {
        
            if (buckets[i][j] != -100) {
                A[pos++] = buckets[i][j];
            }
        }
    }
}



//------------------------------------------------------------------------------//



void bucketsorting(int A[]) {

    omp_set_num_threads(4);
    int buckets[NBUCKETS][NELEMENTS];
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < NBUCKETS; i++) {
        for (int j = 0; j < NELEMENTS; j++) {
            buckets[i][j] = -100;
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < NELEMENTS; i++) {
        int AI = A[i];
        int bucket_id = AI/(100/NBUCKETS);

        for (int j = 0; j < NELEMENTS; j++) {
            if (buckets[bucket_id][j] == -100) {
                buckets[bucket_id][j] = AI;
                break;
            }
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < NBUCKETS; i++) { 
        if (buckets[i][0] != -100) {
            mergesort(buckets[i], 0, NELEMENTS - 1);
        }
    }
    
    bucketmerging(buckets, A);
}



//------------------------------------------------------------------------------//





int main() {

    int numbers[NELEMENTS];
    clock_t start, end;
    
    srand(time(NULL));
    
    
    //Start Computation time
    start = clock();
    for (int i = 0; i < NELEMENTS; i++) {
        numbers[i] = rand() % 100;
    }
    bucketsorting(numbers);
    end = clock();
    //End Computation time


    double t;
    t = ((double) (end - start))/CLOCKS_PER_SEC;
    
    printf("Time taken: %f seconds\n", t);
    return 0;
}


