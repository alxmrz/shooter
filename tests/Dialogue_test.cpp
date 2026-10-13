#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "GameObjectsTestFixture.h"
#include "Resources.h"

TEST_F(GameObjectsFixture, TypewriterRevealsPrefix) {
    spawnDialogue({"Hello"}, 10.f);
    updateDialogue(0.2f);
    expectDialogueText("He");
}

TEST_F(GameObjectsFixture, TypewriterCompletesAfterEnoughTime) {
    spawnDialogue({"Hello"}, 10.f);
    updateDialogue(1.0f);
    expectDialogueText("Hello");
}

TEST_F(GameObjectsFixture, AdvanceDuringTypingRevealsWholeFragment) {
    spawnDialogue({"Hello", "World"}, 10.f);
    updateDialogue(0.1f);
    advanceDialogue();
    expectDialogueText("Hello");
    expectDialogueIndex(0);
}

TEST_F(GameObjectsFixture, AdvanceAfterCompleteShowsNextFragment) {
    spawnDialogue({"Hello", "World"}, 10.f);
    updateDialogue(1.0f);
    advanceDialogue();
    expectDialogueIndex(1);
    EXPECT_TRUE(dialogueOpen());
    expectDialogueText("");
}

TEST_F(GameObjectsFixture, AdvanceOnLastFragmentClosesDialogue) {
    spawnDialogue({"Hello"}, 10.f);
    updateDialogue(1.0f);
    advanceDialogue();
    EXPECT_FALSE(dialogueOpen());
}

TEST_F(GameObjectsFixture, EmptyFragmentsKeepDialogueClosed) {
    spawnDialogue({});
    EXPECT_FALSE(dialogueOpen());
}

TEST_F(GameObjectsFixture, PromptShownWhileOpenHiddenAfterClose) {
    spawnDialogue({"Hello"}, 10.f);
    EXPECT_TRUE(dialoguePromptShown());
    updateDialogue(1.0f);
    advanceDialogue();
    EXPECT_FALSE(dialoguePromptShown());
}

TEST_F(GameObjectsFixture, DialogueLinesIgnoreBlankLines) {
    std::istringstream input("line one\n\n   \nline two\n");
    std::vector<std::string> lines = Resources::parseDialogueLines(input);
    ASSERT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines[0], "line one");
    EXPECT_EQ(lines[1], "line two");
}
