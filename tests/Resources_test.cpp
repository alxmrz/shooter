#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include "Resources.h"

TEST(ResourcesDialogueTest, ParsesNonBlankLines) {
    std::istringstream input("line one\n\n  \nline two\r\n");

    std::vector<std::string> lines = Resources::parseDialogueLines(input);

    ASSERT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines[0], "line one");
    EXPECT_EQ(lines[1], "line two");
}

TEST(ResourcesDialogueTest, EmptyInputYieldsNoLines) {
    std::istringstream input("");

    std::vector<std::string> lines = Resources::parseDialogueLines(input);

    EXPECT_TRUE(lines.empty());
}

TEST(ResourcesDialogueTest, KeepsOriginalOrder) {
    std::istringstream input("first\nsecond\nthird\n");

    std::vector<std::string> lines = Resources::parseDialogueLines(input);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "first");
    EXPECT_EQ(lines[1], "second");
    EXPECT_EQ(lines[2], "third");
}
