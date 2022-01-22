#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "graph.h"
using namespace std;

struct Stop {       //Code,Name,Zone,Latitude,Longitude
    string code;
    string name;
    string zone;
    string latitude;
    string longitude;
};

bool readStopsFile(const string &filename, vector<pair<int,Stop>>& stops) {
    ifstream stopsFile(filename);
    if (!stopsFile.is_open() || stopsFile.eof())
        return false;
    string title;
    getline(stopsFile, title);
    int i = 1;
    while (!stopsFile.eof()){
        Stop stop;
        getline(stopsFile, stop.code, ',');
        getline(stopsFile, stop.name, ',');
        getline(stopsFile, stop.zone, ',');
        getline(stopsFile, stop.latitude, ',');
        getline(stopsFile, stop.longitude);

        pair<int,Stop> stopPair(i,stop);
        stops.push_back(stopPair);
        i++;
    }
    return true;
}

bool readLines(string filename) {
    ifstream stopsFile(filename);
    if (!stopsFile.is_open() || stopsFile.eof())
        return false;

}

int main() {
    vector<pair<int,Stop>> stopsIndex;

    if (!readStopsFile("dataset\\stops.csv", stopsIndex))
        return 1;

    //printing the stops :
    for (auto &s : stopsIndex) {
        cout << s.first << endl;
        cout << s.second.code << " / " << s.second.name << " / " << s.second.zone << " / " << s.second.latitude << " / " << s.second.longitude << endl;
    }
    //-------
    Graph graph(stopsIndex.size(), true);
    for (auto &s : stopsIndex) {
        graph.addEdge();  // need to read the lines first !
    }

    return 0;
}
