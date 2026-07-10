src = $(wildcard ./src/*.cpp)    
obj = $(patsubst ./src/%.cpp, ./obj/%.o, $(src))

ALL:main
myArgs =  -Wall -g
inc_path = ./inc

$(obj):./obj/%.o:./src/%.cpp
	g++ -c $< -o $@ $(myArgs) -I $(inc_path) 

main:$(obj)
	g++ $^ -o $@ $(myArgs)

clean:
	-rm -rf $(obj) main

.PHONY: clean ALL
