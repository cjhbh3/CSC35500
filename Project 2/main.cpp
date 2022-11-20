// Name: CJ Hess
// Date: 11/1/2022

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sem.h>

#include "AirportAnimator.hpp"

using namespace std;

// Threads for each plane
pthread_t plane[8];

int totalPassengers, totalTours;
int toursDone = 0;

const int runawayKey = 1, passKey = 2;

struct sembuf semGo[1];
struct sembuf semWait[1];

int semIDR, semIDP;

void *planeFunction(void *arg) {
    int sleepTime;
    int *planeNum = (int *) arg;

    while (toursDone < totalTours) {

        // Boarding
        AirportAnimator::updateStatus(*planeNum, "BOARD");

        for (int i = 0; i < 13; i++) {
            // 12 passengers per plane is max
            // However, multiple threads running means some won't get full immediately
            sleepTime = rand() % 3;
            sleep(sleepTime);

            semop(semIDP, semWait, 1);
            AirportAnimator::updatePassengers(*planeNum, i);
        }

        // Taxi 
        AirportAnimator::updateStatus(*planeNum, "TAXI");
        AirportAnimator::taxiOut(*planeNum);

        semop(semIDR, semWait, 1);

        // Takeoff (only one plane should take off at a time)
        AirportAnimator::updateStatus(*planeNum, "TKOFF");
        AirportAnimator::takeoff(*planeNum);


        AirportAnimator::updateStatus(*planeNum, "TOUR");
        semop(semIDR, semGo, 1);
        // Sleep for between 15-45 seconds
        sleepTime = (15+rand()%31);
        sleep(sleepTime);
        
        AirportAnimator::updateStatus(*planeNum,"LNDRQ");
        sleep(2);

        //check to see if runway is empty
        semop(semIDR, semWait, 1);
    
        AirportAnimator::updateStatus(*planeNum,"LAND");
        AirportAnimator::land(*planeNum);

        toursDone++;
        AirportAnimator::updateTours(toursDone);

        AirportAnimator::updateStatus(*planeNum, "TAXI");

        //updates semaphore
        semop(semIDR, semGo,1);
    
        AirportAnimator::taxiIn(*planeNum);

        AirportAnimator::updateStatus(*planeNum, "DEPLN");

        for (int i = 0; i < 13; i++) {
            AirportAnimator::updatePassengers(*planeNum, (12-i));
            sleep(1);
            semop(semIDP, semGo, 1);
        }
    }
}

#define NUM_SEMAPHORES 1

int main(int argc, char* argv[]) {

    // check arg count
    // to compile, include -lncurses
    if (argc != 3) {
        cerr << "Please executing using ./executable passengerNum toursNum" << endl;
        return 1;
    }


    // atoi() to get the nums from arg[]
    totalTours = atoi(argv[2]);
    totalPassengers = atoi(argv[1]);

    semIDP = semget(2, NUM_SEMAPHORES, IPC_CREAT|IPC_EXCL|0666);

    if (semIDP < 0)
        semIDP = semget(2, NUM_SEMAPHORES, 0);
    
    semIDR = semget(1, NUM_SEMAPHORES, IPC_CREAT|IPC_EXCL|0666); // for runway

    if (semIDR < 0)
        semIDR = semget(1,  NUM_SEMAPHORES, 0);

    

    //initalize the semaphore
    semctl(semIDP, 0, SETVAL, totalPassengers);
    
    semctl(semIDR, 0, SETVAL, 1);

    semGo[0].sem_num = 0;
    semGo[0].sem_op = 1;
    semGo[0].sem_flg = 0;

    semWait[0].sem_num = 0;
    semWait[0].sem_op = -1;
    semWait[0].sem_flg = 0;

    AirportAnimator::init();

   // Create our threads and run.
    int p1 = 0,
        p2 = 1,
        p3 = 2,
        p4 = 3,
        p5 = 4,
        p6 = 5,
        p7 = 6,
        p8 = 7;


    
    pthread_create (&plane[0], NULL, planeFunction, &p1);
    pthread_create (&plane[1], NULL, planeFunction, &p2);
    pthread_create (&plane[2], NULL, planeFunction, &p3);
    pthread_create (&plane[3], NULL, planeFunction, &p4);
    pthread_create (&plane[4], NULL, planeFunction, &p5);
    pthread_create (&plane[5], NULL, planeFunction, &p6);
    pthread_create (&plane[6], NULL, planeFunction, &p7);
    pthread_create (&plane[7], NULL, planeFunction, &p8);

    pthread_join(plane[0], NULL);
    pthread_join(plane[1], NULL);
    pthread_join(plane[2], NULL);
    pthread_join(plane[3], NULL);
    pthread_join(plane[4], NULL);
    pthread_join(plane[5], NULL);
    pthread_join(plane[6], NULL);
    pthread_join(plane[7], NULL);
    
    AirportAnimator::end();
    
    return 0;
}