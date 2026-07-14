src = $(wildcard *.cpp)
target = $(patsubst %.cpp, %, $(src))

ALL:$(target)

%:%.cpp
	g++ $< -o $@

clean:
	-rm -rf $(target) 

.PHONY: clean ALL
