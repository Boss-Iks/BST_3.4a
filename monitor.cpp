#include "monitor.h"

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

// Global program state
vector<MonthInfo> months(12);
vector<IPNode *> topAccesses(15, nullptr);
//for tests 
std::vector<MonthInfo> getMonths() {
    return months;
}

std::vector<IPNode *> getTopAccesses() {
    return topAccesses;
}
static ifstream logFile;
static mutex dataMutex;

// If possible, keeps the file handle open and only reads new lines.
string extractLine() {
    // TODO:
    // 1. Open the log file if it is not already open.
    // 2. Read the next available line.
    // 3. If no new line is available, return an empty string.
    // 4. Keep the file handle open between calls.

    return "";
}

// Inserts/updates the new line's info into the correct month's splay tree
void updateTree(const string &line) {
    // TODO:
    // 1. Parse the line: IP address, month, success/failure, etc.
    // 2. Find or create the correct IPNode in months[month].rootNode.
    // 3. Update accesses and successAccesses.
    // 4. Update distinctAddresses if this is a new IP for the month.
    // 5. Call updateHighestAccess().
}

// Called after each updateTree to maintain the top-15 accesses vector
void updateHighestAccess() {
    // TODO:
    // 1. Traverse all month trees.
    // 2. Find the 15 IP nodes with the highest access counts.
    // 3. Store their pointers in topAccesses[0] through topAccesses[14].
}

// Returns info for a specific IP address.
string searchIP(const string &ip) {
    // TODO:
    // 1. Search each month's tree for the IP.
    // 2. Collect total accesses and successful accesses.
    // 3. Return a formatted result string.

    return "";
}

// Returns info for a range of IPs.
string searchRange(const string &startIP, const string &endIP) {
    // TODO:
    // 1. Convert IPs to a comparable form if needed.
    // 2. Traverse each month's tree in order.
    // 3. Collect nodes whose IPs fall within [startIP, endIP].
    // 4. Return a formatted result string.

    return "";
}

// Launches a non-blocking thread to handle user queries.
void getUserQuery() {
    // TODO:
    // 1. Create a detached thread or background worker.
    // 2. Read user commands without blocking the main log-reading loop.
    // 3. Call searchIP() or searchRange() depending on the command.
    // 4. Print or store the result.
}

// Writes the full report to output.txt.
void generateReport() {
    // TODO:
    // 1. Open output.txt.
    // 2. Write monthly reports.
    // 3. Write the top 15 accesses.
    // 4. Write range-query results if applicable.
    // 5. Close the file.
}

int main(){
    return 1;
}