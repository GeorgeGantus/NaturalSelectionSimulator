#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#include "classes/Individual.h"
using namespace std;
#include "configs/globals.h"

int sumFood(int (*food)[SIZE]) {
    int k = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            /* code */
            k += food[i][j];
        }
    }
    //cout << "Sobraram:" << k << endl;
    return k;
}

void startEnviroment(int (*enviroment)[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            enviroment[i][j] = 0;
        }
    }
}

vector<Individual> startPopulation(int size) {
    vector<Individual> population;
    for (int i = 0; i < START_POPULATION; i++) {
        int x_randm = rand() % SIZE;
        int y_randm = rand() % SIZE;

        Individual ind(x_randm, y_randm);
        population.push_back(ind);
    }
    return population;
}

void generateFood(int (*enviroment)[SIZE]) {
    for (int i = 0; i < FOOD_AMOUNT; i++) {
        int x_randm = rand() % SIZE;
        int y_randm = rand() % SIZE;
        enviroment[x_randm][y_randm] += 1;
    }
}

Individual born(Individual parent) {
    Individual aux = parent;
    int x = rand() % SIZE;
    int y = rand() % SIZE;
    aux.foodEated = 0;
    aux.setHome(x, y);
    float randm = (float)rand() / RAND_MAX;
    float randm2 = (float)rand() / RAND_MAX;
    float mutation = (((float)rand() / RAND_MAX) * MUTATION_RATE);
    float mutation2 = (((float)rand() / RAND_MAX) * MUTATION_RATE);
    if (randm > 0.5) {
        aux.genes[0] = aux.genes[0] + mutation;
    } else {
        aux.genes[0] = aux.genes[0] - mutation;
    }
    if (randm2 > 0.5) {
        aux.genes[1] = aux.genes[1] + mutation2;
    } else {
        aux.genes[1] = aux.genes[1] - mutation2;
    }

    aux.stepsToGo = aux.genes[1] * 10;
    return aux;
}

vector<float> meanBeakSize(vector<Individual> population) {
    vector<Individual>::iterator it;
    vector<float> mean;
    mean.push_back(0);
    mean.push_back(0);
    for (it = population.begin(); it < population.end(); it++) {
        mean[0] += (*it).genes[0];
        mean[1] += (*it).genes[1];
    }

    mean[0] = mean[0] / population.size();
    mean[1] = mean[1] / population.size();
    //cout << "BICO MEDIO: " << mean[0] << endl;
    //cout << "VELOCIDADE MEDIA: " << mean[1] << endl;
    //cout << "PROP: " << (mean[0]) / FOOD_SIZE << endl;
    return mean;
}

int main(int argc, char const *argv[]) {
    ofstream graphicData;
    ofstream movesData;
    ofstream foodData;
    graphicData.open("data5.txt");
    movesData.open("moves.txt");
    foodData.open("food.txt");
    graphicData << SIZE << " " << FOOD_SIZE << " " << FOOD_AMOUNT << " " << MUTATION_RATE << " " << GENERATIONS << " " << START_POPULATION << endl;
    srand(10);
    bool extintion = false;
    //start enviroment
    int enviroment[SIZE][SIZE];
    startEnviroment(enviroment);

    //start population
    vector<Individual> population = startPopulation(SIZE);
    vector<Individual>::iterator iterador;

    for (int k = 0; k < GENERATIONS; k++) {
        //generate food
        generateFood(enviroment);
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                foodData << enviroment[i][j] << " ";
            }
            foodData << endl;
        }
        movesData << population.size() << endl;

        for (iterador = population.begin(); iterador < population.end(); iterador++) {
            int first = (*iterador).getPosition().first;
            int second = (*iterador).getPosition().second;
            movesData << first << " " << second << endl;
        }

        //***********************************************
        /* vector<Individual>::iterator itAux;
        queue<Individual *> q;
        for (itAux = population.begin(); itAux < population.end(); itAux++) {
            if ((*itAux).stepsToGo > 0) {
                //cout << (*itAux).stepsToGo << " ";
                q.push(&(*itAux));
            }
        }
        while (!q.empty()) {
            q.front()->move(SIZE - 1, enviroment);
            if (q.front()->stepsToGo > 0) {
                q.push(q.front());
            }
            q.pop();
        } */
        //Move population
        for (int i = 0; i < MOVES_PER_TURN; i++) {
            vector<Individual>::iterator it;
            for (it = population.begin(); it < population.end(); it++) {
                (*it).move(SIZE - 1, enviroment);
                int first = (*it).getPosition().first;
                int second = (*it).getPosition().second;
                movesData << first << " " << second << endl;
            }
        }
        movesData << "-1 -1 " << k << endl;

        vector<Individual>::iterator it;
        int toRemove = 0;
        int toDuplicate = 0;
        vector<Individual> newIndividuals;
        // cout << "Population SIZE: " << population.size() << endl;
        for (it = population.begin(); it < population.end(); it++) {
            //cout << "comeu: " << (*it).foodEated << endl;
            //cout << (*it).foodEated << " ";

            bool erased = false;
            if ((*it).foodEated >= 70) {
                newIndividuals.push_back(born(*it));
                //toDuplicate++;
            }
            if ((*it).foodEated < 15) {
                population.erase(it--);
                //toRemove++;
                erased = true;
            }
            if (!erased) {
                (*it).foodEated = 0;
                (*it).goHome();
                /* code */
            }
        }
        //cout << endl;
        //movesData << population.size() << endl;
        if (population.size() == 0) {
            extintion = true;
            break;
        }

        if (newIndividuals.size() != 0) {
            population.insert(population.end(), newIndividuals.begin(), newIndividuals.end());
            /* code */
        }

        //cout << "GERACAO " << k << ": pop->" << population.size() << endl;
        vector<float> mean = meanBeakSize(population);
        graphicData << population.size() << " " << sumFood(enviroment) + FOOD_AMOUNT << " " << mean[0] << " " << mean[1] << endl;
        //**************************************
    }
    graphicData << extintion;
    if (extintion) {
        cout << "A Populacao foi extinta F" << endl;
        /* code */
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << enviroment[i][j] << " ";
        }
        cout << endl;
    }
    graphicData.close();
    movesData.close();
    return 0;
}