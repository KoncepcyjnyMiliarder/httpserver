CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++11 -Werror -I.
LDLIBS = -lstdc++ -lpthread
TARGETS = server

all: server

server: main.o request_parser.o response_maker.o server.o session.o

main.o: main.cpp

request_parser.o: request_parser.cpp

response_maker.o: response_maker.cpp

server.o: server.cpp

session.o: session.cpp

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)
