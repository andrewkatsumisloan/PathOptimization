#include <vector>
#include <random>
#include <stdlib.h>

#include "provided.h"
#include "support.h"

using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}
// Find a mileage-efficient order for the deliveries.
// This just randomly shuffles the deliveries and checks to see if the 
// mileage is reduced. If it is, overwrite the input delivery order.

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    vector<DeliveryRequest> pts;
    build_loop(depot, deliveries, pts);
    oldCrowDistance = loop_distance(pts);
    vector<DeliveryRequest> improved;
    cerr << "original total crow distance: " << oldCrowDistance << endl;
    newCrowDistance = 10000000000.0;
    int range = deliveries.size();
    int tries = 0;
    while ( oldCrowDistance <= newCrowDistance && tries < range)
    {
        int a = (rand() % range) + 1;
        int b = (rand() % range) + 1;
        swap_waypoints(pts, a, b);
        newCrowDistance = loop_distance(pts);
        tries++;
    }
    if(newCrowDistance < oldCrowDistance)
    {
        for(int i = 0; i < deliveries.size(); i++)
        {
            deliveries[i] = pts[i+1]; 
        }
    }
    cerr << "new total crow distance: " << newCrowDistance << endl;
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
