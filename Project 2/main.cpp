// Name: CJ Hess
// Date: 11/1/2022

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

#include "AirportAnimator.hpp"

using namespace std;

// Threads for each plane
pthread_t planes[8];

int main(int argc, char* argv[]) {

    // check arg count
    // to compile, include -lncurses
    if (argc != 3) {
        cerr << "Please executing using ./executable passengerNum toursNum" << endl;
        return 1;
    }


    // atoi() to get the nums from arg[]
    int totalTours = atoi(argv[2]);
    int totalPassengers = atoi(argv[1]);

    cout << "Total tours to run: " << totalTours << endl;
    cout << "Total passengers: " << totalPassengers << endl;

    return 0;
}