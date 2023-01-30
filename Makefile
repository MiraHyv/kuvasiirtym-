src =  \
src/kello.cpp \
src/ikkuna.cpp \
src/shader.cpp \
src/kuva.cpp \
src/kuvanPiirtaja.cpp \
src/asetukset.cpp \
src/siirtyma.cpp \
src/main.cpp

cflags = \
`pkg-config glfw3 glew opencv --cflags` -std=c++17 

lflags = `pkg-config glfw3 glew opencv --libs`

executable = piirto

all:
	g++ $(src) $(cflags) $(lflags) -o $(executable)

