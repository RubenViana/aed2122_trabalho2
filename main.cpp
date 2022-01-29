#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "graph.h"
#include <map>
#include <cmath>
#include <iomanip>

#if defined(_WIN32)
#define CLEAR_MACRO() system("cls")
#elif defined(unix) || defined(__unix__) || defined(__unix)
#define CLEAR_MACRO() system("clear")
#endif

using namespace std;

/**
 * This struct saves all the information about a Stop, like the code, name, zone,
 * latitude and longitude.
 */
struct Stop {       //Code,Name,Zone,Latitude,Longitude
    string code;
    string name;
    string zone;
    string latitude;
    string longitude;
};

/**
 * This struct saves all the information about the line of the bus, like the code, name
 * and the path of both directions.
 */
struct Line {
    string code;
    string name;
    vector<string> stopsDir0;
    vector<string> stopsDir1;
};

/**
 * This function reads the given file that contains all the information about the STCP stops,
 * and saves them in a vector with the name of the respective stop and gives to each
 * one of them an index (1,...,n)
 * @param filename (file name with information about each stop)
 * @param stopsIndex (vector that contains the code and the given index of each stop)
 * @return This function returns the vector with all STCP stops
 */
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

/**
 * This functions reads the file that contain all the information about the lines in a vector
 * of the type Line (previous created struct).
 * @param filename (file name with information about each Line)
 * @return This function returns the vector with all the lines.
 */
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

/**
 * This function reads the information of all the files of all lines and constructs the vector
 * that contains the track of the bus.
 */
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

void waitEnter(){
    cin.clear();cin.ignore();
    std::cout << std::endl << "Press enter to continue..." << std::endl;
    std::string str;
    std::getline(std::cin, str);
}

/**
 * Calculates the distance between two given coordinates.
 * @param lat1
 * @param lon1
 * @param lat2
 * @param lon2
 * @return This function returns the distance between two coordinates.
 */
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

/**
 * This function goes through all the sops and tries to find the nearest stop to the
 * coordinate given.
 * @param lat
 * @param log
 * @param stops (vector with all the stops)
 * @return This function return the code of the closest stop to the coordinates given.
 */
string findClosestStop(double lat, double log, const vector<Stop>& stops){
    string closestStop = "";
    double dist = INT64_MAX;
    for (int i = 0; i < stops.size() - 1; i++){
        if (haversine(lat,log,stod(stops[i].latitude), stod(stops[i].longitude)) < dist) {
            dist = haversine(lat,log,stod(stops[i].latitude), stod(stops[i].longitude));
            closestStop = stops[i].code;
        }
    }
    return closestStop;
}

/**
 * This function asks to the user to input a starting point and an end point.
 * The user can give the code of the stop or the coordinates of the respective
 * stop or his coordinates.
 * @param type (Format type that the user wrote. 1 -> coordinates ; 2 -> stops code)
 * @param stops (vector with all the stops)
 * @param start (The starting point)
 * @param end (The end point)
 */
void getStartEndPoints(int type, const vector<Stop> &stops, string &start, string &end){
    CLEAR_MACRO();
    cout << "-----INPUT-----\n\n";
    if (type == 1){
        double startLat, startLong, endLat, endLong;
        cout << "START POINT :" << endl;
        cout << "  - Latitude -> "; cin >> startLat;
        cout << endl;
        cout << "  - Longitude -> "; cin >> startLong;
        cout << endl;
        start = findClosestStop(startLat, startLong, stops);
        cout << "Starting Stop : " << start << endl;
        cout << endl << endl;

        cout << "END POINT :" << endl;
        cout << "  - Latitude -> "; cin >> endLat;
        cout << endl;
        cout << "  - Longitude -> "; cin >> endLong;
        cout << endl;
        end = findClosestStop(endLat, endLong, stops);
        cout << "Ending Stop : " << end << endl;
        cout << endl << endl;
    }
    else if (type == 2){
        cout << "START POINT :" << endl;
        cout << "  - Stop Code -> "; cin >> start;
        for (auto &c : start) c = toupper(c);
        cout << endl << endl;

        cout << "END POINT :" << endl;
        cout << "  - Stop Code -> "; cin >> end;
        for (auto &c : end) c = toupper(c);
        cout << endl << endl;
    }
}

/**
 * This function asks to the user to choose hat type of path he wants (for example,
 * with less stops, less distance)
 */
int getSearchMethod(){
    int searchMethod;
    while (true) {
        cout << "SEARCH METHOD :\n\n";
        cout << "1 - Less Distance\n";
        cout << "2 - Less Stops\n";
        cout << "0 - Return\n";
        cout << endl << "Search Method : ";
        cin >> searchMethod;
        if (cin.fail()) {
            cout << "Invalid Input !" << endl;
            cin.clear();
            cin.ignore(9999, '\n');
            searchMethod = -1;
        }
        else{
            switch (searchMethod) {
                case 1 : return 1;
                case 2 : return 2;
                case 0 : return 0;
                default : break;
            }
        }
    }
}

/**
 * This function presents to the user the Main Menu, asking what type of input he wants
 * to introduce (coordinates of a place or the code of a stop)
 * @param stops
 * @param g (graph with all the stops and distances)
 * @param stopsIndex (vector with the index of all stops)
 */
