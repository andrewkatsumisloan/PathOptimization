#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include "provided.h" 
using namespace std;

void build_loop(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, vector<DeliveryRequest>& v);
double loop_distance(const vector<DeliveryRequest>& v);
void swap_waypoints(vector<DeliveryRequest>& stopplan, const int a, const  int b);
string turn_direction(double angle);
string compass_direction(double angle);

#endif
