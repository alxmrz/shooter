#include <gtest/gtest.h>
#include "Event.h"

TEST(EventHandlerTest, EventTypeClosed) {
    Event event(static_cast<int>(Event::Type::Closed));
    EXPECT_TRUE(event.HasType(Event::Type::Closed));
    EXPECT_FALSE(event.HasType(Event::Type::KeyPressed));
}

TEST(EventHandlerTest, EventTypeKeyPressed) {
    Event event(static_cast<int>(Event::Type::KeyPressed));
    EXPECT_TRUE(event.HasType(Event::Type::KeyPressed));
    EXPECT_FALSE(event.HasType(Event::Type::KeyReleased));
}

TEST(EventHandlerTest, EventTypeKeyReleased) {
    Event event(static_cast<int>(Event::Type::KeyReleased));
    EXPECT_TRUE(event.HasType(Event::Type::KeyReleased));
    EXPECT_FALSE(event.HasType(Event::Type::KeyPressed));
}

TEST(EventHandlerTest, EventTypeMouseWheelScrolled) {
    Event event(static_cast<int>(Event::Type::MouseWheelScrolled));
    EXPECT_TRUE(event.HasType(Event::Type::MouseWheelScrolled));
    EXPECT_FALSE(event.HasType(Event::Type::MouseButtonPressed));
}

TEST(EventHandlerTest, EventTypeMouseButtonPressed) {
    Event event(static_cast<int>(Event::Type::MouseButtonPressed));
    EXPECT_TRUE(event.HasType(Event::Type::MouseButtonPressed));
    EXPECT_FALSE(event.HasType(Event::Type::MouseMoved));
}

TEST(EventHandlerTest, EventTypeMouseMoved) {
    Event event(static_cast<int>(Event::Type::MouseMoved));
    EXPECT_TRUE(event.HasType(Event::Type::MouseMoved));
    EXPECT_FALSE(event.HasType(Event::Type::Closed));
}

TEST(EventHandlerTest, EventDefaultConstructorCreatesNoType) {
    Event event;
    EXPECT_FALSE(event.HasType(Event::Type::Closed));
    EXPECT_FALSE(event.HasType(Event::Type::KeyPressed));
    EXPECT_FALSE(event.HasType(Event::Type::KeyReleased));
    EXPECT_FALSE(event.HasType(Event::Type::MouseWheelScrolled));
    EXPECT_FALSE(event.HasType(Event::Type::MouseButtonPressed));
    EXPECT_FALSE(event.HasType(Event::Type::MouseMoved));
}

TEST(EventHandlerTest, EventConstructorWithIntType) {
    Event event(42);
    EXPECT_FALSE(event.HasType(Event::Type::Closed));
    EXPECT_FALSE(event.HasType(Event::Type::KeyPressed));
}

TEST(EventHandlerTest, IsKeyBoardUpDefaultFalse) {
    Event event;
    EXPECT_FALSE(event.IsKeyBoardUp());
}

TEST(EventHandlerTest, IsKeyBoardLeftDefaultFalse) {
    Event event;
    EXPECT_FALSE(event.IsKeyBoardLeft());
}

TEST(EventHandlerTest, IsKeyBoardRightDefaultFalse) {
    Event event;
    EXPECT_FALSE(event.IsKeyBoardRight());
}

TEST(EventHandlerTest, IsKeyReleasedDefaultFalse) {
    Event event;
    EXPECT_FALSE(event.IsKeyReleased());
}
