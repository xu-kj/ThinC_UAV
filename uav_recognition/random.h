#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include <ctime>

struct Random {
    Random() {
        time_t seconds;
        time(&seconds);
        srand((unsigned int) seconds);
    }

    int operator () (int num) {
        return rand() % (num + 1);
    }

    int operator () (int num1, int num2) {
        return rand() % (num2 - num1 + 1) + num1;
    }
};

static Random rnd;

#endif /* RANDOM_H */