// Datastructures.cc
//
// Student name: Minh Khanh Pham
// Student email: minhkhanh.pham@tuni.fi
// Student number: 151377446

#include "datastructures.hh"
#include <random>
#include <cmath>
#include <queue>
#include <set>
#include <map>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations_.size();
}

void Datastructures::clear_all()
{
    affiliations_.clear();
    publications_.clear();
    name_affiliation.clear();
    coord_affiliation.clear();
    distance_affiliation.clear();
    connections.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> allAffiliations;
    allAffiliations.reserve(affiliations_.size());

    for (const auto& pair : affiliations_) {
        allAffiliations.push_back(pair.first);
    }

    return allAffiliations;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name& name, Coord xy)
{
    // Check if the affiliation already exists
    if (affiliations_.count(id) > 0)
    {
        // Affiliation with the same ID already exists
        return false;
    }

    // Create a new Affiliation object
    Affiliation newAffiliation={id, name, xy, {}};

    // Add the new affiliation to the affiliations_ unordered_map
    affiliations_[id] = newAffiliation;

    name_affiliation.push_back(std::make_pair(name,id));
    sorted_name = false;
    coord_affiliation.insert({xy,id});

    double distance = std::sqrt(xy.x * xy.x + xy.y * xy.y);
    distance_affiliation.push_back(std::make_pair(distance, id));
    sorted_distance = false;

    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto it = affiliations_.find(id);

    if (it != affiliations_.end())
    {
        // Affiliation with the given ID found, return its name
        return it->second.name;
    }
    else
    {
        // Affiliation with the given ID not found, return NO_NAME
        return NO_NAME;
    }
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = affiliations_.find(id);

    if (it != affiliations_.end())
    {
        // Affiliation with the given ID found, return its coord
        return it->second.location;
    }
    else
    {
        // Affiliation with the given ID not found, return NO_COORD
        return NO_COORD;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    std::vector<AffiliationID> sortedAffiliations;

    if (!sorted_name) {
            // Sort the affiliations only if they have not been sorted before
            std::sort(name_affiliation.begin(), name_affiliation.end());

            sorted_name = true;
        }

        for (const auto& pair : name_affiliation) {
            sortedAffiliations.push_back(pair.second);
        }

    return sortedAffiliations;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    std::vector<AffiliationID> sortedAffiliations;

    if (!sorted_distance) {
        std::sort(distance_affiliation.begin(),distance_affiliation.end(), [&](const auto& a, const auto& b) {
            if (std::abs(a.first - b.first) < 1e-9) {
                return affiliations_[a.second].location.y < affiliations_[b.second].location.y;
            }
            return a.first < b.first;
        });
        sorted_distance = true;

    }
    for (const auto& pair : distance_affiliation) {
        sortedAffiliations.push_back(pair.second);
    }

    return sortedAffiliations;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto it = coord_affiliation.find(xy);
    if (it != coord_affiliation.end()) {
        return it->second;
    } else {
        return NO_AFFILIATION;
    }
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it = affiliations_.find(id);

    if (it != affiliations_.end())
    {
        // Affiliation with the given ID found, update its location
        it->second.location = newcoord;

        // Update the coordinates in coordIDMap if present
        for (auto it = coord_affiliation.begin(); it != coord_affiliation.end(); ) {
            if (it->second == id) {
                it = coord_affiliation.erase(it); // Remove current entry

                // Insert a new entry with the updated Coord
                coord_affiliation[newcoord] = id;
            } else {
                ++it;
            }
        }


        for (auto& pair : distance_affiliation) {
            if (pair.second == id) {
                double distance = std::sqrt(newcoord.x * newcoord.x + newcoord.y * newcoord.y);
                pair.first = distance;
            }
        }

        sorted_distance = false; // Update the flag to indicate distances are not sorted
        return true;
    }
    return false;
}

bool Datastructures::add_publication(PublicationID id, const Name& name, Year year, const std::vector<AffiliationID>& affiliations)
{
    // Check if the publication with the given ID already exists
    if (publications_.find(id) != publications_.end())
    {
        // Publication with the given ID already exists, return false
        return false;
    }

    // Create a new publication with the provided information
    Publication newPublication={id, name, year, affiliations, {}, NO_PUBLICATION};

    // Add the new publication to the publications_ unordered_map
    publications_[id] = newPublication;

    // Update affiliations_ to include the new publication
    for (const auto& affiliationID : affiliations)
    {
        affiliations_[affiliationID].publications.push_back(id);
    }

    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> allPublicationIDs;

    for (const auto& entry : publications_)
    {
        allPublicationIDs.push_back(entry.first);
    }

    return allPublicationIDs;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = publications_.find(id);

    if (it != publications_.end())
    {
        // Publication with the given ID exists, return its name
        return it->second.name;
    }
    else
    {
        // Publication with the given ID does not exist, return NO_NAME
        return NO_NAME;
    }
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto it = publications_.find(id);

    if (it != publications_.end())
    {
        // Publication with the given ID exists, return its year
        return it->second.year;
    }
    else
    {
        // Publication with the given ID does not exist, return NO_YEAR
        return NO_YEAR;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto it = publications_.find(id);

    if (it != publications_.end())
    {
        // Publication with the given ID exists, return its affiliations
        return it->second.affiliations;
    }
    else
    {
        // Publication with the given ID does not exist, return vector with NO_AFFILIATION
        return {NO_AFFILIATION};
    }
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto it = publications_.find(id);
    auto parent_it = publications_.find(parentid);

    if (it != publications_.end() && parent_it != publications_.end())
    {
        // Add the reference
        parent_it->second.referencing.push_back(id);
        it->second.referenced_by = parentid;
        return true;
    }
    return false;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto it = publications_.find(id);
    if (it == publications_.end()) {
        // Publication with the given ID doesn't exist
        return {NO_PUBLICATION};
    }

    // Return the direct references in any arbitrary order
    return it->second.referencing;
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto affiliation_it = affiliations_.find(affiliationid);
    auto publication_it = publications_.find(publicationid);

    // Check if both affiliation and publication exist
    if (affiliation_it == affiliations_.end() || publication_it == publications_.end()) {
        return false;
    }

    // Add the affiliation to the publication
    publication_it->second.affiliations.push_back(affiliationid);
    affiliation_it->second.publications.push_back(publicationid);

    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    auto affiliation_it = affiliations_.find(id);

    // Check if the affiliation exists
    if (affiliation_it != affiliations_.end())
    {
        // Publication with the given ID exists, return its publications
        return affiliation_it->second.publications;
    }
    else
    {
        // Publication with the given ID does not exist, return vector with NO_PUBLICATION
        return {NO_PUBLICATION};
    }
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto publication_it = publications_.find(id);

    // Check if the publication exists
    if (publication_it == publications_.end()) {
        return NO_PUBLICATION;
    }

    // Get the referencing publications
    PublicationID parentid = publication_it->second.referenced_by;

    // Check if there are any referencing publications
    if (parentid != NO_PUBLICATION && publications_.count(parentid) > 0) {
        return parentid;
    }

    return NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID>> Datastructures::get_publications_after(AffiliationID id, Year year)
{
    auto affiliation_it = affiliations_.find(id);

    // Check if the affiliation exists
    if (affiliation_it == affiliations_.end()) {
        return {{NO_YEAR, NO_PUBLICATION}};
    }

    std::vector<std::pair<Year, PublicationID>> result;
    result.reserve(publications_.size());

    // Iterate through the publications of the affiliation
    for (const PublicationID& publication_id : affiliation_it->second.publications) {
        auto publication_it = publications_.find(publication_id);

        // Check if the publication exists
        if (publication_it != publications_.end() && publication_it->second.year >= year) {
            result.push_back({publication_it->second.year, publication_id});
        }
    }

    // Sort the result based on year and then by publication ID
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return std::tie(a.first, a.second) < std::tie(b.first, b.second);
    });
    result.shrink_to_fit();

    return result;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> chain;
    auto it = publications_.find(id);

    // Check if the publication with the given ID exists
    if (it == publications_.end()) {
        // Return a vector with a single item NO_PUBLICATION
        return {NO_PUBLICATION};
    }

    // Traverse the referencing chain until there is no more parent
    while (it->second.referenced_by != NO_PUBLICATION) {

        // Move to the parent publication
        it = publications_.find(it->second.referenced_by);

        // Check if the parent publication exists
        if (it == publications_.end()) {
            // If the parent publication doesn't exist, break out of the loop
            break;
        }

        // Add the current parent to the vector
        chain.push_back(it->first);
    }
    return chain;
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    std::vector<PublicationID> all_references;

    auto it = publications_.find(id);

    if (it == publications_.end()) {
        return {NO_PUBLICATION};
    }

    std::queue<PublicationID> queue;
        queue.push(id);

        while (!queue.empty()) {
            auto current_id = queue.front();
            queue.pop();

            auto current_it = publications_.find(current_id);
            if (current_it == publications_.end()) {
                // If the current publication doesn't exist, skip it
                continue;
            }

            // Add all referencing publications to the queue for further exploration
            for (auto child_id : current_it->second.referencing) {
                queue.push(child_id);
                // Add the current child to the result vector
                all_references.push_back(child_id);
        }
    }

    return all_references;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    std::vector<AffiliationID> closestAffiliationIDs;

    for (const auto& entry : affiliations_)
    {
        closestAffiliationIDs.push_back(entry.first);
    }

    std::sort(closestAffiliationIDs.begin(), closestAffiliationIDs.end(),
              [this, xy](const AffiliationID& id1, const AffiliationID& id2) {
                  const Affiliation& affiliation1 = affiliations_.at(id1);
                  const Affiliation& affiliation2 = affiliations_.at(id2);

                  const Distance distance1 = sqrt(pow(affiliation1.location.x - xy.x, 2) + pow(affiliation1.location.y - xy.y, 2));
                  const Distance distance2 = sqrt(pow(affiliation2.location.x - xy.x, 2) + pow(affiliation2.location.y - xy.y, 2));

                  if (distance1 == distance2)
                  {
                      return affiliation1.location.y < affiliation2.location.y;
                  }
                  return distance1 < distance2;
              });

    if (closestAffiliationIDs.empty())
    {
        return {};
    }

    if (closestAffiliationIDs.size() < 3)
    {
        return closestAffiliationIDs;
    }

    return {closestAffiliationIDs[0], closestAffiliationIDs[1], closestAffiliationIDs[2]};
}

bool Datastructures::remove_affiliation(AffiliationID id)
{

    auto it = affiliations_.find(id);
    if (it == affiliations_.end())
    {
        return false;
    }

    affiliations_.erase(it);

    // Remove the affiliation from name_affiliation
    auto name_affiliationIt = std::find_if(name_affiliation.begin(), name_affiliation.end(),
        [id](const std::pair<Name, AffiliationID>& pair) { return pair.second == id; });

    if (name_affiliationIt != name_affiliation.end())
    {
        name_affiliation.erase(name_affiliationIt);
    }

    // Remove the affiliation from coord_affiliation
    auto coord_affiliationIt = std::find_if(coord_affiliation.begin(), coord_affiliation.end(),
        [id](const std::pair<Coord, AffiliationID>& pair) { return pair.second == id; });

    if (coord_affiliationIt != coord_affiliation.end())
    {
        coord_affiliation.erase(coord_affiliationIt);
    }

    // Remove the affiliation from distance_affiliation
    auto distance_affiliationIt = std::find_if(distance_affiliation.begin(), distance_affiliation.end(),
        [id](const std::pair<double, AffiliationID>& pair) { return pair.second == id; });

    if (distance_affiliationIt != distance_affiliation.end())
    {
        distance_affiliation.erase(distance_affiliationIt);
    }

    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    auto it1 = publications_.find(id1);
    auto it2 = publications_.find(id2);

    if (it1 == publications_.end() || it2 == publications_.end()) {
        return NO_PUBLICATION;
    }

    // Get the chains of publications that reference id1 and id2
    std::vector<PublicationID> chain1 = get_referenced_by_chain(id1);
    std::vector<PublicationID> chain2 = get_referenced_by_chain(id2);

    PublicationID closest_common_parent = NO_PUBLICATION;
    for (PublicationID parent1 : chain1) {
        for (PublicationID parent2 : chain2) {
            if (parent1 == parent2) {
                closest_common_parent = parent1;
                return closest_common_parent;
            }
        }
    }

    return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto it = publications_.find(publicationid);

    if (it == publications_.end()) {
        return false;
    }

    // Remove the publication from publications_
    publications_.erase(it);

    // Remove the referenced_by to the publication
    for (auto& affiliation : affiliations_) {
        auto& referenced_by = affiliation.second.publications;
        referenced_by.erase(std::remove(referenced_by.begin(), referenced_by.end(), publicationid), referenced_by.end());
    }

    // Remove references
    for (auto& publication : publications_) {

        auto& referencing = publication.second.referencing;
        referencing.erase(std::remove(referencing.begin(), referencing.end(), publicationid), referencing.end());

        if (publication.second.referenced_by == publicationid) {
            publication.second.referenced_by = NO_PUBLICATION;
        }
    }
    return true;
}


std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
    // Create an empty vector to store the connections
    std::vector<Connection> connections;

    // Check if the given ID exists in the affiliations_ map
    if (affiliations_.find(id) == affiliations_.end())
    {
        // If not, return an empty vector
        return connections;
    }

    // Loop through the affiliations_ map
    for (auto& pair : affiliations_)
    {
        // Get the other affiliation ID and object
        AffiliationID other_id = pair.first;
        Affiliation other_aff = pair.second;

        // Skip the given ID
        if (other_id == id)
        {
            continue;
        }

        // Check if there is a connection between the given ID and the other ID
        // Initialize the weight to zero
        Weight weight = 0;

        // Get the publications of the first affiliation
        std::vector<PublicationID> pubs1 = affiliations_[id].publications;

        // Get the publications of the second affiliation
        std::vector<PublicationID> pubs2 = affiliations_[other_id].publications;

        // Loop through the publications of the first affiliation
        for (PublicationID pub1 : pubs1)
        {
            // Loop through the publications of the second affiliation
            for (PublicationID pub2 : pubs2)
            {
                // If the publications are the same, increment the weight
                if (pub1 == pub2)
                {
                    weight++;
                }
            }
        }

        // If the weight is not NO_WEIGHT, there is a connection
        if (weight != NO_WEIGHT && weight != 0)
        {
            // Create a Connection object with the given ID, the other ID, and the weight
            Connection connection{id, other_id, weight};

            // Push the connection to the vector
            connections.push_back(connection);
        }
    }

    // Return the vector of connections
    return connections;
}

