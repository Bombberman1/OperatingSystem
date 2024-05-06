#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#define ARR_SIZE 10
#define SIZE 1000000
#define PI 3.14159265359


int global_counter = 0;
int global_success = 0;

struct ThreadData {
     float x_min;
     float x_max;

     float y_min;
     float y_max;

     float z_min;
     float z_max;

     float radius;

     int thread_counter;
     int thread_success;
};

struct ThreadData* create_thread_data(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max, float radius) {
     struct ThreadData* temp = (struct ThreadData*)malloc(sizeof(struct ThreadData));
     temp -> x_min = x_min;
     temp -> x_max = x_max;
     temp -> y_min = y_min;
     temp -> y_max = y_max;
     temp -> z_min = z_min;
     temp -> z_max = z_max;
     temp -> radius = radius;
     temp -> thread_counter = 0;
     temp -> thread_success = 0;

     return temp;
}

void* thread_func(void *data) {
     struct ThreadData* temp_data = data;

     int seed = (int)time(NULL);
     while(global_counter < SIZE) {
          float x_rand = temp_data -> x_min + ((float) rand_r(&seed) / RAND_MAX) * (temp_data -> x_max - temp_data -> x_min);
          float y_rand = temp_data -> y_min + ((float) rand_r(&seed) / RAND_MAX) * (temp_data -> y_max - temp_data -> y_min);
          float z_rand = temp_data -> z_min + ((float) rand_r(&seed) / RAND_MAX) * (temp_data -> z_max - temp_data -> z_min);

          float length = sqrt(pow(x_rand, 2) + pow(y_rand, 2) + pow(z_rand, 2));

          if(length < temp_data -> radius) {
               global_success++;
               temp_data -> thread_success++;
          }

          global_counter++;
          temp_data -> thread_counter++;
     }

     return NULL;
}

int main() {
     float radius = 5.0;

     float x_min = 0.0;
     float x_max = radius;

     float y_min = 0.0;
     float y_max = radius;

     float z_min = 0.0;
     float z_max = radius;

     pthread_t threads[ARR_SIZE];

     struct ThreadData* data_arr[ARR_SIZE];
     for(unsigned char i = 0; i < ARR_SIZE; i++) {
          data_arr[i] = create_thread_data(x_min, x_max, y_min, y_max, z_min, z_max, radius);

          pthread_create(&threads[i], NULL, thread_func, (void*)data_arr[i]);
     }

     int threads_counter = 0;
     int threads_success = 0;

     for(unsigned char i = 0; i < ARR_SIZE; i++) {
          pthread_join(threads[i], NULL);
          threads_counter += data_arr[i] -> thread_counter;
          threads_success += data_arr[i] -> thread_success;
     }

     float result_exact = 4.0 / 3.0 * PI * pow(radius, 3);
     float result_monte_global = (x_max - x_min) * (y_max - y_min) * (z_max - z_min) * global_success / global_counter * 8.0;
     float result_monte_threads = (x_max - x_min) * (y_max - y_min) * (z_max - z_min) * threads_success / threads_counter * 8.0;

     printf("Global counter: %d    Global success: %d\n", global_counter, global_success);
     printf("Threads counter: %d    Threads success: %d\n", threads_counter, threads_success);
     printf("Result exact: %f\n", result_exact);
     printf("Result monte global: %f\n", result_monte_global);
     printf("Result monte threads: %f\n", result_monte_threads);

     return 0;
}
