#include <vector>

using namespace std;

struct Scenary
{
  unsigned int visited_clients;
  unsigned int vehicles_number;
  unsigned int max_capacity;

  vector<unsigned int> demands_per_client;
  vector<vector<unsigned int>> distances_between_clients;

  Scenary(unsigned int clients, unsigned int vehicles, unsigned int capacity, vector<unsigned int> demands, vector<vector<unsigned int>> distances);
};