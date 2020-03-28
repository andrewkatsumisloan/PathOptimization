// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

#include <list>
#include <vector>

using namespace std;


template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    int m_occupied;
    double m_maxload;
    int m_associations;
    // m_buckets is a vector of list pointers that contain pairs holding KeyType and ValueType pointers
    vector< list<pair <const KeyType*, const ValueType*> > *> m_buckets;
};

// This is just the constructor for the ExpandableHashMap class, which takes just the load factor.  WE ARE IMPLEMENTING AN OPEN HASH TABLE.
// Seems to clearly imply that the data structure to use is a Hash map
// A newly constructed ExpandableHashMap must have 8 buckets and no associations
// Default maximumLoadFactor is 0.5
// There must be at most one mapping for a unique key, if there is a second association, it must overwrite the previous one.
// The Big-O of inserting an item in your hash map must be O(1) in the average case, with the understanding that some insertions may result in an O(N) insertion time should the hash table need to be resized, where N is the number of associations in the table.
// The Big-O of searching for an item in your hash map must be O(1) in the average case.
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor): m_buckets(8), m_associations(0)
{
    m_maxload = maximumLoadFactor;
    for (int i = 0; i < m_buckets.size(); i++)
    {
        m_buckets[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    this->reset();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    // Reset HashMap bucket to 8
    // Deletes all items
    
    for (int i = 0; i < m_buckets.size(); i++)
    {
         if(m_buckets[i] != nullptr)
         {
             typename list< pair<const KeyType*, const ValueType*>>::const_iterator li = m_buckets[i]->begin();
             while (li != m_buckets[i]->end())
             {
                 //cerr << "associate: deleting " << *(li->first) << endl;
                 delete li->first;
                 delete li->second;
                 li++;
             }
             delete m_buckets[i];
         }
    }
    m_occupied = 0;
    m_associations = 0;
    m_buckets.clear();
    m_buckets.resize(8);
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_associations;
}


// Associate is a function that adds a pair of keys ( ) and values ( ) to the hash map.
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hasher(const KeyType&);
    // If it m_occupied/num_buckets > maximumLoadFactor, allocate more space
    double sz = m_buckets.size();
    if (m_occupied/sz > m_maxload)
    {
        vector<pair<const KeyType, const ValueType> > temp_bucket;
        for (int u = 0; u < m_buckets.size(); u++)
        {
            
            if (m_buckets[u] == nullptr)
            {
                continue;
            }
            else
            {
                typename list< pair<const KeyType*, const ValueType*>>::const_iterator li = m_buckets[u]->begin();
                while (li != m_buckets[u]->end())
                {

                    temp_bucket.push_back(pair<KeyType, ValueType>(*(li->first), *(li->second)));
                    //cerr << "associate: deleting " << *(li->first) << endl;
                    delete li->first;
                    delete li->second;
                    
                    li++;
                }
            }
        }
        sz = m_buckets.size() * 2;
        m_buckets.clear();
//        cerr << "new bucket size = " << sz << endl;
        m_buckets.resize(sz);
        m_occupied = 0;
        m_associations = 0;
        for (int i = 0; i < temp_bucket.size(); i++)
        {
            associate(temp_bucket[i].first, temp_bucket[i].second);
        }
    }
    int index = hasher(key) % m_buckets.size();
    //cerr << key << " maps to hash " << index << endl;
    if (m_buckets[index] == nullptr)
    {
        m_buckets[index] = new list<pair<const KeyType*,const ValueType*> >;
        m_occupied++;
    }
    else
    {
        typename list< pair<const KeyType*, const ValueType*>>::const_iterator li = m_buckets[index]->begin();
        while (li != m_buckets[index]->end())
        {
            if (key == *(li->first))
            {
                delete li->first;
                delete li->second;
                //cerr << "associate: deleted " << key << endl;
                m_buckets[index]->erase(li);
                m_associations--;
                break;
            }
            li++;
        }
    }
    KeyType* nkey = new KeyType;
    ValueType* nvalue = new ValueType;
    *nkey = key;
    *nvalue = value;
    //cerr << "associate: allocated " << key << endl;
    pair<const KeyType*, const ValueType*> p = make_pair(nkey, nvalue);
    m_buckets[index]->push_front(p);
    m_associations++;
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType&);
    int index = hasher(key) % m_buckets.size();

    if (m_buckets[index] == nullptr)
    {
        return nullptr;
    }
  
    typename list< pair<const KeyType*, const ValueType*>>::const_iterator li = m_buckets[index]->begin();
    while (li != m_buckets[index]->end())
    {
        if (*(li->first) == key)
        {
            return li->second;
        }
        li++;
    }
    
    return nullptr;
}

