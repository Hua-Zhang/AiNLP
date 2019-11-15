 
objects = main.o config.o kmeans.o

kmeans: $(objects)
	g++ -o kmeans $(objects)

main.o: config.h kmeans.h

config.o: config.h

kmeans.o: kmeans.h config.h


.PHONY: clean

clean:
	rm kmeans $(objects)
