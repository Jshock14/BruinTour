//
//  geodb.h
//  CS32 Project 4
//
//  Created by Jason Schacher on 3/12/24.
//

#ifndef geodb_h
#define geodb_h

#include "hashmap.h"
#include "geopoint.h"
#include "base_classes.h"
#include <vector>
#include <string>
using namespace std;


class GeoDatabase: public GeoDatabaseBase
{
public:
 GeoDatabase();
 virtual ~GeoDatabase();
 virtual bool load(const std::string& map_data_file);
 virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
 virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
 virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    HashMap<GeoPoint> poiToPt;
    HashMap<vector<GeoPoint>> ptToConnected;
    HashMap<string> ptsToRoad;
    void associate(string name, const GeoPoint& pt1, const GeoPoint& pt2);
};

#endif /* geodb_h */
