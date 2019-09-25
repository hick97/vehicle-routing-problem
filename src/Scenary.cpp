#include "Scenary.h"
#include <stdio.h>

Scenary::Scenary(unsigned int clients, unsigned int vehicles, unsigned int capacity, vector<unsigned int> demands, vector<vector<unsigned int>> distances) : visited_clients(clients), vehicles_number(vehicles), max_capacity(capacity), demands_per_client(demands), distances_between_clients(distances)
{
}