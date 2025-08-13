//
//  router.h
//  CS32 Project 4
//
//  Created by Jason Schacher on 3/13/24.
//

#ifndef router_h
#define router_h

#include "base_classes.h"
#include "geodb.h"
#include "hashmap.h"

class Router: public RouterBase
{
public:
 Router(const GeoDatabaseBase& geo_db);
 virtual ~Router();
 virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    const GeoDatabaseBase* m_geodb;
    double heuristic(const GeoPoint& start, const GeoPoint& end) const;
    vector<GeoPoint> reconstruct_path(HashMap<GeoPoint>& cameFrom, GeoPoint current) const;

};

#endif /* router_h */