std::vector<Connection> Datastructures::get_all_connections()
{

    // Loop through the affiliations_ map
    for (auto& pair1 : affiliations_)
    {
        // Get the first affiliation ID and object
        AffiliationID id1 = pair1.first;
        Affiliation aff1 = pair1.second;

        // Loop through the affiliations_ map again
        for (auto& pair2 : affiliations_)
        {
            // Get the second affiliation ID and object
            AffiliationID id2 = pair2.first;
            Affiliation aff2 = pair2.second;

            // Skip the same ID
            if (id1 == id2)
            {
                continue;
            }

            // Check if there is a connection between the two IDs
            Weight weight = 0;

            // Get the publications of the first affiliation
            std::vector<PublicationID> pubs1 = affiliations_[id1].publications;

            // Get the publications of the second affiliation
            std::vector<PublicationID> pubs2 = affiliations_[id2].publications;

            // Loop through the publications of the first affiliation
            for (PublicationID pub1 : pubs1)
            {
                // Loop through the publications of the second affiliation
                for (PublicationID pub2 : pubs2)
                {
                    // If the publications are the same, increment the weight
                    if (pub1 == pub2)
                    {
                        weight++;
                    }
                }
            }

            // If the weight is not NO_WEIGHT, there is a connection
            if (weight != NO_WEIGHT && weight != 0)
            {
                // Create a Connection object with the smaller ID in aff1 and the larger ID in aff2
                Connection connection{std::min(id1, id2), std::max(id1, id2), weight};

                // Create a pair of IDs as the key for the map
                std::pair<AffiliationID, AffiliationID> key{std::min(id1, id2), std::max(id1, id2)};

                // Check if the map already has a connection for this pair
                if (connections.count(key) > 0)
                {
                    // Get the existing connection
                    Connection existing = connections[key];

                    // Compare the weights and update the map with the better connection
                    if (connection.weight > existing.weight)
                    {
                        connections[key] = connection;
                    }
                }
                else
                {
                    // Insert the connection to the map
                    connections[key] = connection;
                }
            }
        }
    }

    // Create an empty vector to store the connections
    std::vector<Connection> all_connections;

    // Loop through the map and push the connections to the vector
    for (auto& pair : connections)
    {
        all_connections.push_back(pair.second);
    }

    // Return the vector of connections
    return all_connections;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    // Create an empty vector to store the path
    Path path;

    // Check if the source and target IDs exist in the affiliations_ map
    if (affiliations_.count(source) == 0 || affiliations_.count(target) == 0)
    {
        // If not, return an empty vector
        return path;
    }

    // Create a set to store the visited affiliations
    std::set<AffiliationID> visited;

    // Create a queue to store the current affiliation and the path so far
    std::queue<std::pair<AffiliationID, Path>> queue;

    // Push the source ID and an empty path to the queue
    queue.push({source, path});

    // Mark the source ID as visited
    visited.insert(source);

    // Loop until the queue is empty or the target is found
    while (!queue.empty())
    {
        // Get the front element of the queue and pop it
        auto front = queue.front();
        queue.pop();

        // Get the current affiliation ID and the path so far
        AffiliationID current = front.first;
        Path current_path = front.second;

        // Check if the current ID is the target ID
        if (current == target)
        {
            // If yes, return the current path
            return current_path;
        }

        // Get the connections of the current affiliation
        std::vector<Connection> connections = get_connected_affiliations(current);

        // Loop through the connections
        for (const Connection &connection : connections)
        {
            // Get the other affiliation ID
            AffiliationID other = (connection.aff1 == current) ? connection.aff2 : connection.aff1;

            // Check if the other ID is not visited
            if (visited.count(other) == 0)
            {
                // If not, mark it as visited
                visited.insert(other);

                // Create a new path by appending the connection to the current path
                Path new_path = current_path;
                new_path.push_back(connection);

                // Push the other ID and the new path to the queue
                queue.push({other, new_path});
            }
        }
    }

    // If the target is not found, return an empty vector
    return path;
}

Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{

    // Use your get_any_path function to get an arbitrary path
    Path any_path = get_any_path(source, target);

    // Initialize variables to keep track of the minimum affiliations and the corresponding path
    size_t min_affiliations = affiliations_.size(); // initialize with a large value
    Path min_affiliations_path;

    // Iterate through connections in the path
    for (size_t i = 0; i < any_path.size(); ++i)
    {
        Path new_path;
        for (size_t j = 0; j < any_path.size(); ++j)
        {
            new_path.push_back(any_path[j]);
        }

        // If the new path exists and has fewer affiliations, update the minimum affiliations path
        if (!new_path.empty() && new_path.size() < min_affiliations)
        {
            min_affiliations = new_path.size();
            min_affiliations_path = new_path;
        }
    }

    return min_affiliations_path;
}

