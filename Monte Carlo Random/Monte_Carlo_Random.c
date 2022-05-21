#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

double RANDOM(int *seed)
{
  //Creating new seed for random number generation
  *seed = ((6820 * (*seed)) % 262144);
  srand((*seed));
  return (double) rand() / (double) RAND_MAX;
}

void generateRandom(int N, int *seed)
{
  int i;
  int thread_id;
  int thread_seed;

  #pragma omp parallel private(i, thread_id, thread_seed) shared(N)
  {
    //Generate new seed for each thread
    thread_id = omp_get_thread_num();
    thread_seed = (*seed) + thread_id;

    #pragma omp for
    for (i = 0; i < N; i++)
    {
      printf ( "%6d  %6d  %2d  %.4f\n", thread_id, thread_seed, i, RANDOM(&thread_seed));
    }
  }
}

int main()
{
  int n = 20;
  int seed = 573846753;

  printf ( "Number of processors available = %d\n", omp_get_num_procs());
  printf ( "Number of threads = %d\n", omp_get_max_threads());
  printf ( "\nThread   Seed  Iter   Xi\n" );
  time_t startTime = clock();
  generateRandom(n, &seed);
  double endTime = (clock() - startTime) / (double) CLOCKS_PER_SEC;
  int threads = omp_get_max_threads();
  printf("\nElapsed time (s) = %.4f\n", endTime / (double) threads);
  printf("CPU time (s) = %.4f\n", endTime);
  return 0;
}

