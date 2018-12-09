CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system

SOURCES= \
main.cpp \
src/Application.cpp \
src/Builder.cpp \
src/CObject.cpp \
src/Event.cpp \
src/GameObjects.cpp \
src/GameState.cpp \
src/Scene.cpp \
src/Window.cpp \
src/objects/Ground.cpp \
src/objects/Shooter.cpp \
src/ui/Button.cpp \
src/ui/Text.cpp \

DIRS = build build/src build/src/objects build/src/ui


OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=run

DIR_OBJECTS:=$(addprefix  build/, $(OBJECTS))

	
all: clean dirs $(SOURCES) $(EXECUTABLE)
	
dirs:
	$(info $(shell mkdir -p $(DIRS)))
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(DIR_OBJECTS) $(LDFLAGS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o build/$@

clean:
	rm -rf *.o dist/hello build/*.o build/src/*.o
	
ex:
	echo $(DIR_OBJECTS)