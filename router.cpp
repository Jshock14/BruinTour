//
//  router.cpp
//  CS32 Project 4
//
//  Created by Jason Schacher on 3/13/24.
//

#include "router.h"
#include "geotools.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include "geopoint.h"
#include <unordered_set>
using namespace std;


Router::Router(const GeoDatabaseBase& geo_db) : m_geodb(&geo_db) {}

Router::~Router() {}

double Router::heuristic(const GeoPoint& start, const GeoPoint& end) const {
    return distance_earth_miles(start, end);
}

// Build the path from start to current
vector<GeoPoint> Router::reconstruct_path(HashMap<GeoPoint>& cameFrom, GeoPoint current) const {
    vector<GeoPoint> path;
    path.push_back(current);

    while (cameFrom.find(current.to_string()) != nullptr) {
        current = cameFrom[current.to_string()];
        path.insert(path.begin(), current);
    }

    return path;
}

vector<GeoPoint> Router::route(const GeoPoint& start, const GeoPoint& goal) const {
    HashMap<GeoPoint> cameFrom;                  // tracks the best previous node
    HashMap<pair<double, double>> scores;        // first = gScore, second = fScore

    scores[start.to_string()].first = 0;                                   // gScore[start] = 0
    scores[start.to_string()].second = heuristic(start, goal);             // fScore[start] = heuristic

    auto cmp = [&scores](const GeoPoint& a, const GeoPoint& b) {
        return scores[a.to_string()].second > scores[b.to_string()].second;
    };

    priority_queue<GeoPoint, vector<GeoPoint>, decltype(cmp)> openSet(cmp);
    openSet.push(start);

    unordered_set<string> closedSet;

    while (!openSet.empty()) {
        GeoPoint current = openSet.top();
        openSet.pop();

        // Check if goal reached
        if (current.to_string() == goal.to_string())
            return reconstruct_path(cameFrom, current);

        closedSet.insert(current.to_string());

        // Examine neighbors
        for (const GeoPoint& neighbor : m_geodb->get_connected_points(current)) {
            if (closedSet.find(neighbor.to_string()) != closedSet.end())
                continue; // already evaluated

            double tentative_gScore = scores[current.to_string()].first + distance_earth_miles(current, neighbor);

            bool not_in_scores = (scores.find(neighbor.to_string()) == nullptr);
            if (not_in_scores || tentative_gScore < scores[neighbor.to_string()].first) {
                cameFrom[neighbor.to_string()] = current;
                scores[neighbor.to_string()].first = tentative_gScore;
                scores[neighbor.to_string()].second = tentative_gScore + heuristic(neighbor, goal);

                openSet.push(neighbor);
            }
        }
    }

    // If goal was never reached
    return vector<GeoPoint>();
}


