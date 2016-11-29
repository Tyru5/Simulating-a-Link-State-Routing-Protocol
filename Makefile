CC = g++
EXEC = router
OBJS =  
H_FILE = router.h
FLAGS = -Wall 

all: $(EXEC)
router: router.cpp $(OBJS) $(H_FILE)
	$(CC) $(SEQFLAGS) $(MATHFLAG) -o $@ router.cpp  $(OBJS)

clean: 
	rm -f *.o $(OBJS) $(EXEC)
