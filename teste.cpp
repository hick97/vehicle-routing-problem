#include <deque>
#include <limits>
#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

typedef pair<unsigned int, unsigned int> pi;

void print_block(const deque<unsigned int> &block){
    for (auto value : block) {
        cout << value << " ";
    }
}

void neighborhood_change(){
    // TO DO: Really swap the blocks
}

pair<unsigned int, pair<pair<unsigned int, unsigned int>, pair<unsigned int, unsigned int>>> best_block_neighbor(
        short int block_size, 
        const vector<vector<unsigned int>> &routes, 
        unsigned int total_route_cost, 
        const vector<unsigned int> &demands_per_route,
        const vector<unsigned int> &demands,
        unsigned int max_capacity,
        const vector<vector<unsigned int>> &distances){

    // Save the best total cost
    unsigned int best_total_cost = std::numeric_limits<unsigned int>::max();
    unsigned int final_from_route, final_to_route, start_index_from, start_index_to; 
    
    deque<unsigned int> from_block, to_block;
    unsigned int current_cost;

    for (unsigned int from_route = 0; from_route < routes.size(); from_route++) {
        cout << "NEW ROUTE\n";

        // Make the first block in this route
        unsigned int from_previous_client = 0;
        auto from_client = next(routes[from_route].begin(), 1);
        unsigned int begin_from_block_index = 0, capacity_from_block = 0;
        while (from_client != routes[from_route].end() && from_block.size() <= block_size) {
            from_block.push_back(*from_client);
            capacity_from_block += demands[*from_client];
            from_client++;
        }
        unsigned int from_next_client = *from_client;

        // Now lets compare the cost of swaping this block with blocks from other routes
        while (from_client != routes[from_route].end()) {
            //cout << "First block = "; print_block(from_block); cout << "\n";
            //cout << "Capacity is " << capacity_from_block << "\n";

            for (unsigned int to_route = 0; to_route < routes.size(); to_route++) {
                if (from_route == to_route){
                    // Trying swap block between the same routes
                    continue;
                }

                // Make the first block with this route intended to swap with the other block previously made
                unsigned int to_previous_client = 0;
                auto to_client = next(routes[to_route].begin(), 1);
                unsigned int begin_to_block_index = 0, capacity_to_block = 0;
                while (to_client != routes[to_route].end() && to_block.size() <= block_size) {
                    to_block.push_back(*to_client);
                    capacity_to_block += demands[*to_client];
                    //to_previous_client = *to_client;
                    to_client++;
                }
                unsigned int to_next_client = *to_client;

                while (to_client != routes[to_route].end()) {
                    //cout << "\tSecond block = "; print_block(to_block); cout << "\n";
                    //cout << "\tCapacity is " << capacity_to_block << "\n";
                    
                    // Calculate the route cost after swaping these two blocks
                    current_cost = total_route_cost 
                                    - distances[from_previous_client][from_block.front()]  
                                    + distances[from_previous_client][to_block.front()]
                                    - distances[from_block.back()][from_next_client]
                                    + distances[from_block.back()][to_next_client]
                                    - distances[to_previous_client][to_block.front()]
                                    + distances[to_previous_client][from_block.front()]
                                    - distances[to_block.back()][to_next_client]
                                    + distances[to_block.back()][from_next_client];

                    // Found a smaller cost after swaping blocks, save the route's and the block's indexes
                    if (current_cost < best_total_cost){
                        // But first make sure that the capacity are compatible with the route
                        if((demands_per_route[from_route] - capacity_from_block + capacity_to_block <= max_capacity) 
                            && (demands_per_route[to_route] - capacity_to_block + capacity_from_block <= max_capacity)){
                                cout << "FOUND BETTER COST " << current_cost << "\n";
                                cout << "From previous = " << from_previous_client << "\n";
                                cout << "From block = "; print_block(from_block); cout << "\n";
                                cout << "From next = " << from_next_client << "\n";
                                cout << "To previous = " << to_previous_client << "\n";
                                cout << "To block = "; print_block(to_block); cout << "\n";
                                cout << "To next = " << to_next_client << "\n";

                                best_total_cost = current_cost;
                                final_from_route = from_route;
                                final_to_route = to_route;
                                start_index_from = begin_from_block_index;
                                start_index_to = begin_to_block_index;
                            }
                    }

                    // Advance the second block as a sliding window
                    to_previous_client = to_block.front();
                    to_block.pop_front();
                    capacity_to_block -= demands[to_previous_client];
                    begin_to_block_index++;

                    capacity_to_block += demands[*to_client];
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
            capacity_from_block -= demands[from_previous_client];
            begin_from_block_index++;

            from_block.push_back(*from_client);
            capacity_from_block += demands[*from_client];
            from_client++;
            from_next_client = *from_client;
        }
        from_block.clear();
    }

    return {best_total_cost, {{final_from_route, start_index_from}, {final_to_route, start_index_to}}};
}

unsigned int calculate_cost(const vector<vector<unsigned int>> &routes,  
        const vector<vector<unsigned int>> &distances){

        unsigned int cost = 0;
        for (auto &route : routes) {
            for (int c = 1; c < route.size(); c++) {
                cost += distances[route[c-1]][route[c]];
            }
        }

        return cost;
}

vector<unsigned int> calculate_demand(const vector<vector<unsigned int>> &routes, 
                                      const vector<unsigned int> &demands){
    vector<unsigned int> demands_per_route;
    for (auto &route : routes){
        unsigned int capacity = 0;

        for (int c = 1; c < route.size(); c++) {
                capacity += demands[route[c]];
        }

        demands_per_route.push_back(capacity);
    }
    
    return demands_per_route;
}


int main(){
    vector<vector<unsigned int>> routes = {
        {0, 2, 7, 15, 9, 16, 17, 11, 4, 1, 0},
        {0, 10, 14, 12, 3, 8, 13, 5, 18, 6, 0}
    };

    vector<unsigned int> demands = {0, 19, 30, 16, 23, 11, 31, 15, 28, 14, 8, 7, 14, 19, 11, 26, 17, 6, 15};

    vector<vector<unsigned int>> distances = {
        {0, 14, 19, 33, 22, 23, 12, 22, 32, 32, 21, 28, 30, 31, 30, 32, 39, 44, 16},
        {14, 0, 15, 19, 12, 24, 12, 19, 21, 27, 7, 19, 16, 33, 17, 31, 27, 31, 19},
        {19, 15, 0, 21, 26, 10, 7, 4, 17, 13, 16, 33, 25, 18, 29, 16, 24, 30, 9},
        {33, 19, 21, 0, 21, 31, 25, 23, 8, 24, 12, 25, 9, 37, 16, 32, 10, 12, 30},
        {22, 12, 26, 21, 0, 36, 24, 30, 26, 37, 12, 7, 13, 44, 9, 42, 31, 33, 30},
        {23, 24, 10, 31, 36, 0, 13, 8, 25, 13, 26, 43, 35, 8, 39, 9, 32, 38, 7},
        {12, 12, 7, 25, 24, 13, 0, 10, 23, 20, 16, 31, 26, 21, 28, 21, 30, 35, 7},
        {22, 19, 4, 23, 30, 8, 10, 0, 18, 10, 19, 37, 28, 15, 32, 12, 24, 30, 9},
        {32, 21, 17, 8, 26, 25, 23, 18, 0, 17, 15, 32, 17, 31, 23, 25, 7, 13, 26},
        {32, 27, 13, 24, 37, 13, 20, 10, 17, 0, 25, 44, 31, 16, 37, 9, 21, 27, 18},
        {21, 7, 16, 12, 12, 26, 16, 19, 15, 25, 0, 19, 10, 34, 13, 31, 21, 24, 22},
        {28, 19, 33, 25, 7, 43, 31, 37, 32, 44, 19, 0, 16, 51, 10, 49, 35, 36, 38},
        {30, 16, 25, 9, 13, 35, 26, 28, 17, 31, 10, 16, 0, 43, 6, 38, 19, 20, 32},
        {31, 33, 18, 37, 44, 8, 21, 15, 31, 16, 34, 51, 43, 0, 47, 7, 36, 42, 15},
        {30, 17, 29, 16, 9, 39, 28, 32, 23, 37, 13, 10, 6, 47, 0, 43, 26, 26, 35},
        {32, 31, 16, 32, 42, 9, 21, 12, 25, 9, 31, 49, 38, 7, 43, 0, 30, 36, 16},
        {39, 27, 24, 10, 31, 32, 30, 24, 7, 21, 21, 35, 19, 36, 26, 30, 0, 6, 33},
        {44, 31, 30, 12, 33, 38, 35, 30, 13, 27, 24, 36, 20, 42, 26, 36, 6, 0, 38},
        {16, 19, 9, 30, 30, 7, 7, 9, 26, 18, 22, 38, 32, 15, 35, 16, 33, 38, 0}
        };

        
    for (auto r : routes){
        for (auto v : r){
            cout << v << " ";
        }
        cout << "\n";
    }

    unsigned int total_cost = calculate_cost(routes, distances);
    auto demands_per_route = calculate_demand(routes, demands);


    unsigned int block_size = 0;
    cout << "Total cost = " << total_cost << "\n";
    auto result = best_block_neighbor(block_size, routes, total_cost, demands_per_route, demands, 160, distances);


    cout << "Best neighbor cost is " << result.first << "\n";
    pair<unsigned int, unsigned int> from_result, to_result;
    tie(from_result, to_result) = result.second;

    cout << "Swaping [ ";
    short int i = 0;
    while (i++ <= block_size){
        cout << routes[from_result.first][from_result.second + i] << " ";
    }
    cout << "] from route " << from_result.first;

    cout << " with [ ";
    i = 0;
    while (i++ <= block_size){
        cout << routes[to_result.first][to_result.second + i] << " ";
    }
    cout << "] from route " << to_result.first << "\n";
        
}