#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>


using namespace std;

struct Node {       //Code,Name,Zone,Latitude,Longitude
    string code;
    string name;
    string zone;
    string latitude;
    string longitude;
};

bool readStopsFile(const string &filename, vector<Node>& stops) {
    ifstream stopsFile(filename);
    if (!stopsFile.is_open() || stopsFile.eof())
        return false;
    string title;
    getline(stopsFile, title);
    while (!stopsFile.eof()){

        Node stop;
        getline(stopsFile, stop.code, ',');
        getline(stopsFile, stop.name, ',');
        getline(stopsFile, stop.zone, ',');
        getline(stopsFile, stop.latitude, ',');
        getline(stopsFile, stop.longitude);


        stops.push_back(stop);
    }
    return true;
}

int main() {
    vector<Node> stops; //replace vector with graph!

    if (!readStopsFile("dataset\\stops.csv", stops))
        return 1;

    //printing the stops :
    for (auto &s : stops)
        cout << s.code << " -- " << s.name << " -- " << s.zone << " -- " << s.latitude << " -- " << s.longitude << endl;
    //-------

    return 0;
}
