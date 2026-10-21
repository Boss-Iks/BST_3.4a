#include<string>
#include<vector>
using namespace std;
struct IPNode{
    string ipAddress;
    int accesses, successAccesses;
    
    IPNode *left;
    IPNode *right;
    IPNode *parent;
    IPNode(const string &ip = "")
        : ipAddress(ip), accesses(0), successAccesses(0),
          left(nullptr), right(nullptr), parent(nullptr) {}
};
struct MonthInfo{
    IPNode *rootNode;
    int disctinctAddresses;

    MonthInfo(IPNode *rootIP=nullptr) : rootNode(rootIP), disctinctAddresses(0) {}
};

//test helper functions
vector<MonthInfo> getMonths();
vector<IPNode *> getTopAccesses() ;

// If possible, keeps the file handle open and only reads new lines.
string extractLine();

// Inserts/updates the new line's info into the correct month's splay tree
void updateTree(const string &line);

// Called after each updateTree to maintain the top-15 accesses vector
void updateHighestAccess();

// Returns info for a specific IP address (searches all months or current month)
// Returns a formatted string with the result
string searchIP(const string &ip);

// Returns info for a range of IPs (also used for block/subnet searches)
string searchRange(const string &startIP, const string &endIP);

// Launches a non-blocking thread to handle user queries (search IP, range, etc.)
void getUserQuery();

// Writes the full report to output.txt:
// monthly reports, top 15, and range query results
void generateReport();

