all : dnasearch
dnasearch : dnasearch.o
	g++ dnasearch.o -o dnasearch
dnasearch.o : dnasearch.cpp
	g++ -c dnasearch.cpp -o dnasearch.o
clean :
	rm dnasearch.o
	rm dnasearch
run :
	./dnasearch d = bact.txt q = mydna.txt n = 27 a = 0