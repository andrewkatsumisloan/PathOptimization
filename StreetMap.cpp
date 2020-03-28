#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include "ExpandableHashMap.h"

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
};

StreetMapImpl::StreetMapImpl():m_map()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream in(mapFile);
    if (!in) // If can't open file.
    {
        cerr << "Failed to load map data." << endl;
        return false;
    }
    
    string line; // String that will temporarily store coordinate text.
    while (getline(in, line))
    {
        string streetName = line;
        getline(in, line);
        string s_numSeg = line;
        int numSeg = stod(s_numSeg);
//        cerr << "Street name: " << streetName << " – Number of segments in this street = " <<numSeg << endl;
        for (int i = 0; i < numSeg; i++)
        {
            getline(in, line);
//          double x1, y1, x2, y2;
            string x1, x2, y1, y2;
            
            stringstream ss;
            ss << line;
            ss >> x1;
            ss >> y1;
            ss >> x2;
            ss >> y2;
            GeoCoord a(x1, y1);
            GeoCoord b(x2, y2);
            StreetSegment c(a, b, streetName);
            StreetSegment d(b, a, streetName);
            vector<StreetSegment> *x = m_map.find(a);
            if( x == nullptr)
            {
                vector<StreetSegment> v;
                v.push_back(c);
                m_map.associate(a, v);
            }
            else
            {
                x->push_back(c);
            }
            x = m_map.find(b);
            if( x == nullptr)
            {
                vector<StreetSegment> v;
                v.push_back(d);
                m_map.associate(b, v);
            }
            else
            {
                x->push_back(d);
            }
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment>* result = m_map.find(gc);
    if(result == nullptr)
        return false;
    segs = *result;
    return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
