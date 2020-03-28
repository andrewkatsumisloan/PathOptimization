#include "provided.h"
#include <vector>
#include "support.h"

using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
    
private:
    PointToPointRouter m_ppr;
    const StreetMap *m_sm;    
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm):m_ppr(sm), m_sm(sm)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    vector<DeliveryRequest> m_deliveries = deliveries;
    DeliveryOptimizer dopt(m_sm);
    double old_dist;
    double new_dist;
    dopt.optimizeDeliveryOrder(depot, m_deliveries, old_dist, new_dist);

    totalDistanceTravelled = 0.0;
    GeoCoord current_start = depot;
    GeoCoord current_end = depot;
    list<StreetSegment> lss;
    double incrdist = 0.0;
    
    for(int i=0; i<m_deliveries.size()+1; i++)
    {
        lss.clear();
        current_end = i >= m_deliveries.size()? depot : m_deliveries[i].location;
        m_ppr.generatePointToPointRoute(current_start, current_end, lss, incrdist);
        list<StreetSegment>::iterator li = lss.begin();
        list<StreetSegment>::iterator lasti = li;
        string streetname = li->name;
        while(li != lss.end())
        {
            DeliveryCommand dc;
            if(commands.size() == 0 || li == lss.begin() )
            {
                dc.initAsProceedCommand(compass_direction(angleOfLine(*li)), li->name, 0.0);
                commands.push_back(dc);
            }
            else if(li->name != streetname)
            {
                string turn_dir = turn_direction(angleBetween2Lines(*lasti, *li) );
                if(turn_dir != "straight")
                {
                    DeliveryCommand t;
                    t.initAsTurnCommand(turn_dir, li->name);
                    commands.push_back(t);
                }
                dc.initAsProceedCommand(compass_direction(angleOfLine(*li)), li->name, 0.0);
                                        
                commands.push_back(dc);
                streetname = li->name;
            }
            commands.back().increaseDistance(distanceEarthMiles(li->start, li->end));
            lasti = li;
            li++;
        }
        DeliveryCommand deliv;
        if (i < m_deliveries.size())
        {
            deliv.initAsDeliverCommand(m_deliveries[i].item);
            commands.push_back(deliv);
        }
        current_start = current_end;
        totalDistanceTravelled += incrdist;
    }

    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
