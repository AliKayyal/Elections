CC=gcc
OBJS= main.o Citizen.o CitizenCandidate.o City.o CityCandidate.o mtm_elections.o mtm_print.o ./uniqueOrderedList/uniqueOrderedList.o 
EXEC= mtmElection
DEBUG_FLAG=-g
COMP_FLAG=-std=c99 -Wall -Werror
$(EXEC):$(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -L. -lmtm -o $@
main.o: main.c mtm_elections.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c	
mtm_elections.o: mtm_elections.c mtm_print.h mtm_elections.h ./uniqueOrderedList/uniqueOrderedList.h City.h Citizen.h CitizenCandidate.h CityCandidate.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
Citizen.o: Citizen.c ./uniqueOrderedList/uniqueOrderedList.h Citizen.h City.h CitizenCandidate.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
CitizenCandidate.o: CitizenCandidate.c ./uniqueOrderedList/uniqueOrderedList.h CitizenCandidate.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
City.o: City.c ./uniqueOrderedList/uniqueOrderedList.h Citizen.h City.h CityCandidate.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
CityCandidate.o: CityCandidate.c ./uniqueOrderedList/uniqueOrderedList.h CityCandidate.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
mtm_print.o: mtm_print.c mtm_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
uniqueOrderedList.o: ./uniqueOrderedList/uniqueOrderedList.c ./uniqueOrderedList/uniqueOrderedList.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(EXEC):$(OBJS) 	
 