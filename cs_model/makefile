LDFLAGS = -lpthread

src = $(wildcard *.cpp)
target = $(patsubst %.cpp, %, $(src))

ALL:$(target)

%:%.cpp
	g++ $< -o $@ $(LDFLAGS)

clean:
	-rm -rf $(target) 

.PHONY: clean ALL
