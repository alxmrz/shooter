CC=g++ 
CFLAGS=-c -Wall -g -std=c++11
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system

SOURCES= \
main.cpp \
src/Application.cpp \
src/CObject.cpp \
src/Event.cpp \
src/GameObjects.cpp \
src/GameState.cpp \
src/Scene.cpp \
src/Window.cpp \
src/objects/Ground.cpp \
src/objects/Shooter.cpp \
src/objects/Bullet.cpp \
src/ui/Button.cpp \
src/ui/Text.cpp \

DIRS = build build/src build/src/objects build/src/ui


OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=run

DIR_OBJECTS:=$(addprefix  build/, $(OBJECTS))

	
all: dirs $(SOURCE) $(EXECUTABLE)
	
dirs:
	$(info $(shell mkdir -p $(DIRS)))
	
$(EXECUTABLE): $(DIR_OBJECTS) 
	$(CC) -no-pie $(DIR_OBJECTS) $(LDFLAGS) -o $@

build/%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o ./run build/*.o build/src/*.o build/src/objects/*.o build/src/ui/*.o
	
ex:
	echo $(DIR_OBJECTS)