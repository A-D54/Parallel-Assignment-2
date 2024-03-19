

/* 
	Done on: March 14th, 2024
	Parallel Code For BucketSort Using Pthreads
	Note that the mergesort function i used was one i wrote back in cp1
*/




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NBUCKETS 10
#define NTHREADS 4
#define NELEMENTS 1000

typedef struct TArg{
    int left;
    int right;
    int (*hispart)[NELEMENTS];
} thread;

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





void *runner(void * args){
    thread * th = (thread *) args;
    
    for (int i=th->left; i<th->right; i++) {
    
    
        if (th->hispart[i][0] != -100) {
        
            
            mergesort(th->hispart[i], 0, NELEMENTS - 1);
        }
        
        
    }
    pthread_exit(0);
}





//------------------------------------------------------------------------------//




void bucketsorting(int A[]) {
    
    int buckets[NBUCKETS][NELEMENTS];
    pthread_t threads[NTHREADS];
    thread th[NTHREADS];
    
    
    int threadbucket = NBUCKETS/NTHREADS;
    for (int i = 0; i < NBUCKETS; i++) {
        for (int j = 0; j < NELEMENTS; j++) {
        
            buckets[i][j] = -100;
            
        }
    }

   
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

    for(int i = 0;i < NTHREADS; i++){
    
        th[i].hispart = buckets;
        th[i].left = i*threadbucket;
        th[i].right = (i+1)*threadbucket;

        if (i == NTHREADS - 1) {
            th[i].right = NBUCKETS;
        }
        
        pthread_create(&threads[i], NULL, runner,(void*) &th[i]);
    }
    
    
     for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
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


