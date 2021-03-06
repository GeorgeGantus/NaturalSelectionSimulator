#include <set>
#include <utility>
#include <vector>

#include "../configs/globals.h"
using namespace std;

class Individual {
   private:
    int x;
    int y;
    pair<int, int> home;
    set<pair<int, int>> path;

   public:
    int stepsToGo;
    int stepsWent;
    vector<float> genes;  //beak size, velocity
    int foodEated;
    Individual(int x, int y);
    void move(int limit, int (*food)[SIZE]);
    pair<int, int> getPosition();
    void goHome();
    void setHome(int x, int y);
    void eat();
};