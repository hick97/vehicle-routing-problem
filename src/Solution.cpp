#include "Solution.h"
#include <limits>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

Solution::Solution(Scenary *scenary) : fitness(0), s(scenary), route_demands(scenary->vehicles_number, 0), routes(scenary->vehicles_number)
{
}

Solution::Solution(Solution &sol) : fitness(sol.fitness), s(sol.s), route_demands(sol.route_demands), routes(sol.routes)
{
}
bool sortbysec(const pair<int, int> &a,
               const pair<int, int> &b)
{
  return (a.second > b.second);
}

unsigned int Solution::construction()
{

  //Starting candidates list
  vector<pair<int, int>> candidates;
  for (unsigned int i = 1; i < this->s->demands_per_client.size(); i++)
  {
    candidates.push_back(make_pair(i, this->s->demands_per_client[i]));
  }

  sort(candidates.begin(), candidates.end(), sortbysec);

  //Setting distribution center in each route
  for (unsigned int i = 0; i < this->routes.size(); i++)
  {
    for (unsigned int j = 0; j < 2; j++)
    {
      this->routes[i].push_back(0);
    }
  }

  while (candidates.size() > 0)
  {

    // Balance after "insertion"
    unsigned int balance;

    // Cadidate analyzed
    unsigned int current_candidate = candidates[0].first;
    unsigned int current_demand;

    // Position in which the candidate will be inserted
    int final_candidate_route = -1;
    int final_candidate_route_position = -1;

    //setting initial cost to infinity
    unsigned int better_cost = std::numeric_limits<unsigned int>::max();

    //Last route with capacity
    auto free_route = -1;

    //Walking in routes
    for (unsigned int r = 0; r < this->routes.size(); r++)
    {

      //Walking between route nodes
      for (unsigned int i = 1; i < this->routes[r].size(); i++)
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

      //Removing cadidate from list
      candidates.erase(candidates.begin());
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

        //Removing cadidate from list
        candidates.erase(candidates.begin());
      }
      else
      {
        // Node will still be in cadidate list
        continue;
      }
    }
  }
  // std::cout << this->fitness << ", ";

  return this->fitness;

  /*
  printf("Constructed route cost: %d\n", this->fitness);

  // Printando rotas:
  printf("Constructed routes: \n");

  for (unsigned int i = 0; i < this->routes.size(); i++)
  {
    for (unsigned int j = 0; j < this->routes[i].size(); j++)
    {
      cout << this->routes[i][j] << " ";
    }
    cout << "\n";
  }

  //this->print_state();
  //this->print_real_state();
  /*
  // Printando rotas:
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

void Solution::print_state()
{
  std::cout << "=================================\n";
  std::cout << "Current cost = " << this->fitness << "\nRoutes = \n";

  for (unsigned int route = 0; route < this->routes.size(); route++)
  {
    std::cout << "[";
    for (unsigned int client = 0; client < this->routes[route].size(); client++)
    {
      std::cout << this->routes[route][client];

      if (client < this->routes[route].size() - 1)
      {
        std::cout << ", ";
      }
    }

    if (route < this->routes.size() - 1)
    {
      std::cout << "],\n";
    }
    else
    {
      std::cout << "]\n";
    }
  }

  std::cout << "Demands = [";
  for (unsigned int demand = 0; demand < this->route_demands.size(); demand++)
  {
    std::cout << this->route_demands[demand];

    if (demand < this->route_demands.size() - 1)
    {
      std::cout << ", ";
    }
    else
    {
      std::cout << "]\n";
    }
  }
}

void Solution::print_real_state()
{
  std::cout << "=================================\n";
  unsigned long long cost = 0;
  for (auto &route : this->routes)
  {
    for (unsigned int c = 1; c < route.size(); c++)
    {
      cost += this->s->distances_between_clients[route[c - 1]][route[c]];
    }
  }
  std::cout << "Real cost should be = " << cost << "\n";

  std::cout << "Real demands should be = [";
  for (auto &route : this->routes)
  {
    cost = 0;
    for (auto &client : route)
    {
      cost += this->s->demands_per_client[client];
    }
    std::cout << cost << ", ";
  }
  std::cout << "]\n";
}

void Solution::vnd()
{
  unsigned int iteration = 0;
  //third moviment
  while (true)
  {

    //second moviment
    while (true)
    {

      //first moviment
      while (true)
      {
        //std::cout << "-----------------------------------------\n";
        //std::cout << "1 NEIGHBORHOOD - " << ++iteration << "\n";
        bool improved = firstMoviment();
        //print_state();
        //print_real_state();

        if (!improved)
          break;
      }

      //std::cout << "-----------------------------------------\n";
      //std::cout << "2 NEIGHBORHOOD - " << ++iteration << "\n";
      bool improved = secondMoviment();
      //print_state();
      //print_real_state();

      if (!improved)
        break;
    }
    //std::cout << "-----------------------------------------\n";
    //std::cout << "3 NEIGHBORHOOD - " << ++iteration << "\n";
    bool improved = thirdMoviment();
    //print_state();
    //print_real_state();

    if (!improved)
      break;
  }

  /*
  std::cout << "VND cost: " << this->fitness << "\n";
  std::cout << "VND routes:\n";
  for (auto &route : this->routes)
  {
    for (auto &client : route)
    {
      std::cout << client << " ";
    }
    std::cout << "\n";
  }*/
}

