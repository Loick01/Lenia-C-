
LIBS = -lm -lSDL2 
CPP = g++   

main : 
	$(CPP) main.cpp -o main $(LIBS)

clean :
	rm main
	
run : 
	./main
