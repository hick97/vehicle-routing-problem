#include "Solution.h"
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

Solution::Solution(Scenary *scenary) : fitness(0), s(scenary), route_demands(scenary->vehicles_number, 0), routes(scenary->vehicles_number)
{
}

void Solution::construction()
{

  //Starting candidates list
  vector<unsigned int> candidates;
  for (auto i = 1; i < this->s->demands_per_client.size(); i++)
  {
    candidates.push_back(i);
  }

  //Setting distribution center in each route
  for (int i = 0; i < this->routes.size(); i++)
  {
    for (int j = 0; j < 2; j++)
    {
      this->routes[i].push_back(0);
    }
  }

  while (candidates.size() > 0)
  {
    //Choosing a random candidate to push in routes
    srand(time(NULL));
    int random_index = rand() % (candidates.size());

    // Balance after "insertion"
    unsigned int balance;

    // Cadidate analyzed
    unsigned int current_candidate = candidates[random_index];
    unsigned int current_demand;

    // Position in which the candidate will be inserted
    int final_candidate_route = -1;
    int final_candidate_route_position = -1;

    //setting initial cost to infinity
    unsigned int better_cost = std::numeric_limits<unsigned int>::max();

    //Last route with capacity
    auto free_route = -1;

    //Walking in routes
    for (int r = 0; r < this->routes.size(); r++)
    {

      //Walking between route nodes
      for (int i = 1; i < this->routes[r].size(); i++)
      {

        // Checking if route have capacity
        if (this->route_demands[r] >= this->s->max_capacity)
        {
          //printf("Dont have capacity in currrent route\n");
          break;
        }

        // Setting route as last route with capacity
        free_route = r;

        current_demand = this->s->demands_per_client[current_candidate];

        unsigned int current_value = this->routes[r][i];
        unsigned int previous_value = this->routes[r][i - 1];

        //Checking what change ocurred with node "insertion"
        balance =
            this->fitness +
            (this->s->distances_between_clients[previous_value][current_candidate] +
             this->s->distances_between_clients[current_candidate][current_value]) -
            (this->s->distances_between_clients[current_value][previous_value]);

        //Checking if have capacity with node insertition and had
        if (this->route_demands[r] + current_demand <= this->s->max_capacity && (balance < better_cost))
        {
          // Updating better cost
          better_cost = balance;

          // Updating candidate position to be inserted
          final_candidate_route = r;
          final_candidate_route_position = i;
        }
      }
    }
    if (final_candidate_route != -1 && final_candidate_route_position != -1)
    {
      // Real insertition
      auto itPos = this->routes[final_candidate_route].begin() + final_candidate_route_position;
      this->routes[final_candidate_route].insert(itPos, current_candidate);
      this->route_demands[final_candidate_route] += current_demand;

      //Updating total cost
      this->fitness = better_cost;
    }
    else
    {
      //If there is no improvement and have space in last free route:
      if ((this->route_demands[free_route] += this->s->demands_per_client[current_candidate]) <= this->s->max_capacity)
      {
        //Updating total cost
        this->fitness = this->fitness +
                        (this->s->distances_between_clients[current_candidate][0] +
                         this->s->distances_between_clients[current_candidate][this->routes[free_route][this->routes[free_route].size() - 2]]);

        // Inserting candidate in last free route
        auto itPos = this->routes[free_route].end() - 1;
        this->routes[free_route].insert(itPos, current_candidate);

        this->route_demands[free_route] += this->s->demands_per_client[current_candidate];
      }
      else
      {
        // Node will still be in cadidate list
        break;
      }
    }

    //Removing cadidate from list
    candidates.erase(candidates.begin() + random_index);
  }

  printf("Final cost: %d\n", this->fitness);

  // Printando rotas:
  printf("Final routes: \n");

  for (int i = 0; i < this->routes.size(); i++)
  {
    for (int j = 0; j < this->routes[i].size(); j++)
    {
      cout << this->routes[i][j] << " ";
    }
    cout << "\n";
  }

  // Printando rotas:
  /*
  printf("Final demands: %u\n", this->s->max_capacity);
  auto sum = 0;

  for (int i = 0; i < this->routes.size(); i++)
  {
    for (int j = 0; j < this->routes[i].size(); j++)
    {
      sum += this->s->demands_per_client[this->routes[i][j]];
      cout << this->s->demands_per_client[this->routes[i][j]] << " ";
    }
    cout << "Demanda da rota:" << sum << "\n";
    sum = 0;
  }

  cout << "\n";
  */
}
void Solution::vnd()
{
  //third moviment
  while (true)
  {

    //second moviment
    while (true)
    {

      //first moviment
      while (true)
      {

        //TODO: call moviment
        bool improved = firstMoviment();

        if (!improved)
          break;

        //TODO: comparing moviment result with global result
        //TODO: if improved, global = moviment result, if dont improved break;
      }

      //TODO: call moviment
      bool improved = secondMoviment();

      if (!improved)
          break;

      //TODO: comparing moviment result with global result
      //TODO: if improved, global = moviment result, if dont improved break;
    }
    //TODO: call moviment

    bool improved = thirdMoviment();

    if (!improved)
        break;
    //TODO: comparing moviment result with global result
    //TODO: if improved, global = moviment result, if dont improved break;
  }
}

