#include "support.h"
#include <fstream>
#include <iomanip>
#include <vector>

// This makes a vector of DeliveryRequest whose first and last
// entry are the depot
void build_loop(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, vector<DeliveryRequest>& v)
{
    v.clear();
    v.push_back(DeliveryRequest("depot", depot));
    for(int i = 0; i< deliveries.size(); i++)
    {
        v.push_back(deliveries[i]);
    }
    v.push_back(DeliveryRequest("depot", depot));
}

// Given a suitably constructed vector of DeliveryRequest
// measure the "crow" distance of the complete circuit. 
double loop_distance(const vector<DeliveryRequest>& v)
{
    double dist = 0.0;
    for(int i = 0; i < v.size()-1; i++)
    {
        dist+=distanceEarthMiles(v[i].location, v[i+1].location);
    }
    return dist;
}

// Given two unique indices into a vector of DeliveryRequest, if neither of the indices point to 
// the first or last elements, swap the elements.
void swap_waypoints(vector<DeliveryRequest>& stopplan, const int a, const  int b)
{
    if(a > 0 && a < stopplan.size()-1 &&
        b> 0 && b< stopplan.size()-1 && a != b)
    {
        DeliveryRequest dr = stopplan[b];
        stopplan[b] = stopplan[a];
        stopplan[a] = dr;
    }
}

// According to the specification, given an angle in degrees,
// compute a textual turn direction (left, right or straight)
string turn_direction(double angle)
{
    if(angle < 1.0 || angle > 359.0)
    {
        return "straight";
    }
    else if(angle >=1.0 && angle < 180)
    {
        return "left";
    }
    else if(angle >= 180.0 && angle < 359.0)
    {
        return "right";
    }
    return "straight";
}

// According to the specification, given an angle in degrees,
// compute a compass direction
string compass_direction(double angle)
{
    if(0 <= angle && angle < 22.5)
        return "east";
    else if(22.5  <= angle && angle < 67.5)
        return "northeast";
    else if(67.5 <=angle && angle < 112.5)
        return "north";
    else if(112.5<= angle && angle < 157.5)
        return "northwest";
    else if(157.5<=angle && angle< 202.5)
        return "west";
    else if(202.5<=angle && angle < 247.5)
        return "southwest";
    else if(247.5<=angle && angle < 292.5)
        return "south";
    else if(292.5<=angle && angle <337.5)
        return "southeast";
    else if( angle>=337.5)
        return "east";
    return "east";
}


