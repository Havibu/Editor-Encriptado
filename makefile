fecha.o: fecha.cpp fecha.hpp
	g++ -c -g fecha.cpp fecha.hpp -std=c++11
	
entrada.o: entrada.hpp
	g++ -c -g entrada.hpp -std=c++11
