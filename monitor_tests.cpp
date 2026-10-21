#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "monitor.h"

namespace {

class MonitorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset externally visible global state before every test.
        vector<MonthInfo> months=getMonths();
        months.assign(12, MonthInfo());
        vector<IPNode *>topAccesses=getTopAccesses();
        topAccesses.assign(15, nullptr);

        logFileName = "monitor_test_log.txt";
        std::ofstream logFile(logFileName);
        ASSERT_TRUE(logFile.is_open());

        logFile << "2026-01-15 192.168.1.10 success\n";
        logFile << "2026-01-15 192.168.1.10 success\n";
        logFile << "2026-01-15 192.168.1.20 failure\n";
        logFile << "2026-02-20 10.1.1.5 success\n";
        logFile.close();
    }

    void TearDown() override {
        std::remove(logFileName.c_str());
        std::remove("output.txt");
    }

    static bool containsTopAccess(const std::string& ip) {
        for (const IPNode* node : getTopAccesses()) {
            if (node != nullptr && node->ipAddress == ip) {
                return true;
            }
        }
        return false;
    }

    std::string logFileName;
};

TEST_F(MonitorTest, ExtractLineReadsEachLogFileLine) {
    EXPECT_EQ(extractLine(), "2026-01-15 192.168.1.10 success");
    EXPECT_EQ(extractLine(), "2026-01-15 192.168.1.10 success");
    EXPECT_EQ(extractLine(), "2026-01-15 192.168.1.20 failure");
    EXPECT_EQ(extractLine(), "2026-02-20 10.1.1.5 success");

    // Once the file is exhausted, it should return an empty string.
    EXPECT_EQ(extractLine(), "");
}

TEST_F(MonitorTest, ExtractLineKeepsFileHandleOpenBetweenCalls) {
    EXPECT_EQ(extractLine(), "2026-01-15 192.168.1.10 success");

    // Append a new line after the first read; a persistent handle should see it.
    std::ofstream logFile(logFileName, std::ios::app);
    ASSERT_TRUE(logFile.is_open());
    logFile << "2026-01-16 192.168.1.30 success\n";
    logFile.close();

    EXPECT_EQ(extractLine(), "2026-01-15 192.168.1.10 success");
    EXPECT_EQ(extractLine(), "2026-01-15 192.168.1.20 failure");
    EXPECT_EQ(extractLine(), "2026-02-20 10.1.1.5 success");
    EXPECT_EQ(extractLine(), "2026-01-16 192.168.1.30 success");
}

TEST_F(MonitorTest, UpdateTreeCreatesNodeForFirstAccess) {
    updateTree("2026-01-15 192.168.1.10 success");

    const std::string result = searchIP("192.168.1.10");
    EXPECT_NE(result.find("192.168.1.10"), std::string::npos);
    EXPECT_NE(result.find("1"), std::string::npos);
    EXPECT_NE(result.find("1 successful"), std::string::npos);
}

TEST_F(MonitorTest, UpdateTreeCountsRepeatedAccesses) {
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.10 failure");

    const std::string result = searchIP("192.168.1.10");

    // Use structured output or explicitly labeled fields in searchIP()
    // so these checks cannot pass by finding digits inside the IP address.
    EXPECT_NE(result.find("accesses=3"), std::string::npos);
    EXPECT_NE(result.find("successful=2"), std::string::npos);
}

TEST_F(MonitorTest, UpdateTreeSeparatesMonths) {
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-02-20 192.168.1.10 success");

    const MonthInfo& january = getMonths()[0];
    const MonthInfo& february = getMonths()[1];

    ASSERT_NE(january.rootNode, nullptr);
    ASSERT_NE(february.rootNode, nullptr);

    EXPECT_EQ(january.rootNode->accesses, 1);
    EXPECT_EQ(february.rootNode->accesses, 1);
    EXPECT_EQ(january.disctinctAddresses, 1);
    EXPECT_EQ(february.disctinctAddresses, 1);
}

TEST_F(MonitorTest, UpdateTreeCountsDistinctAddresses) {
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.10 failure");
    updateTree("2026-01-15 192.168.1.20 success");

    EXPECT_EQ(getMonths()[0].disctinctAddresses, 2);
}

TEST_F(MonitorTest, SearchIPReturnsEmptyForUnknownAddress) {
    updateTree("2026-01-15 192.168.1.10 success");

    EXPECT_EQ(searchIP("203.0.113.99"), "");
}

TEST_F(MonitorTest, SearchRangeFindsOnlyAddressesInRange) {
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.20 success");
    updateTree("2026-01-15 10.0.0.1 success");

    const std::string result =
        searchRange("192.168.1.0", "192.168.1.255");

    EXPECT_NE(result.find("192.168.1.10"), std::string::npos);
    EXPECT_NE(result.find("192.168.1.20"), std::string::npos);
    EXPECT_EQ(result.find("10.0.0.1"), std::string::npos);
}

TEST_F(MonitorTest, UpdateHighestAccessRanksMostAccessedIPs) {
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.10 success");
    updateTree("2026-01-15 192.168.1.20 success");
    updateTree("2026-01-15 192.168.1.20 failure");

    updateHighestAccess();

    ASSERT_FALSE(getTopAccesses().empty());
    ASSERT_NE(getTopAccesses()[0], nullptr);

    EXPECT_EQ(getTopAccesses()[0]->ipAddress, "192.168.1.10");
    EXPECT_EQ(getTopAccesses()[0]->accesses, 3);

    EXPECT_TRUE(containsTopAccess("192.168.1.20"));
}

TEST_F(MonitorTest, TopAccessesHasAtMostFifteenEntries) {
    for (int i = 1; i <= 20; ++i) {
        updateTree("2026-01-15 192.168.1." + std::to_string(i) + " success");
    }

    updateHighestAccess();

    const std::vector<IPNode*> top = getTopAccesses();
    EXPECT_EQ(top.size(), 15u);

    for (const IPNode* node : top) {
        EXPECT_NE(node, nullptr);
    }
}

TEST_F(MonitorTest, GenerateReportCreatesNonEmptyOutputFile) {
    updateTree("2026-01-15 192.168.1.10 success");
    updateHighestAccess();

    generateReport();

    std::ifstream output("output.txt");
    ASSERT_TRUE(output.is_open());

    std::string contents(
        (std::istreambuf_iterator<char>(output)),
        std::istreambuf_iterator<char>());

    EXPECT_FALSE(contents.empty());
    EXPECT_NE(contents.find("192.168.1.10"), std::string::npos);
}

}  // namespace