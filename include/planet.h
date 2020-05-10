#ifndef PLANET_H
#define PLANET_H


typedef struct planets {
    int mercury;
    int venus;
    int earth;
    int mars;
    int jupiter;
    int saturn;
    int uranus;
    int neptune;
} Planets;

const Planets defaultX = {1000, 1500, 2000, 2500, 5000, 6000, 7000, 8000};


#endif