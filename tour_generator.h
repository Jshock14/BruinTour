//
//  tour_generator.h
//  CS32 Project 4
//
//  Created by Jason Schacher on 3/13/24.
//

#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"
#include "router.h"
#include "geodb.h"
#include <string>
using namespace std;

class TourGenerator: public TourGeneratorBase
{
public:
 TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
 virtual ~TourGenerator();
 virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const GeoDatabaseBase* m_geodb;
    const RouterBase* m_router;
    string getDirection(const GeoPoint& pt1, const GeoPoint& pt2) const;
};

#endif /* tour_generator_h */
