#include <iostream>
#include "Solution.h"
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

int main() {
    string file_names[] = {
        //"data/P-n19-k2.txt" ,
        "data/P-n45-k5.txt"//,
        //"data/P-n55-k7.txt",
        //"data/P-n20-k2.txt",
        //"data/P-n50-k10.txt",
        //"data/P-n23-k8.txt",
        //"data/P-n51-k10.txt"
    };

    for (auto &file_name : file_names)
    {
        ifstream file;
        file.open(file_name);

        if (!file.is_open())
        {
            cout << "Could not open the file";
            return 1;
        }

        cout << "Processing file " << file_name << "\n\n";

        // Ignore first line
        string ignore_line;
        getline(file, ignore_line);

        // Get the number of clients, vehicles an the capacity of each vehicle
        unsigned int clients, vehicles, capacity;
        getline(file, ignore_line, ':');
        file >> clients;
        getline(file, ignore_line, ':');
        file >> vehicles;
        getline(file, ignore_line, ':');
        file >> capacity;
        //cout << clients << " " << vehicles << " " << capacity << "\n";

        // Get the demand of each client
        getline(file, ignore_line, ':');
        vector<unsigned int> demands(clients);
        vector<vector<unsigned int>> distances(clients, vector<unsigned int>(clients));
        unsigned int ignore_int, value;
        for (int c = 0; c < clients; c++)
        {
            file >> ignore_int >> demands[c];
            //cout << c << " " << demands[c] << '\n';
        }

        // Ignore the next two lines
        getline(file, ignore_line, ' ');
        getline(file, ignore_line, ' ');

        // Get the distance between each client
        for (int i = 0; i < clients; i++)
        {
            //cout << "starting";
            for (int j = 0; j < clients; j++)
            {
                file >> distances[i][j];

                //cout << distances[i][j] << " ";
            }
        }

        file.close();

        /*
        // Printing the distance matrix in the python list format
        for (int i = 0; i < clients; i++) {
            cout << "[";
            for (int j = 0; j < clients; j++)
            {
                cout << distances[i][j];

                if (j != clients-1){
                    cout << ", ";
                }

                //cout << distances[i][j] << " ";
            }
            cout << "], \n";
        }*/

        
        // Running construction
        time_t begin, middle, end;

        for(short int i = 0; i < 1; i++){
            Scenary scenary(clients, vehicles, capacity, demands, distances);
            Solution solution(&scenary);
            begin = time(NULL);
            solution.construction();
            
            middle = time(NULL);
            //cout << difftime(middle, begin) << ", ";

            solution.vns(100);
            end = time(NULL);
            //cout << difftime(end, middle) << "\n";
        }
    }
}