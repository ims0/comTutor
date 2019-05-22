cc = g++ -std=c++11 
prom = ft
out = ../output/
deps = ../include/findstr.h 
obj = ../src/findstr.o TestCase.o
LIBS = -lgtest -lgtest_main -lpthread

$(prom): $(obj)
	$(cc) $(obj)  $(LIBS) -o $(out)$(prom)

%.o: %.cpp $(deps)
	$(cc) -c $< -o $@
	
	
.PHONY: clean
clean:
	rm -f $(obj) 
