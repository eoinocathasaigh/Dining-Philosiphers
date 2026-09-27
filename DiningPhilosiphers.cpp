#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

//Defining various variables to be used in the program
#define N 10
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

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


//Defining a monitor class to handle the synchronization of the philosophers
class monitor
{
    int fork[N];
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
            fork[i] = THINKING;
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
}