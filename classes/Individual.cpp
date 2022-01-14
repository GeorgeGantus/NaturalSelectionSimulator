#include "Individual.h"

#include <math.h>
#include <stdlib.h>

#include <iostream>
#include <utility>

#include "../configs/globals.h"
using namespace std;
Individual::Individual(int x, int y) {  //Construtor do individuo
    this->x = x;
    this->y = y;
    home.first = x;
    home.second = y;
    genes.push_back(0.5);
    genes.push_back(1);
    stepsToGo = genes[1] * 10;
    stepsWent = 0;
    foodEated = 0;
}

void Individual::move(int limit, int (*food)[SIZE]) {  //método que movimenta o individuo
    float randm = (float)rand() / RAND_MAX;
    if (randm < 0.25) {  //subir
        if (y < limit) {
            y++;
        }
    } else if (randm < 0.5) {  //direita
        if (x < limit) {
            x++;
        }
    } else if (randm < 0.75) {  //descer
        if (y > 0) {
            y--;
        }
    } else {  //esquerda
        if (x > 0) {
            x--;
        }
    }
    pair<int, int> position;
    position.first = x;
    position.second = y;
    path.insert(position);
    //se encontrar comida --> coma
    if (food[x][y] != 0) {
        food[x][y]--;
        eat();
        // cout << "comeu na posicao (" << x << "," << y << ")" << endl;
    }

    stepsToGo--;
    stepsWent++;
}
pair<int, int> Individual::getPosition() {  //obtem a posição de um individuo
    pair<int, int> position;
    position.first = x;
    position.second = y;
    return position;
}

//Metodo que retorna um individuo a sua posição inicial
void Individual::goHome() {
    x = home.first;
    y = home.second;
    stepsToGo = genes[1] * 10;
    stepsWent = 0;
}

//Define a posição inicial do individuo
void Individual::setHome(int x, int y) {
    home.first = x;
    home.second = y;
}
//metodo que representa a alimentação
void Individual::eat() {
    if (genes[0] / FOOD_SIZE > 1.5) {
        foodEated += (1 - ((genes[0] / FOOD_SIZE) / 10)) * 20;
    } else {
        foodEated += (genes[0] / FOOD_SIZE) * 20;
    }
    //foodEated++;
}