bool Solution::firstMoviment()
{
  bool improved = false;

  //Starting candidates list with all clients
  vector<unsigned int> candidates;
  for (auto i = 1; i < this->s->demands_per_client.size(); i++)
  {
    candidates.push_back(i);
  }

  while (candidates.size() > 0)
  {
    //Choosing a random candidate to push in routes
    srand(time(NULL));
    int random_index = rand() % (candidates.size());

    // Balance after "insertion"
    unsigned int balance;

    // Cadidate analyzed
    unsigned int current_candidate = candidates[random_index];
    unsigned int current_demand = this->s->demands_per_client[current_candidate];
    std::pair<int, int> candidate_positions;
    candidate_positions = this->getClientPositionInRoutes(current_candidate);

    // Position in which the candidate will be inserted
    int final_candidate_route = -1;
    int final_candidate_route_position = -1;

    //setting initial cost
    unsigned int better_cost = this->fitness;

    //Walking in routes
    for (int r = 0; r < this->routes.size(); r++)
    {

      //Walking between route nodes
      for (int i = 1; i < this->routes[r].size(); i++)
      {

        // Checking if route have capacity
        if (this->route_demands[r] >= this->s->max_capacity)
        {
          //printf("Dont have capacity in currrent route \n");
          break;
        }
        // Checking analyzed position
        if (this->routes[r][i] == current_candidate || this->routes[r][i - 1] == current_candidate)
        {
          //printf("analyzed positions is current position of candidate\n");
          continue;
        }

        unsigned int current_value = this->routes[r][i];
        unsigned int previous_value = this->routes[r][i - 1];

        //Checking what change ocurred with node "insertion"
        balance =
            this->fitness +
            (this->s->distances_between_clients[previous_value][current_candidate] +
             this->s->distances_between_clients[current_candidate][current_value]) -
            (this->s->distances_between_clients[current_value][previous_value]);

        //Removing cost of old candidate position
        unsigned int next_value = this->routes[candidate_positions.first][candidate_positions.second + 1];
        previous_value = routes[candidate_positions.first][candidate_positions.second - 1];

        balance -=
            (this->s->distances_between_clients[current_candidate][next_value] +
             this->s->distances_between_clients[current_candidate][previous_value]);
        balance += this->s->distances_between_clients[next_value][previous_value];

        //printf(" balance após remover o custo do candidato: %u\n", balance);
        //Checking if have capacity with node insertition and had performance
        if (this->route_demands[r] + current_demand <= this->s->max_capacity && (balance < better_cost))
        {
          //printf("Tive um custo melhor pq o balance foi:%u \n", balance);

          // Updating better cost
          better_cost = balance;

          // Updating candidate position to be inserted
          final_candidate_route = r;
          final_candidate_route_position = i;
        }
      }
    }
    if (final_candidate_route != -1 && final_candidate_route_position != -1)
    {
      improved = true;

      // Real insertition
      auto itPos = this->routes[final_candidate_route].begin() + final_candidate_route_position;
      this->routes[final_candidate_route].insert(itPos, current_candidate);
      this->route_demands[final_candidate_route] += current_demand;
      this->route_demands[candidate_positions.first] -= current_demand;

      //Updating total cost
      this->fitness = better_cost;

      //Cheking if new position is smaller than old position
      int route_to_remove = candidate_positions.first;
      int pos_to_remove = candidate_positions.second;

      if (final_candidate_route == candidate_positions.first &&
          final_candidate_route_position < candidate_positions.second)
      {
        pos_to_remove += 1;
      }
      //Removing cadidate from old position
      this->routes[route_to_remove].erase(this->routes[route_to_remove].begin() + pos_to_remove);
    }
    //Removing cadidate from list
    candidates.erase(candidates.begin() + random_index);
  }

  printf("Final cost after moviment: %d\n", this->fitness);

  // Printando rotas:
  printf("Final routes after moviment: \n");

  for (int i = 0; i < this->routes.size(); i++)
  {
    for (int j = 0; j < this->routes[i].size(); j++)
    {
      cout << this->routes[i][j] << " ";
    }
    cout << "\n";
  }

  cout << "\n";

  return improved;

  // Printando rotas:
  /*
  printf("Final demands after moviment: %u\n", this->s->max_capacity);
  auto sum = 0;

  for (int i = 0; i < this->routes.size(); i++)
  {
    for (int j = 0; j < this->routes[i].size(); j++)
    {
      sum += this->s->demands_per_client[this->routes[i][j]];
      cout << this->s->demands_per_client[this->routes[i][j]] << " ";
    }
    cout << "Demanda da rota:" << sum << "\n";
    sum = 0;
  }

  cout << "\n";
  */
}
pair<int, int> Solution::getClientPositionInRoutes(unsigned int value)
{
  std::pair<int, int> positions;
  for (auto i = 0; i < this->routes.size(); i++)
  {
    for (auto j = 0; j < this->routes[i].size(); j++)
    {
      if (routes[i][j] == value)
      {
        positions.first = i;
        positions.second = j;

        return positions;
      }
    }
  }
}

