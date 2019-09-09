#include "Solution.h"
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

Solution::Solution(Scenary *scenary) : fitness(0), s(scenary)
{
}

unsigned int Solution::construction()
{

  //Starting candidates list
  vector<unsigned int> candidates;
  for (auto i = 1; i < this->s->demands_per_client.size(); i++)
  {
    candidates.push_back(i);
  }

  //Starting routes capacities
  vector<unsigned int> route_demands(this->s->vehicles_number, 0);

  // Initializing routes
  vector<vector<unsigned int>> routes(this->s->vehicles_number);
  unsigned int total_cost = 0;

  //Setting distribution center in each route
  for (int i = 0; i < routes.size(); i++)
  {
    for (int j = 0; j < 2; j++)
    {
      routes[i].push_back(0);
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

    //printf("Candidate: %u\n", current_candidate);

    // Position in which the candidate will be inserted
    int final_candidate_route = -1;
    int final_candidate_route_position = -1;

    //setting initial cost to infinity
    unsigned int better_cost = std::numeric_limits<unsigned int>::max();

    //Last route with capacity
    auto free_route = -1;

    //Walking in routes
    for (int r = 0; r < routes.size(); r++)
    {
      //printf("-------------------- WALKIN IN ROUTE %d ---------- \n\n", r);

      //Walking between route nodes
      for (int i = 1; i < routes[r].size(); i++)
      {

        // Checking if route have capacity
        if (route_demands[r] >= this->s->max_capacity)
        {
          //printf("Dont have capacity in currrent route\n");
          break;
        }

        // Setting route as last route with capacity
        free_route = r;

        current_demand = this->s->demands_per_client[current_candidate];

        unsigned int current_value = routes[r][i];
        unsigned int previous_value = routes[r][i - 1];

        //Checking what change ocurred with node "insertion"
        balance =
            total_cost +
            (this->s->distances_between_clients[previous_value][current_candidate] +
             this->s->distances_between_clients[current_candidate][current_value]) -
            (this->s->distances_between_clients[current_value][previous_value]);

        //printf("Balance: %u\n", balance);
        //printf("Better cost: %u\n", better_cost);

        //Checking if have capacity with node insertition
        if (route_demands[r] + current_demand <= this->s->max_capacity)
        {

          // Checking if had performance
          if (balance < better_cost)
          {
            // Updating better cost
            better_cost = balance;

            // Updating candidate position to be inserted
            final_candidate_route = r;
            final_candidate_route_position = i;
          }
        }
        else
        {
          //printf("Route %d is full\n", r);
        }
      }
    }
    if (final_candidate_route != -1 && final_candidate_route_position != -1)
    {
      // Real insertition
      auto itPos = routes[final_candidate_route].begin() + final_candidate_route_position;
      routes[final_candidate_route].insert(itPos, current_candidate);
      route_demands[final_candidate_route] += current_demand;

      //Updating total cost
      total_cost = better_cost;
    }
    else
    {
      //If there is no improvement:
      //Updating total cost
      total_cost = total_cost +
                   (this->s->distances_between_clients[current_candidate][0] +
                    this->s->distances_between_clients[current_candidate][routes[free_route][routes[free_route].size() - 2]]);

      // Inserting candidate in last free route
      auto itPos = routes[free_route].end() - 1;
      routes[free_route].insert(itPos, current_candidate);

      route_demands[free_route] += this->s->demands_per_client[current_candidate];
    }

    //Removing cadidate from list
    candidates.erase(candidates.begin() + random_index);

    /*
    printf("List after remove: \n");
    for (auto i = 0; i < candidates.size(); i++)
    {
      cout << candidates[i] << " ";
    }
    cout << "\n";
    */
  }
  printf("Final cost: %d\n", total_cost);

  // Printando rotas:
  printf("Final routes: \n");

  for (int i = 0; i < routes.size(); i++)
  {
    for (int j = 0; j < routes[i].size(); j++)
    {
      cout << routes[i][j] << " ";
    }
    cout << "\n";
  }

  cout << "\n";

  // Printando rotas:
  printf("Final demands: \n");

  for (int i = 0; i < routes.size(); i++)
  {
    for (int j = 0; j < routes[i].size(); j++)
    {
      cout << this->s->demands_per_client[routes[i][j]] << " ";
    }
    cout << "\n";
  }

  cout << "\n";
}