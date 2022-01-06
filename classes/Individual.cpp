#include "Individual.h"

#include <stdlib.h>

#include <iostream>
#include <utility>

#include "../configs/globals.h"
using namespace std;
Individual::Individual(int x, int y) {
    this->x = x;
    this->y = y;
    home.first = x;
    home.second = y;
    genes.push_back(0.5);
    foodEated = 0;
}

void Individual::move(int limit, int (*food)[SIZE]) {
    float randm = (float)rand() / RAND_MAX;
    if (randm < 0.25) {  //move up
        if (y < limit) {
            y++;
        }
    } else if (randm < 0.5) {  //move right
        if (x < limit) {
            x++;
        }
    } else if (randm < 0.75) {  //move down
        if (y > 0) {
            y--;
        }
    } else {  //move left
        if (x > 0) {
            x--;
        }
    }
    pair<int, int> position;
    position.first = x;
    position.second = y;
    path.insert(position);

    if (food[x][y] != 0) {
        food[x][y]--;
        eat();
        // cout << "comeu na posicao (" << x << "," << y << ")" << endl;
    }
}
pair<int, int> Individual::getPosition() {
    pair<int, int> position;
    position.first = x;
    position.second = y;
    return position;
}

void Individual::goHome() {
    x = home.first;
    y = home.second;
}

void Individual::setHome(int x, int y) {
    home.first = x;
    home.second = y;
}
void Individual::eat() {
    if (genes[0] / FOOD_SIZE > 1.5) {
        foodEated += (1 - ((genes[0] / FOOD_SIZE) / 10)) * 20;
    } else {
        foodEated += (genes[0] / FOOD_SIZE) * 20;
    }
    //foodEated++;
}