bool Solution::secondMoviment() {
  return swap_best_block_neighbor(0);
}

bool Solution::thirdMoviment() {
  return swap_best_block_neighbor(4);
}

void print_block(const deque<unsigned int> &block){
    for (auto value : block) {
        cout << value << " ";
    }
}

bool Solution::swap_best_block_neighbor(short int block_size){
    // Save the best total cost
    unsigned int best_total_cost = std::numeric_limits<unsigned int>::max();
    unsigned int final_from_route, final_to_route, start_index_from, start_index_to, 
                  final_from_capacity, final_to_capacity; 
    
    deque<unsigned int> from_block, to_block;
    unsigned int current_cost;

    for (unsigned int from_route = 0; from_route < this->routes.size(); from_route++) {
        cout << "NEW ROUTE\n";

        // Make the first block in this route
        unsigned int from_previous_client = 0;
        auto from_client = next(this->routes[from_route].begin(), 1);
        unsigned int begin_from_block_index = 0, capacity_from_block = 0;
        while (from_client != this->routes[from_route].end() && from_block.size() <= block_size) {
            from_block.push_back(*from_client);
            capacity_from_block += this->s->demands_per_client[*from_client];
            from_client++;
        }
        unsigned int from_next_client = *from_client;

        // Now lets compare the cost of swaping this block with blocks from other routes
        while (from_client != this->routes[from_route].end()) {
            cout << "First block = "; print_block(from_block); cout << "\n";
            cout << "Capacity is " << capacity_from_block << "\n";

            for (unsigned int to_route = 0; to_route < this->routes.size(); to_route++) {
                if (from_route == to_route){
                    // Trying swap block between the same routes
                    continue;
                }

                // Make the first block with this route intended to swap with the other block previously made
                unsigned int to_previous_client = 0;
                auto to_client = next(this->routes[to_route].begin(), 1);
                unsigned int begin_to_block_index = 0, capacity_to_block = 0;
                while (to_client != this->routes[to_route].end() && to_block.size() <= block_size) {
                    to_block.push_back(*to_client);
                    capacity_to_block += this->s->demands_per_client[*to_client];
                    //to_previous_client = *to_client;
                    to_client++;
                }
                unsigned int to_next_client = *from_client;

                while (to_client != this->routes[to_route].end()) {
                    cout << "\tSecond block = "; print_block(to_block); cout << "\n";
                    cout << "\tCapacity is " << capacity_to_block << "\n";
                    
                    // Calculate the route cost after swaping these two blocks
                    current_cost = this->fitness 
                                    - this->s->distances_between_clients[from_previous_client][from_block.front()]  
                                    + this->s->distances_between_clients[from_previous_client][to_block.front()]
                                    - this->s->distances_between_clients[from_block.back()][from_next_client]
                                    + this->s->distances_between_clients[from_block.back()][to_next_client]
                                    - this->s->distances_between_clients[to_previous_client][to_block.front()]
                                    + this->s->distances_between_clients[to_previous_client][from_block.front()]
                                    - this->s->distances_between_clients[to_block.back()][to_next_client]
                                    + this->s->distances_between_clients[to_block.back()][from_next_client];

                    // Found a smaller cost after swaping blocks, save the route's and the block's indexes
                    if (current_cost <=  best_total_cost){
                        // But first make sure that the capacity are compatible with the route
                        if((this->route_demands[from_route] - capacity_from_block + capacity_to_block <= this->s->max_capacity) 
                            && (this->route_demands[to_route] - capacity_to_block + capacity_from_block <= this->s->max_capacity)){
                                // cout << "\tFOUND BETTER COST " << current_cost << " " << "\n";
                                best_total_cost = current_cost;
                                final_from_route = from_route;
                                final_to_route = to_route;
                                start_index_from = begin_from_block_index;
                                start_index_to = begin_to_block_index;
                                final_from_capacity = capacity_from_block;
                                final_to_capacity = capacity_to_block;
                            }
                    }

                    // Advance the second block as a sliding window
                    to_previous_client = to_block.front();
                    to_block.pop_front();
                    capacity_to_block -= this->s->demands_per_client[to_previous_client];
                    begin_to_block_index++;

                    capacity_to_block += this->s->demands_per_client[*to_client];
                    to_block.push_back(*to_client);
                    to_client++;
                    to_next_client = *to_client;
                }

                // Tested the swap with all blocks from this route, 
                // clear the data structure to start testing other route
                to_block.clear();
            }

            // Advance the first block as a sliding window
            from_previous_client = from_block.front();
            from_block.pop_front();
            capacity_from_block -= this->s->demands_per_client[from_previous_client];
            begin_from_block_index++;

            from_block.push_back(*from_client);
            capacity_from_block += this->s->demands_per_client[*from_client];
            from_client++;
            from_next_client = *from_client;
        }
        from_block.clear();
    }

    // If found a better neighbor update values
    if (best_total_cost <= this->fitness){
      // Updating best cost
      this->fitness = best_total_cost;

      // Swaping block
      for (short int i = 0; i <= block_size; i++) {
        swap(
          this->routes[final_from_route][start_index_from + i],
          this->routes[final_to_route][start_index_to + i]
        );
      }

      // Updating route demands
      this->route_demands[final_from_route] = this->route_demands[final_from_route] - final_from_capacity + final_to_capacity;
      this->route_demands[final_to_route] = this->route_demands[final_to_route] - final_to_capacity + final_from_capacity;

      return true;
    }


    return false;
}
