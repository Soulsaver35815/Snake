CXX       := /usr/bin/g++
CXX_FLAGS := -std=c++17 -fdiagnostics-color=always -Wall
RELEASE_FLAGS := -Ofast
DEBUG_FLAGS := -fsanitize=address
LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio


SRC     := src
INCLUDE := include

all: clean main.exe

release: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $(RELEASE_FLAGS) -I$(INCLUDE) $^ -o Snake $(LIBRARIES) 

main.exe: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

debug: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) -I$(INCLUDE) $^ -o $@.exe $(LIBRARIES)
run: all
	./main.exe

clean:
	rm -f *.exe
