#include "provided.h"
#include <list>
#include <queue>
#include "ExpandableHashMap.h"

using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_sm;
    
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm): m_sm(sm)
{
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    totalDistanceTravelled = 0;
    if (start == end)
    {
        return DELIVERY_SUCCESS;
    }
    ExpandableHashMap<GeoCoord, GeoCoord> m_visited;
    queue<GeoCoord> geoQ;
    
    vector<StreetSegment> segs;
    geoQ.push(start);
    m_visited.associate(start, start);
    while (!geoQ.empty())
    {
        GeoCoord a = geoQ.front();
        geoQ.pop();
        if (a == end)
            break;
 
        m_sm->getSegmentsThatStartWith(a, segs);
        vector<StreetSegment>::iterator vi = segs.begin();
        while (vi != segs.end())
        {
            if (m_visited.find(vi->end) == nullptr)
            {
                geoQ.push(vi->end);
                m_visited.associate(vi->end, vi->start);
            }
            vi++;
        }
        segs.clear();
    }
    GeoCoord *gc = m_visited.find(end);
    GeoCoord nextgc = end;
    if(gc != nullptr)
    {
        route.clear();
        while( *gc != start)
        {
            m_sm->getSegmentsThatStartWith(*gc, segs);
            string name = segs[0].name;
            for(int i=0; i<segs.size(); i++)
            {
                if(segs[i].end == nextgc)
                {
                    name = segs[i].name;
                }
            }
            route.push_front(StreetSegment(*gc, nextgc, name ));
            totalDistanceTravelled += distanceEarthMiles(*gc, nextgc);
            nextgc = *gc;
            gc = m_visited.find(nextgc);
        }
        m_sm->getSegmentsThatStartWith(start, segs);
        string name = segs[0].name;
        for(int i=0; i<segs.size(); i++)
        {
            if(segs[i].end == *gc)
            {
                name = segs[i].name;
            }
        }
        totalDistanceTravelled += distanceEarthMiles(start, nextgc);
        route.push_front(StreetSegment(start, nextgc, name ));
        return DELIVERY_SUCCESS;
    }
    
    return NO_ROUTE;  
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
