#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#include "classes/Individual.h"
using namespace std;
#include "configs/globals.h"
//Metodo que retorna o total de comida disponivel no ambiente
int sumFood(int (*food)[SIZE]) {
    int k = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            /* code */
            k += food[i][j];
        }
    }
    return k;
}

//Metodo que inicializa o ambiente sem comida nenhuma
void startEnviroment(int (*enviroment)[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            enviroment[i][j] = 0;
        }
    }
}
//Metodo que inicializa a população em posições aleatórias
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

//metodo que adiciona comida ao ambiente
void generateFood(int (*enviroment)[SIZE]) {
    for (int i = 0; i < FOOD_AMOUNT; i++) {
        int x_randm = rand() % SIZE;
        int y_randm = rand() % SIZE;
        enviroment[x_randm][y_randm] += 1;
    }
}

//Método que gera um novo individuo através de um individuo pai
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
//Metodo que retorna o tamanho médio do bico na população
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
    return mean;
}

int main(int argc, char const *argv[]) {
    ofstream graphicData;  //arquivos para plotar gráficos e gerar a animação
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
        //Enviar informações da comida para arquivo de dados
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                foodData << enviroment[i][j] << " ";
            }
            foodData << endl;
        }
        //Enviar dados da população para arquivo de dados
        movesData << population.size() << endl;

        for (iterador = population.begin(); iterador < population.end(); iterador++) {
            int first = (*iterador).getPosition().first;
            int second = (*iterador).getPosition().second;
            movesData << first << " " << second << endl;
        }

        //Realizar o movimento dos individuos da população, cada individuo se move em ordem no mapa um numero fixo de vezes
        for (int i = 0; i < MOVES_PER_TURN; i++) {
            vector<Individual>::iterator it;
            for (it = population.begin(); it < population.end(); it++) {
                (*it).move(SIZE - 1, enviroment);
                int first = (*it).getPosition().first;
                int second = (*it).getPosition().second;
                movesData << first << " " << second << endl;
            }
        }
        //dado que indica o dim de um dia enviado para o arquivo de dados
        movesData << "-1 -1 " << k << endl;

        //Identificar os individuos que 'morreram' ou reproduziram
        vector<Individual>::iterator it;
        vector<Individual> newIndividuals;
        for (it = population.begin(); it < population.end(); it++) {
            bool erased = false;
            if ((*it).foodEated >= 70) {
                newIndividuals.push_back(born(*it));
            }
            if ((*it).foodEated < 15) {
                population.erase(it--);
                erased = true;
            }
            if (!erased) {
                (*it).foodEated = 0;
                (*it).goHome();
                /* code */
            }
        }

        //Verificar se a população foi extinta
        if (population.size() == 0) {
            extintion = true;
            break;
        }
        //Adicionar novos individuos a população
        if (newIndividuals.size() != 0) {
            population.insert(population.end(), newIndividuals.begin(), newIndividuals.end());
        }

        //Enviar dados para os arquivos
        vector<float> mean = meanBeakSize(population);
        graphicData << population.size() << " " << sumFood(enviroment) + FOOD_AMOUNT << " " << mean[0] << " " << mean[1] << endl;
    }
    graphicData << extintion;
    if (extintion) {
        cout << "A Populacao foi extinta F" << endl;
        /* code */
    }
    //printar o restante de comida
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