void mainMenu(const vector<Stop> &stops, Graph &g, map<string,int> &stopsIndex){
    int opt;
    while (true){
        CLEAR_MACRO();
        cout << "\tMAIN MENU\n\n";
        cout << "1 - INPUT : LATITUDE/LONGITUDE\n";
        cout << "2 - INPUT : STOP_CODE\n";
        cout << "3 - GLOBAL NETWORK MST\n";
        cout << "0 - EXIT\n";
        cout << endl << "Option : ";
        cin >> opt;
        if (cin.fail()) {
            cout << "Invalid Input !" << endl;
            cin.clear();
            cin.ignore(9999, '\n');
            opt = -1;
        }
        else {
            string startStop, endStop;
            list<int> path;
            list<string> pathLine;
            int searchMethod = 0;
            switch (opt) {
                case 1 :
                    getStartEndPoints(1, stops, startStop, endStop);
                    break;
                case 2 :
                    getStartEndPoints(2, stops, startStop, endStop);
                    break;
                case 3:
                    cout << "GLOBAL NETWORK MST = " << g.kruskal() << " meters\n";
                    searchMethod = -1;
                    waitEnter();
                    break;
                case 0 :
                    exit(0);
                default :
                    cout << "Invalid Input !" << endl;
                    searchMethod = -1;
                    break;
            }
            if (searchMethod != -1) {
                while (true) {
                    searchMethod = getSearchMethod();
                    if (searchMethod == 0) break;
                    else if (searchMethod == 1) {
                        path = g.dijkstra_path(stopsIndex[startStop], stopsIndex[endStop]);
                        pathLine = g.dijkstra_pathLines(stopsIndex[startStop], stopsIndex[endStop]);
                    } else if (searchMethod == 2) {
                        path = g.bfs_path(stopsIndex[startStop], stopsIndex[endStop]);
                        pathLine = g.bfs_pathLines(stopsIndex[startStop], stopsIndex[endStop]);
                    }
                    //printing the path for the trip :  ... falta oraganizar a separaçao das colunas ...
                    int i = 1;
                    cout
                            << " STEP NUMBER  ||  LINE  ||  STOP CODE  ||             STOP NAME            ||     LATITUDE     ||     LONGITUDE  "
                            << endl;
                    for (auto &p: path) {
                        cout
                                << "===================================================================================================================="
                                << endl;
                        cout << setw(8) << i << setw(8) << "||" << setw(5) << *pathLine.begin() << setw(5) << "||"
                             << setw(8) << stops[p - 1].code << setw(7) << "||" << setw(26) << stops[p - 1].name
                             << setw(10)
                             << "||" << setw(15) << stops[p - 1].latitude << setw(5) << "||" << setw(15)
                             << stops[p - 1].longitude << setw(5) << endl;
                        i++;
                        pathLine.pop_front();
                    }
                }
            }
        }
    }
}

int main() {
    map<string, int> stopsIndex;
    vector<Stop> stops = readStopsFile("dataset\\stops.csv", stopsIndex);
    vector<Line> allLines = readLinesFile("dataset\\lines.csv");
    allLines.erase(allLines.begin() + allLines.size() - 1);

    readLineStops(allLines);

    Graph directedGraph(stopsIndex.size(), true);
    Graph undirectedGraph(stopsIndex.size(), false);

    for (int j = 0; j < allLines.size(); j++){
        for (int i = 0; i < allLines[j].stopsDir0.size() - 1; i++) {
            directedGraph.addEdge(stopsIndex[allLines[j].stopsDir0[i]], stopsIndex[allLines[j].stopsDir0[i + 1]],
                                  haversine(stod(stops[stopsIndex[allLines[j].stopsDir0[i]] - 1].latitude),
                                            stod(stops[stopsIndex[allLines[j].stopsDir0[i]] - 1].longitude),
                                            stod(stops[stopsIndex[allLines[j].stopsDir0[i + 1]] - 1].latitude),
                                            stod(stops[stopsIndex[allLines[j].stopsDir0[i + 1]] - 1].longitude)),
                                  allLines[j].code);
            undirectedGraph.addEdge(stopsIndex[allLines[j].stopsDir0[i]], stopsIndex[allLines[j].stopsDir0[i + 1]],
                                    haversine(stod(stops[stopsIndex[allLines[j].stopsDir0[i]] - 1].latitude),
                                              stod(stops[stopsIndex[allLines[j].stopsDir0[i]] - 1].longitude),
                                              stod(stops[stopsIndex[allLines[j].stopsDir0[i + 1]] - 1].latitude),
                                              stod(stops[stopsIndex[allLines[j].stopsDir0[i + 1]] - 1].longitude)),
                                    allLines[j].code);
        }
        if (allLines[j].stopsDir1.size() != 0) {
            for (int i = 0; i < allLines[j].stopsDir1.size() - 1; i++) {
                directedGraph.addEdge(stopsIndex[allLines[j].stopsDir1[i]], stopsIndex[allLines[j].stopsDir1[i + 1]],
                                      haversine(stod(stops[stopsIndex[allLines[j].stopsDir1[i]] - 1].latitude),
                                                stod(stops[stopsIndex[allLines[j].stopsDir1[i]] - 1].longitude),
                                                stod(stops[stopsIndex[allLines[j].stopsDir1[i + 1]] - 1].latitude),
                                                stod(stops[stopsIndex[allLines[j].stopsDir1[i + 1]] - 1].longitude)),
                                      allLines[j].code);
                undirectedGraph.addEdge(stopsIndex[allLines[j].stopsDir1[i]], stopsIndex[allLines[j].stopsDir1[i + 1]],
                                        haversine(stod(stops[stopsIndex[allLines[j].stopsDir1[i]] - 1].latitude),
                                                  stod(stops[stopsIndex[allLines[j].stopsDir1[i]] - 1].longitude),
                                                  stod(stops[stopsIndex[allLines[j].stopsDir1[i + 1]] - 1].latitude),
                                                  stod(stops[stopsIndex[allLines[j].stopsDir1[i + 1]] - 1].longitude)),
                                        allLines[j].code);
            }
        }
    }

    mainMenu(stops, directedGraph, stopsIndex);
    return 0;
}
