CC=g++ 
CFLAGS=-c -Wall -g -std=c++11
LDFLAGS= -L/home/alexandr/libraries/SFML-2.5.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SOURCES= \
main.cpp \
src/Application.cpp \
src/CObject.cpp \
src/Event.cpp \
src/EventHandler.cpp \
src/GameObjects.cpp \
src/GameState.cpp \
src/Map.cpp \
src/AI.cpp \
src/Scene.cpp \
src/Window.cpp \
src/Fabric.cpp \
src/Resources.cpp \
src/objects/backgrounds/Ground.cpp \
src/objects/interactive/Crystal.cpp \
src/objects/interactive/Heart.cpp \
src/objects/interactive/Ammunition.cpp \
src/objects/interactive/Platform.cpp \
src/objects/units/Unit.cpp \
src/objects/units/Shooter.cpp \
src/objects/interactive/Bullet.cpp \
src/objects/interfaces/Movable.cpp \
src/ui/Button.cpp \
src/ui/Text.cpp \
include/tinyxml/tinyxml2.cpp

DIRS = build \
build/src \
build/src/objects \
build/src/objects/backgrounds \
build/src/objects/interactive \
build/src/objects/interfaces \
build/src/objects/units \
build/src/ui \
build/include/tinyxml


OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=run

DIR_OBJECTS:=$(addprefix  build/, $(OBJECTS))
DIR_OBJECTS_REMOVE:=$(addsuffix /*.o, $(DIRS))

	
all: clean dirs $(SOURCE) $(EXECUTABLE)

exec:
	./$(EXECUTABLE)

dirs:
	$(info $(shell mkdir -p $(DIRS)))
	
$(EXECUTABLE): $(DIR_OBJECTS) 
	$(CC) -no-pie $(DIR_OBJECTS) $(LDFLAGS) -o $@

build/%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o ./run $(DIR_OBJECTS_REMOVE)

# --- Testing ---
BUILD_DIR=build_cmake

test-build:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake -DBUILD_TESTS=ON .. && cmake --build . --target tests

test: test-build
	cd $(BUILD_DIR) && ./tests/tests

test-clean:
	rm -rf $(BUILD_DIR)

.PHONY: all exec dirs clean test test test-clean