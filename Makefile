
SOURCE		=	vector.hpp main.cpp
EXE		=	a.out


a.out:	$(SOURCE)
	@g++ -std=c++14 -o $(EXE) -Wall $(SOURCE)

clean:
	@rm -f $(EXE)