Path Datastructures::get_path_of_least_friction(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_of_least_friction()");
}


PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target) {
    // Check if the source and target IDs exist in the affiliations_ map
    if (affiliations_.count(source) == 0 || affiliations_.count(target) == 0) {
        return PathWithDist(); // Return an empty vector if source or target not found
    }

    // Create a set to store the visited affiliations
    std::set<AffiliationID> visited;

    // Create a priority queue to store the next nodes to explore based on distance
    std::priority_queue<std::pair<Distance, AffiliationID>> pq;

    // Map to store the distance from source to each node
    std::unordered_map<AffiliationID, Distance> distance;

    // Map to store the previous node in the shortest path
    std::unordered_map<AffiliationID, Connection> previous;

    // Initialize distances
    distance[source] = 0;

    // Enqueue the source node with distance 0
    pq.push({0, source});

    while (!pq.empty()) {
        AffiliationID current = pq.top().second;
        pq.pop();

        // Check if the current node is the target
        if (current == target) {
            // Reconstruct the path
            PathWithDist result;
            AffiliationID current_node = target;

            while (previous.find(current_node) != previous.end()) {
                Connection connection = previous[current_node];
                // Calculate the distance of the edge by subtracting the distance of the previous node
                Distance edge_distance = distance[current_node] - distance[connection.aff1 == current_node ? connection.aff2 : connection.aff1];
                result.push_back({connection, edge_distance});
                current_node = connection.aff1 == current_node ? connection.aff2 : connection.aff1;
            }

            std::reverse(result.begin(), result.end());
            return result;
        }

        // Mark the current node as visited
        visited.insert(current);

        // Get the connections of the current affiliation
        std::vector<Connection> connections = get_connected_affiliations(current);

        // Loop through the connections
        for (const Connection &connection : connections) {
            // Get the other affiliation ID
            AffiliationID other = (connection.aff1 == current) ? connection.aff2 : connection.aff1;

            // Check if the other ID is not visited
            if (visited.count(other) == 0) {
                // Calculate Euclidean distance inline
                Coord coord1 = affiliations_[current].location;
                Coord coord2 = affiliations_[other].location;
                Distance tentative_distance = static_cast<Distance>(
                    std::sqrt(std::pow(coord1.x - coord2.x, 2) + std::pow(coord1.y - coord2.y, 2))
                );

                // Calculate the tentative total distance
                Distance total_distance = distance[current] + tentative_distance;

                // If the tentative total distance is smaller, update the distance and enqueue the neighbor
                if (distance.find(other) == distance.end() || total_distance < distance[other]) {
                    distance[other] = total_distance;
                    previous[other] = connection;
                    pq.push({-total_distance, other});
                }
            }
        }
    }

    // If the loop completes, there is no path from source to target
    return PathWithDist();
}
