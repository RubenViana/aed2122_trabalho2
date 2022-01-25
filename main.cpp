#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "graph.h"
#include <map>
#include <cmath>

using namespace std;

struct Stop {       //Code,Name,Zone,Latitude,Longitude
    string code;
    string name;
    string zone;
    string latitude;
    string longitude;
};

struct Line {
    string code;
    string name;
    vector<string> stopsDir0;
    vector<string> stopsDir1;
};

vector<Stop> readStopsFile(const string &filename, map<string,int> &stopsIndex) {
    vector<Stop> stops;
    ifstream stopsFile(filename);
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

        stops.push_back(stop);
        stopsIndex.insert({stop.code, i});

        i++;
    }
    return stops;
}

vector<Line> readLinesFile(const string &filename) {
    vector<Line> allLines;
    ifstream linesFile(filename);
    string title;
    getline(linesFile, title);
    while (!linesFile.eof()) {
        Line line;
        getline(linesFile, line.code, ',');
        getline(linesFile,line.name);
        allLines.push_back(line);
    }
    return allLines;
}

void readLineStops (vector<Line> &allLines) {
    for (auto &l : allLines){
        stringstream ss0("dataset\\line_" + l.code + "_0.csv");
        ifstream lineStops0(ss0.str());
        string numberStops0;
        getline(lineStops0, numberStops0);
        for (int i = 0; i < stoi(numberStops0); i++){
            string code;
            getline(lineStops0, code);
            l.stopsDir0.push_back(code);
        }

        stringstream ss1("dataset\\line_" + l.code + "_1.csv");
        ifstream lineStops1(ss1.str());
        string numberStops1;
        getline(lineStops1, numberStops1);
        for (int i = 0; i < stoi(numberStops1); i++){
            string code;
            getline(lineStops1, code);
            l.stopsDir1.push_back(code);
        }


    }
}

double haversine(double lat1, double lon1, double lat2, double lon2){
    // distance between latitudes and longitudes
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return (rad * c) * 1000;     // returns the distance in Km
}

int main() {
    map<string, int> stopsIndex;
    vector<Stop> stops = readStopsFile("dataset\\stops.csv", stopsIndex);
    vector<Line> allLines = readLinesFile("dataset\\lines.csv");
    allLines.erase(allLines.begin() + allLines.size() - 1);

    readLineStops(allLines);
/*
    //printing the stopsIndex :
    for (auto &s : stopsIndex) {
        cout << s.first << " -> " << s.second << endl;
    }
    //-------
    //printing the stops :
    for (auto &s : stops) {
        cout << s.code << " / " << s.name << " / " << s.zone << " / " << s.latitude << " / " << s.longitude << endl;
    }
    //-------


    //printing the lines :
    for (auto &l : allLines) {
        cout << l.code << " / " << l.name << endl;
    }
    //-------

    //printing the stops of ev :
    for (auto &l : allLines) {
        cout << l.code << " / " << l.name << endl;
        cout << "direction 0 :" << endl;
        for (auto &s : l.stopsDir0)
            cout << stops[stopsIndex[s] - 1].latitude << " / " << stops[stopsIndex[s] - 1].longitude << " -> " ;
        cout << endl;
        cout << "direction 1 :" << endl;
        for (auto &s : l.stopsDir1)
            cout << stops[stopsIndex[s] - 1].latitude << " / " << stops[stopsIndex[s] - 1].longitude << " -> " ;

        cout << endl;
    }

    //-------

    //cout << (int)haversine(stod(stops[stopsIndex[allLines[1].stopsDir0[1]]-1].latitude),stod(stops[stopsIndex[allLines[1].stopsDir0[1]]-1].longitude),stod(stops[stopsIndex[allLines[1].stopsDir0[1+1]]-1].latitude),stod(stops[stopsIndex[allLines[1].stopsDir0[1+1]]-1].longitude)) << endl;
    cout << stod(stops[stopsIndex[allLines[0].stopsDir0[0]]-1].latitude) << endl;
    cout << stod(stops[stopsIndex[allLines[0].stopsDir0[0]]-1].longitude) << endl;
    cout << stod(stops[stopsIndex[allLines[0].stopsDir0[1]]-1].latitude) << endl;
    cout << stod(stops[stopsIndex[allLines[0].stopsDir0[1]]-1].longitude) << endl;
*/

    Graph graph(stopsIndex.size(), true);

    for (int j = 0; j < allLines.size(); j++){

        for (int i = 0; i < allLines[j].stopsDir0.size() - 1; i++){
            graph.addEdge(stopsIndex[allLines[j].stopsDir0[i]],stopsIndex[allLines[j].stopsDir0[i + 1]], haversine(stod(stops[stopsIndex[allLines[j].stopsDir0[i]]-1].latitude),stod(stops[stopsIndex[allLines[j].stopsDir0[i]]-1].longitude),stod(stops[stopsIndex[allLines[j].stopsDir0[i+1]]-1].latitude),stod(stops[stopsIndex[allLines[j].stopsDir0[i+1]]-1].longitude)),allLines[j].code);
        }

        if (allLines[j].stopsDir1.size() != 0) {
            for (int i = 0; i < allLines[j].stopsDir1.size() - 1; i++) {
                graph.addEdge(stopsIndex[allLines[j].stopsDir1[i]], stopsIndex[allLines[j].stopsDir1[i + 1]], haversine(stod(stops[stopsIndex[allLines[j].stopsDir1[i]]-1].latitude),stod(stops[stopsIndex[allLines[j].stopsDir1[i]]-1].longitude),stod(stops[stopsIndex[allLines[j].stopsDir1[i+1]]-1].latitude),stod(stops[stopsIndex[allLines[j].stopsDir1[i+1]]-1].longitude)),allLines[j].code);
            }
        }
    }

    cout << graph.dijkstra_distance(stopsIndex["AAL2"], stopsIndex["BARC1"]) << endl;
    list<int> path = graph.dijkstra_path(stopsIndex["AAL2"], stopsIndex["BARC1"]);
    for (auto p : path) {
        cout << stops[p].code << endl;
    }
    //graph.print();
    return 0;
}
