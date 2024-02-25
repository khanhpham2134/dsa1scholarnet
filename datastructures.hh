// Datastructures.hh
//
// Student name: Minh Khanh Pham
// Student email: minhkhanh.pham@tuni.fi
// Student number: 151377446

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <unordered_set>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
struct Connection;
// Type for a distance (in arbitrary units)
using Distance = int;

using Path = std::vector<Connection>;
using PathWithDist = std::vector<std::pair<Connection,Distance>>;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

struct Connection
{
    AffiliationID aff1 = NO_AFFILIATION;
    AffiliationID aff2 = NO_AFFILIATION;
    Weight weight = NO_WEIGHT;
    bool operator==(const Connection& c1) const{
        return (aff1==c1.aff1) && (aff2==c1.aff2) && (weight==c1.weight);
    }

};
const Connection NO_CONNECTION{NO_AFFILIATION,NO_AFFILIATION,NO_WEIGHT};

struct ConnectionHash
{
    // Use std::hash to hash each member of Connection
    std::size_t operator() (const Connection& c) const
    {
        // Combine the hashes of aff1, aff2, and weight using XOR
        return std::hash<AffiliationID>()(c.aff1) ^
               std::hash<AffiliationID>()(c.aff2) ^
               std::hash<Weight>()(c.weight);
    }
};

struct Affiliation
{
    AffiliationID id;
    Name name;
    Coord location;
    std::vector<PublicationID> publications;
};

struct Publication
{
    PublicationID id;
    Name name;
    Year year;
    std::vector<AffiliationID> affiliations;
    std::vector<PublicationID> referencing;
    PublicationID referenced_by;
};


// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Return the value of the vector's size.
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear() function has constant time complexity and it is called n times
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::transform has a time complexity of O(n)
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: count() function has a constant time complexity
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort has a time complexity of O(n log n)
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort has a time complexity of O(n log n)
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop has a constant time complexity and it is called n times
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop has a constant time complexity and it is called n times
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: find() function has a constant time complexity
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find() function has a constant time complexity
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() function has a constant time complexity and it is called n times
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find() function has a constant time complexity
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort is logarithmic and it is called n times
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The time complexity of the while loop is O(n), where n is the number of publications in the chain.
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: The time complexity of the breadth-first search algorithm is O(n), where n is the number of publications in the chain.
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort has a time complexity of O(nlogn)
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: the function searches each map for the affiliation using a linear search
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: using two for loops, each has linear time complexity
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: the function searches each map for the publication using a linear search. If the maps are large, this could result in slow performance.
    bool remove_publication(PublicationID publicationid);


    // PRG 2 functions:

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: the function loops through all pairs of affiliations, and for each pair, it loops through all publications of both affiliations.
    std::vector<Connection> get_connected_affiliations(AffiliationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: the function loops through all pairs of affiliations twice, and for each pair, it loops through all publications of both affiliations twice.
    std::vector<Connection> get_all_connections();

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: the function uses breadth-first-search
    Path get_any_path(AffiliationID source, AffiliationID target);

    // PRG2 optional functions

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_with_least_affiliations(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_of_least_friction(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    PathWithDist get_shortest_path(AffiliationID source, AffiliationID target);


private:
    std::unordered_map<AffiliationID, Affiliation> affiliations_;
    std::unordered_map<PublicationID, Publication> publications_;
    std::vector<std::pair<Name, AffiliationID>> name_affiliation;
    std::map<Coord, AffiliationID> coord_affiliation;
    std::vector<std::pair<double, AffiliationID>> distance_affiliation;
    bool sorted_name = false;
    bool sorted_distance = false;
    std::map<std::pair<AffiliationID, AffiliationID>, Connection> connections;

};

#endif // DATASTRUCTURES_HH