unsigned int Solution::vns(unsigned int iteration_limit)
{
  Solution cur_solution = *this;
  unsigned int max_neighborhood = 2;

  while (iteration_limit--)
  {
    unsigned int cur_neighborhood = 1;

    while (cur_neighborhood <= max_neighborhood)
    {
      Solution neighbor(cur_solution);

      switch (cur_neighborhood)
      {
      case 1:
        neighbor.swapping_random_nodes();
        break;
      case 2:
        neighbor.swapping_nodes_between_routes();
        break;
      }
      /*
      std::cout << "Vizinho antes VND vizinhança " << cur_neighborhood;
      neighbor.print_state();
      neighbor.print_real_state();*/

      neighbor.vnd();

      /*
      std::cout << "Vizinho depois VND vizinhança " << cur_neighborhood;
      neighbor.print_state();
      neighbor.print_real_state();*/

      if (neighbor.fitness < cur_solution.fitness)
      {
        cur_solution = neighbor;
        cur_neighborhood = 1;
      }
      else
      {
        cur_neighborhood++;
      }

      //cur_solution.print_state();
      //cur_solution.print_real_state();
    }
  }
  //std::cout << "Solução final \n";
  //std::cout << cur_solution.fitness << ", ";
  cur_solution.print_state();
  //cur_solution.print_real_state();
  return cur_solution.fitness;
}

