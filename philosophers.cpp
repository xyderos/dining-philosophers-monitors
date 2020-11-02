#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#define HUNGER 100
#define nrPhil 500

int tHunger;

struct Chopstick{

    pthread_mutex_t lock;

};

void thinking(){

    long time = (rand()&2)/4;

    sleep(time);
}

class monitor{

    public:
        void lFork(long id){

            if(id != nrPhil) pthread_mutex_lock(&forks[id-1].lock);

            else pthread_mutex_lock(&forks[id].lock);
        }

        void rFork(long id){

            if(id != nrPhil) pthread_mutex_lock(&forks[id].lock);

            else pthread_mutex_lock(&forks[id-1].lock);

    }

    void release(long id){

            pthread_mutex_unlock(&forks[id].lock);

            pthread_mutex_unlock(&forks[id-1].lock);
    }

private:

    Chopstick forks[nrPhil + 1];

}table;

void *philosopher(void *arg){

    int hunger = tHunger;

    while(hunger > 0){

        thinking();

        table.lFork((long) arg);

        table.rFork((long) arg);

        hunger --;

        printf("HUNGER %d @  %ld\n",hunger,(long)arg);

        table.release((long) arg);
    }
    return nullptr;
}

int main(int argc, char * argv[]){

    tHunger = (argc > 1) ? atoi(argv[1]) : HUNGER;

    pthread_t philosophers[nrPhil+1];

    for(long i = 1; i <= nrPhil; i++) pthread_create(&philosophers[i], nullptr, philosopher, (void *)i);

    printf("threads created\n");

    for(int i = 1; i <= nrPhil; i++) pthread_join(philosophers[i], nullptr);

    exit(0);
}