#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

//Defining various variables to be used in the program
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

int phil[N];
int times = 200;

//The Algorithm for this problem is as follows:
/** 
 * 1. A philosopher can only eat if both of his neighbors are not eating.
 * 2. A philosopher can only pick up a fork if it is available.
 * 3. A philosopher can only put down a fork if he is holding it.

    Setting up various variables to be used in the monitor class
    monitor ForkMonitor:
    integer array[0..4]
    fork ← [2,2,2,2,2]                                                       
    condition array[0..4]OKtoEat

    //Operations relating to the monitor class - actions the philosophers can take
    operation takeForks(integer i)                  
    if(fork[i]!=2)
    waitC(OKtoEat[i])

    fork[i+1]<- fork[i+1]-1
    fork[i-1] <- fork[i-1]-1   

    operation releaseForks(integer i)
    fork[i+1] <- fork[i+1]+1
    fork[i-1] <- fork[i-1]

    if(fork[i+1]==2)
    signalC(OKtoEat[i+1])
            
    if(fork[i-1]==2)
    signalC(OKtoEat[i-1])

    Main Loop of the philosophers - each philosopher will perform the following actions in continuous loop.
    loop forever :
     p1 : think        
     p2 : takeForks(i)                
     p3 : eat                      
     p4 : releaseForks(i)
 */

class monitor
{
    int philosipherState[N];
    //Defining a condition variable to handle the synchronization of the philosophers
    pthread_cond_t OKtoEat[N];
    //Defining a mutex to handle the synchronization of the philosophers - "Locking the monitor so that only one philosopher can access it at a time" - mutual exclusion
    pthread_mutex_t lock;

    //Within the monitor class, we will define the various operations that the philosophers can perform
    public:
    //Constructor
    monitor(){
        //Initializing state of all philosophers to THINKING
        for(int i = 0; i < N; i++){
            philosipherState[i] = THINKING;
        }

        //Initializing the condition variables - all philosophers are initially allowed to eat
        for(int i = 0; i < N; i++){
            pthread_cond_init(&OKtoEat[i], NULL);
        }
        //Initializing the mutex
        pthread_mutex_init(&lock, NULL);

    }
    //Destructor
    ~monitor(){
        //Destroying the condition variables and mutex
        for(int i = 0; i < N; i++){
            pthread_cond_destroy(&OKtoEat[i]);
        }
        pthread_mutex_destroy(&lock);
    }

    void testCondition(int i){
        //If the philosopher is hungry and both of his neighbors are not eating, then he can eat
        if(philosipherState[i] == HUNGRY && philosipherState[left(i)] != EATING && philosipherState[right(i)] != EATING){
            //Change the state of the philosopher to EATING
            philosipherState[i] = EATING;
            //Signal the condition variable to allow the philosopher to eat
            pthread_cond_signal(&OKtoEat[i]);
        }
    }

    int left(int i){
        return (i + N - 1) % N;
    }

    int right(int i){
        return (i + 1) % N;
    }

    void takeForks(int i){
        //1. Lock the monitor
        pthread_mutex_lock(&lock);
        //2. Change the state of the philosopher to HUNGRY
        philosipherState[i] = HUNGRY;
        //3. Test the condition to see if the philosopher can eat
        testCondition(i);
        //4. If the philosopher cannot eat, then wait on the condition variable
        while (philosipherState[i] != EATING) {
            pthread_cond_wait(&OKtoEat[i], &lock);
        }
        cout << "Philosopher " << i + 1 << " takes fork " << left(i) + 1 << " and " << i + 1 << endl;
        //5. Unlock the monitor
        pthread_mutex_unlock(&lock);
    }

    void releaseForks(int i){
        //1. Lock the monitor
        pthread_mutex_lock(&lock);
        //2. Change the state of the philosopher to THINKING
        philosipherState[i] = THINKING;
        cout << "Philosopher " << i + 1 << " puts fork " << left(i) + 1 << " and " << i + 1 << " down" << endl;
        //3. Test the condition of the left and right neighbors to see if they can eat
        testCondition(left(i));
        testCondition(right(i));
        //4. Unlock the monitor
        pthread_mutex_unlock(&lock);
    }
};

//Defining a philosipher global object to be used by the monitor
monitor philosiperObject;

void *philosopher(void *num)
{
    int c = 0;
    while(c < times){
        int i = *(int *)num;
        sleep(1);
        philosiperObject.takeForks(i);
        usleep(500000); // 0.5 seconds
        philosiperObject.releaseForks(i);
        c++;
    }

    //Must always return NULL at the end of a thread function
    return nullptr;
}

//Main function to create the threads for the philosophers and start the simulation
int main(){
    // Declaration...
    pthread_t thread_id[N];
    pthread_attr_t attr;

    // Initialize and set thread detached attribute...
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Creating the threads for the philosophers
    for(int i = 0; i < N; i++){
        phil[i] = i;
    }

    for(int i = 0; i < N; i++){
        pthread_create(&thread_id[i], &attr, philosopher, &phil[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(thread_id[i], NULL);
    }

    // Destroying
    pthread_attr_destroy(&attr);

    return 0;
}