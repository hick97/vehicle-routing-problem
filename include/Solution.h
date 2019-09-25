#include <vector>
#include "Scenary.h"

using namespace std;

struct Solution
{
  Scenary *s;
  unsigned int fitness;
  vector<unsigned int> route_demands;
  vector<vector<unsigned int>> routes;

  Solution(Scenary *scenary);
  void construction();
  void vnd();
  void print_state();
  void print_real_state();
  bool firstMoviment();
  bool secondMoviment();
  bool thirdMoviment();
  pair<int, int> getClientPositionInRoutes(unsigned int value);
  bool swap_best_block_neighbor(short int block_size);
};