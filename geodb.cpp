//
//  geodb.cpp
//  CS32 Project 4
//
//  Created by Jason Schacher on 3/12/24.
//

#include "geodb.h"
#include <sstream>
#include "geotools.h"

GeoDatabase::GeoDatabase() {}

GeoDatabase::~GeoDatabase() {}

bool GeoDatabase::load(const std::string& map_data_file) {
    ifstream infile(map_data_file);
    if (!infile) {
        return false;
    }
    string line;
    string streetName;
    while (getline(infile, line)) {
        istringstream iss(line);
        string lat1, lon1, lat2, lon2;
        bool hasName = false;
        if (!hasName) {
            streetName = line;
            hasName = true;
        }
        else {
            hasName = false;
            iss >> lat1 >> lon1 >> lat2 >> lon2;
            GeoPoint pt1(lat1, lon1);
            GeoPoint pt2(lat2, lon2);
            associate(streetName, pt1, pt2);
            int numPois = 0;
            infile >> numPois;
            for (int i = 0; i < numPois; i++) {
                string poiName;
                string poiLat, poiLon;
                getline(infile, line);
                poiName = line.substr(0, line.find("|"));
                poiLat = line.substr(line.find("|") + 1, line.find(" "));
                poiLon = line.substr(line.find(" ") + 1);
                GeoPoint poi(poiLat, poiLon);
                poiToPt.insert(poiName, poi);
                GeoPoint midPt = midpoint(pt1, pt2);
                associate(streetName, pt1, midPt);
                associate(streetName, pt2, midPt);
                associate("a path", poi, midPt);
            }
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const {
    const GeoPoint* location = poiToPt.find(poi);
    if (location == nullptr) {
        return false;
    }
    else {
        point = *location;
        return true;
    }
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    const vector<GeoPoint>* connected = ptToConnected.find(pt.to_string());
    if (connected == nullptr) {
        return vector<GeoPoint>();
    }
    else {
        return *connected;
    }
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
    const string* roadName = ptsToRoad.find(pt1.to_string() + "*" + pt2.to_string());
    if (roadName == nullptr) {
        return "";
    }
    else {
        return *roadName;
    }
}

void GeoDatabase::associate(string name, const GeoPoint& pt1, const GeoPoint& pt2) {
    ptToConnected[pt1.to_string()].push_back(pt2);
    ptToConnected[pt2.to_string()].push_back(pt1);
    ptsToRoad.insert(pt1.to_string() + "*" + pt2.to_string(), name);
    ptsToRoad.insert(pt2.to_string() + "*" + pt1.to_string(), name);
}

