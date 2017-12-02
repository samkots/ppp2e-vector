
SOURCE		=	vector.hpp util.hpp main.cpp
EXE		=	a.out


a.out:	$(SOURCE)
	@g++ -std=c++14 -o $(EXE) -Wall $(SOURCE)

debug: $(SOURCE)
	@g++ -std=c++14 -o $(EXE) -Wall $(SOURCE) -g

clean:
	@rm -f $(EXE)