bool Solution::firstMoviment()
{
  bool improved = false;

  //Starting candidates list with all clients
  vector<unsigned int> candidates;
  for (unsigned int i = 1; i < this->s->demands_per_client.size(); i++)
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
    for (unsigned int r = 0; r < this->routes.size(); r++)
    {

      //Walking between route nodes
      for (unsigned int i = 1; i < this->routes[r].size(); i++)
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

  /*
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
  */

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
  for (unsigned int i = 0; i < this->routes.size(); i++)
  {
    for (unsigned int j = 0; j < this->routes[i].size(); j++)
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

bool Solution::secondMoviment()
{
  return swap_best_block_neighbor(0);
}

bool Solution::thirdMoviment()
{
  return swap_best_block_neighbor(3);
}

void print_block(const deque<unsigned int> &block)
{
  for (auto value : block)
  {
    cout << value << " ";
  }
}

bool Solution::swap_best_block_neighbor(unsigned int block_size)
{
  // Save the best total cost
  unsigned int best_total_cost = std::numeric_limits<unsigned int>::max();
  unsigned int final_from_route, final_to_route, start_index_from, start_index_to,
      final_from_capacity, final_to_capacity;

  deque<unsigned int> from_block, to_block;
  unsigned int current_cost;

  for (unsigned int from_route = 0; from_route < this->routes.size(); from_route++)
  {
    //cout << "NEW ROUTE\n";

    // Make the first block in this route
    unsigned int from_previous_client = 0;
    auto from_client = next(this->routes[from_route].begin(), 1);
    unsigned int begin_from_block_index = 0, capacity_from_block = 0;
    while (from_client != this->routes[from_route].end() && from_block.size() <= block_size)
    {
      from_block.push_back(*from_client);
      capacity_from_block += this->s->demands_per_client[*from_client];
      from_client++;
    }
    unsigned int from_next_client = *from_client;

    // Now lets compare the cost of swaping this block with blocks from other routes
    while (from_client != this->routes[from_route].end())
    {
      //cout << "First block = "; print_block(from_block); cout << "\n";
      //cout << "Capacity is " << capacity_from_block << "\n";

      for (unsigned int to_route = from_route + 1; to_route < this->routes.size(); to_route++)
      {
        if (this->routes[to_route].size() - 2 < block_size + 1)
        {
          // Not enough clients to from a block
          continue;
        }
        // Make the first block with this route intended to swap with the other block previously made
        unsigned int to_previous_client = 0;
        auto to_client = next(this->routes[to_route].begin(), 1);
        unsigned int begin_to_block_index = 0, capacity_to_block = 0;
        while (to_client != this->routes[to_route].end() && to_block.size() <= block_size)
        {
          to_block.push_back(*to_client);
          capacity_to_block += this->s->demands_per_client[*to_client];
          to_client++;
        }
        unsigned int to_next_client = *to_client;

        while (to_client != this->routes[to_route].end())
        {
          //cout << "\tSecond block = "; print_block(to_block); cout << "\n";
          //cout << "\tCapacity is " << capacity_to_block << "\n";

          // Calculate the route cost after swaping these two blocks
          current_cost = this->fitness - this->s->distances_between_clients[from_previous_client][from_block.front()] + this->s->distances_between_clients[from_previous_client][to_block.front()] - this->s->distances_between_clients[from_block.back()][from_next_client] + this->s->distances_between_clients[from_block.back()][to_next_client] - this->s->distances_between_clients[to_previous_client][to_block.front()] + this->s->distances_between_clients[to_previous_client][from_block.front()] - this->s->distances_between_clients[to_block.back()][to_next_client] + this->s->distances_between_clients[to_block.back()][from_next_client];

          // Found a smaller cost after swaping blocks, save the route's and the block's indexes
          if (current_cost < best_total_cost)
          {
            // But first make sure that the capacity are compatible with the route
            if ((this->route_demands[from_route] - capacity_from_block + capacity_to_block <= this->s->max_capacity) && (this->route_demands[to_route] - capacity_to_block + capacity_from_block <= this->s->max_capacity))
            {
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
  /*
    std::cout << "Best neighbor cost is " << best_total_cost << "\n";
    std::cout << "Swaping [ ";
    short int i = 0;
    while (i++ <= block_size){
        std::cout << this->routes[final_from_route][start_index_from + i] << " ";
    }
    std::cout << "] from route " << final_from_route;

    std::cout << " with [ ";
    i = 0;
    while (i++ <= block_size){
        std::cout << routes[final_to_route][start_index_to + i] << " ";
    }
    std::cout << "] from route " << final_to_route << "\n";*/
  // If found a better cost update values
  if (best_total_cost < this->fitness)
  {

    // Updating best cost
    this->fitness = best_total_cost;

    // Swaping block
    unsigned int i = 0;
    while (i++ <= block_size)
    {
      swap(
          this->routes[final_from_route][start_index_from + i],
          this->routes[final_to_route][start_index_to + i]);
    }

    // Updating route demands
    this->route_demands[final_from_route] = this->route_demands[final_from_route] - final_from_capacity + final_to_capacity;
    this->route_demands[final_to_route] = this->route_demands[final_to_route] - final_to_capacity + final_from_capacity;

    return true;
  }

  return false;
}

// Disturbances

void Solution::swapping_random_nodes()
{

  // Copying current solution
  //Solution solCopy(sol);
  //srand(time(NULL));

  //Choosing a random route to swap nodes
  int random_route = (rand() % (this->routes.size()));

  // Choosing two position in random route to swap
  int first_pos = rand() % (this->routes[random_route].size() - 2) + 1;
  int second_pos = rand() % (this->routes[random_route].size() - 2) + 1;

  // Avoid choosing consecutive nodes
  while (second_pos == first_pos + 1 || second_pos == first_pos - 1)
  {
    second_pos = rand() % (this->routes[random_route].size() - 2) + 1;
  }

  // Getting elements
  unsigned int first_element = this->routes[random_route][first_pos];
  unsigned int second_element = this->routes[random_route][second_pos];
  /*
  cout << "Pos aleatoria 1: " << first_pos << "\n";
  cout << "Pos aleatoria 2: " << second_pos << "\n";
  cout << "Troquei o elemento: " << first_element << "\n";
  cout << "Com o elemento: " << second_element << "\n";
  */

  unsigned int previous_client_first = this->routes[random_route][first_pos - 1];
  unsigned int next_client_first = this->routes[random_route][first_pos + 1];
  unsigned int previous_client_second = this->routes[random_route][second_pos - 1];
  unsigned int next_client_second = this->routes[random_route][second_pos + 1];

  this->fitness = this->fitness - this->s->distances_between_clients[previous_client_first][first_element] + this->s->distances_between_clients[previous_client_second][first_element] - this->s->distances_between_clients[next_client_first][first_element] + this->s->distances_between_clients[next_client_second][first_element] - this->s->distances_between_clients[previous_client_second][second_element] + this->s->distances_between_clients[previous_client_first][second_element] - this->s->distances_between_clients[next_client_second][second_element] + this->s->distances_between_clients[next_client_first][second_element];

  /*
  // Updating cost after swap
  unsigned int next_value = this->routes[random_route][second_pos + 1];
  unsigned int previous_value = this->routes[random_route][second_pos - 1];

  this->fitness +=
      (this->s->distances_between_clients[previous_value][first_element] +
       this->s->distances_between_clients[next_value][first_element]) -
      (this->s->distances_between_clients[previous_value][second_element] +
       this->s->distances_between_clients[second_element][next_value]);

  next_value = this->routes[random_route][first_pos + 1];
  previous_value = this->routes[random_route][first_pos - 1];

  this->fitness +=
      (this->s->distances_between_clients[previous_value][second_element] +
       this->s->distances_between_clients[next_value][second_element]) -
      (this->s->distances_between_clients[previous_value][first_element] +
       this->s->distances_between_clients[first_element][next_value]); 
  */

  // Updating routes
  swap(
      this->routes[random_route][first_pos],
      this->routes[random_route][second_pos]);
}

void Solution::swapping_nodes_between_routes()
{
  // Copying current solution

  //Choosing random routes to swap nodes
  int first_random_route = -1;
  int second_random_route = -1;

  first_random_route = rand() % (this->routes.size());
  second_random_route = rand() % (this->routes.size());

  // Avoid choosing the same route twice
  while (first_random_route == second_random_route)
  {
    srand(time(NULL));
    second_random_route = rand() % (this->routes.size());
  }

  // Choosing one position in each route
  int first_pos = rand() % (this->routes[first_random_route].size() - 2) + 1;
  int second_pos = rand() % (this->routes[second_random_route].size() - 2) + 1;

  // Getting elements
  unsigned int first_element = this->routes[first_random_route][first_pos];
  unsigned int second_element = this->routes[second_random_route][second_pos];

  // Checking capacities after changes
  while (
      (this->route_demands[first_random_route] +
           this->s->demands_per_client[second_element] -
           this->s->demands_per_client[first_element] >
       this->s->max_capacity) ||
      (this->route_demands[second_random_route] +
           this->s->demands_per_client[first_element] -
           this->s->demands_per_client[second_element] >
       this->s->max_capacity))
  {
    srand(time(NULL));
    first_random_route = rand() % (this->routes.size());
    second_random_route = rand() % (this->routes.size());

    // Getting elements
    first_element = this->routes[first_random_route][first_pos];
    second_element = this->routes[second_random_route][second_pos];

    // Choosing one position in each route
    first_pos = rand() % (this->routes[first_random_route].size() - 2) + 1;
    second_pos = rand() % (this->routes[second_random_route].size() - 2) + 1;

    //cout << "to preso aqui \n";
    //cout << "first pos: " << first_pos << "\n";
    //cout << "second pos: " << second_pos << "\n";

    // Getting elements
    first_element = this->routes[first_random_route][first_pos];
    second_element = this->routes[second_random_route][second_pos];
  }

  unsigned int next_value = -1;
  unsigned int previous_value = -1;

  /*
  unsigned int previous_client_first = this->routes[first_random_route][first_pos - 1];
  unsigned int next_client_first = this->routes[first_random_route][first_pos + 1];
  unsigned int previous_client_second = this->routes[second_random_route][second_pos - 1];
  unsigned int next_client_second = this->routes[second_random_route][second_pos + 1];

  this->fitness = this->fitness 
                    - this->s->distances_between_clients[previous_client_first][first_element] 
                    + this->s->distances_between_clients[previous_client_second][first_element] 
                    - this->s->distances_between_clients[next_client_first][first_element] 
                    + this->s->distances_between_clients[next_client_second][first_element] 
                    - this->s->distances_between_clients[previous_client_second][second_element] 
                    + this->s->distances_between_clients[previous_client_first][second_element] 
                    - this->s->distances_between_clients[next_client_second][second_element] 
                    + this->s->distances_between_clients[next_client_first][second_element];
  */

  // Updating cost

  //Udating cost in second route
  next_value = this->routes[second_random_route][second_pos + 1];
  previous_value = this->routes[second_random_route][second_pos - 1];

  this->fitness +=
      (this->s->distances_between_clients[previous_value][first_element] +
       this->s->distances_between_clients[next_value][first_element]) -
      (this->s->distances_between_clients[previous_value][second_element] +
       this->s->distances_between_clients[second_element][next_value]);

  // Updating cost in first route
  next_value = this->routes[first_random_route][first_pos + 1];
  previous_value = this->routes[first_random_route][first_pos - 1];

  this->fitness +=
      (this->s->distances_between_clients[previous_value][second_element] +
       this->s->distances_between_clients[next_value][second_element]) -
      (this->s->distances_between_clients[previous_value][first_element] +
       this->s->distances_between_clients[first_element][next_value]);

  // Updating demands

  // Updating demand in second random route
  this->route_demands[second_random_route] +=
      this->s->demands_per_client[first_element] -
      this->s->demands_per_client[second_element];

  // Updating demand in second random route
  this->route_demands[first_random_route] +=
      this->s->demands_per_client[second_element] -
      this->s->demands_per_client[first_element];

  // Updating routes
  swap(
      this->routes[first_random_route][first_pos],
      this->routes[second_random_route][second_pos]);

  /*
  unsigned int cost = 0;
  for (auto &route : solCopy.routes)
  {
    for (unsigned int c = 1; c < route.size(); c++)
    {
      cost += solCopy.s->distances_between_clients[route[c - 1]][route[c]];
    }
  }
  std::cout << "Real cost should be = " << cost << "\n";

  std::cout << "Real demands should be = [";
  for (auto &route : this->routes)
  {
    cost = 0;
    for (auto &client : route)
    {
      cost += solCopy.s->demands_per_client[client];
    }
    std::cout << cost << ", ";
  }
  std::cout << "]\n";

  cout << "\nnew fitness: " << solCopy.fitness << "\n";
  */
}
