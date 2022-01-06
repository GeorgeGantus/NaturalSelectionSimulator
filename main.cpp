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
    return aux;
}

int main(int argc, char const *argv[]) {
    srand(10);
    //start enviroment
    int enviroment[SIZE][SIZE];
    startEnviroment(enviroment);

    //start population
    vector<Individual> population = startPopulation(SIZE);

    for (int k = 0; k < GENERATIONS; k++) {
        //generate food
        generateFood(enviroment);
        sumFood(enviroment);

        if (k == 0) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    cout << enviroment[i][j] << " ";
                }
                cout << endl;
            }
        }

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
            // cout << (*it).foodEated << " ";

            bool erased = false;
            (*it).goHome();
            if ((*it).foodEated >= 2) {
                newIndividuals.push_back(born(*it));
                //toDuplicate++;
            }
            if ((*it).foodEated == 0) {
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
        if (newIndividuals.size() != 0) {
            population.insert(population.end(), newIndividuals.begin(), newIndividuals.end());
            /* code */
        }
        cout << "GERACAO " << k << ": pop->" << population.size() << endl;
        //**************************************
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << enviroment[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}