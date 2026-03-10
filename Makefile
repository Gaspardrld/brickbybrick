CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET   = project

SRC  = project.cc game.cc brick.cc ball.cc paddle.cc message.cc tools.cc

# Pour le rendu 2/3 :
#SRC += gui.cc graphic.cc
#CXXFLAGS += $(shell pkg-config --cflags gtkmm-3.0)
#LDFLAGS   = $(shell pkg-config --libs   gtkmm-3.0)

all: $(SRC:.cc=.o)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

project.o : project.cc  game.h
game.o  : game.cc     game.h brick.h ball.h paddle.h message.h tools.h constants.h
brick.o  : brick.cc    brick.h tools.h constants.h message.h
ball.o  : ball.cc     ball.h tools.h constants.h message.h
paddle.o : paddle.cc   paddle.h tools.h constants.h message.h
message.o : message.cc  message.h
tools.o : tools.cc    tools.h
#gui.o : gui.cc      game.h graphic.h
#graphic.o : graphic.cc  graphic.h tools.h

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean