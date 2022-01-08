#include <fstream>
#include <iostream>
#include <vector>

#include "classes/Individual.h"
using namespace std;
#include "configs/globals.h"

void sumFood(int (*food)[SIZE]) {
    int k = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            /* code */
            k += food[i][j];
        }
    }
    cout << "Sobraram:" << k << endl;
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
    float mutation = (((float)rand() / RAND_MAX) * MUTATION_RATE);
    if (randm > 0.5) {
        aux.genes[0] = aux.genes[0] + mutation;
    } else {
        aux.genes[0] = aux.genes[0] - mutation;
    }

    return aux;
}

float meanBeakSize(vector<Individual> population) {
    vector<Individual>::iterator it;
    float sum = 0;
    for (it = population.begin(); it < population.end(); it++) {
        sum += (*it).genes[0];
    }
    cout << "BICO MEDIO: " << sum / population.size() << endl;
    cout << "PROP: " << (sum / population.size()) / FOOD_SIZE << endl;
    return sum / population.size();
}

int main(int argc, char const *argv[]) {
    ofstream graphicData;
    graphicData.open("data.txt");
    graphicData << SIZE << " " << FOOD_SIZE << " " << FOOD_AMOUNT << " " << MUTATION_RATE << " " << GENERATIONS << " " << START_POPULATION << endl;
    srand(10);
    bool extintion = false;
    //start enviroment
    int enviroment[SIZE][SIZE];
    startEnviroment(enviroment);

    //start population
    vector<Individual> population = startPopulation(SIZE);

    for (int k = 0; k < GENERATIONS; k++) {
        //generate food
        generateFood(enviroment);
        sumFood(enviroment);

        /* if (k == 0) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    cout << enviroment[i][j] << " ";
                }
                cout << endl;
            }
        } */

        //***********************************************
        //Move population
        for (int i = 0; i < MOVES_PER_TURN; i++) {
            vector<Individual>::iterator it;
            for (it = population.begin(); it < population.end(); it++) {
                (*it).move(SIZE - 1, enviroment);
            }
        }

        vector<Individual>::iterator it;
        int toRemove = 0;
        int toDuplicate = 0;
        vector<Individual> newIndividuals;
        // cout << "Population SIZE: " << population.size() << endl;
        for (it = population.begin(); it < population.end(); it++) {
            // cout << "comeu: " << (*it).foodEated << endl;
            cout << (*it).foodEated << " ";

            bool erased = false;
            (*it).goHome();
            if ((*it).foodEated >= 58) {
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
                /* code */
            }
        }
        cout << endl;

        if (population.size() == 0) {
            extintion = true;
            break;
        }

        if (newIndividuals.size() != 0) {
            population.insert(population.end(), newIndividuals.begin(), newIndividuals.end());
            /* code */
        }

        cout << "GERACAO " << k << ": pop->" << population.size() << endl;
        graphicData << population.size() << " " << meanBeakSize(population) << endl;
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

    return 0;
}