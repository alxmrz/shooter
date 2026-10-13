#include <gtest/gtest.h>

#include "GameObjectsTestFixture.h"

TEST_F(GameObjectsFixture, WorldPausedWhileDialogueOpen) {
    UpdateSpy* spy = spawnUpdateSpy();
    spawnDialogue({"Hello"}, 10.f);
    go->update(Event(), 0.1f);
    expectObjectNotUpdated(spy);
    expectDialogueText("H");
}

TEST_F(GameObjectsFixture, WorldResumesAfterDialogueClosed) {
    UpdateSpy* spy = spawnUpdateSpy();
    spawnDialogue({"Hi"}, 10.f);
    updateDialogue(1.0f);
    advanceDialogue();
    go->update(Event(), 0.1f);
    expectObjectUpdated(spy);
}
