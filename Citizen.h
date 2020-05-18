
#ifndef CITIZEN_H_
#define CITIZEN_H_
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"

/**The typdef struct Citizen is a struct that has all of the necessary fields
for a citizen. The fields are:
 1-The most important and special field is citizen_candidates which
is a UniqueOrderedList contains all of the candidates that the particular
citizen supports and the priority given to every one of them
 2- unique_id= the id of the citizen
 3- age= the age of the citizen
 4-eduaction_years= the years of eduaction of a candidate */

typedef struct citizen_t *Citizen;

/****************************************************************************/

/**All of the function: CitizenElementsEqual,CitizenElementGreaterThan,
 CitizenCopyElements and CitizenFreeElements are "helpers" that we use to
 compare,insert and
remove elements from the UniqueOrderedListcities according to certain standard
fixed to each struct and how they should be ordered in the UniqueOrderedLists.
If there is no importance of the ordered in a certain list then thae function
<DataType>elementGreaterThan is done according to the id.*/

/****************************************************************************/

/**The function gets two Citizens and compares their ids
 If their ids are equal then it returns true.
 Else it returns false*/

bool CitizenElementsEqual (Citizen ,Citizen );

/****************************************************************************/

/**This function gets two Citizens and compares between their ids and
 checks if the id of the first citizen is greater than the one of the second.
 If this happens then it returns true.
 Else it returns false*/

bool CitizenElementGreaterThan(Citizen ,Citizen );

/****************************************************************************/

/**This function gets a Citizen and returns a copy of it
 It copies all of the fields in it and uses the function
 UniqueOrderedListCopy to copy the list citizen_candidates
 It returns a copy of the struct*/

Citizen CitizenCopyElements(Citizen );

/****************************************************************************/

/**This function gets a Citizen and frees up all of the fields in it
 it uses the function UniqueOrderedListDestroy to free up the UniqueOrderedList
 citizen_candidates in it. At the end it frees up the citizen*/

void CitizenFreeElements(Citizen );

/***************************************************************************/

/**This function searches for a citizen in a certain city and return it.
In case the city is empty or NULL or if the citizen does not exist then the
function returns NULL*/

Citizen GetCitizen(int ,UniqueOrderedList );

/****************************************************************************/

/**This function searches for a certain citizen and checks if he exists
in the system (In any given city).If he doesn't exist or if there
 are no cities in the system then it return NULL.
 Note that the function uses the other helper GetCitizen */

Citizen CitizenExistInSystem(int citizenId,UniqueOrderedList cities);

/*****************************************************************************/

/**This function gets the id of the candidate and searches for him in the
UniqueOrderedList candidates .If he exists then it gets returned.
 Else NULL is returned.(Note that the UniqueOrderedList candidates
 contains citizens that are candidates that is why we have this function
 here)*/
Citizen GetCandidate(int,UniqueOrderedList);

/****************************************************************************/

/**This function gets the id of the candidate and searches for him in the
 UniqueOrderedList candidates .If he exists then it gets returned.
 Else NULL is returned.(Note that the UniqueOrderedList candidates
 contains citizens that are candidates that is why we have this function
 here)
 This is the same as GetCandidate but we used this also so we make
 the code more understandable and get our intentions understood.*/

Citizen CandidateExistsInSystem(int,UniqueOrderedList);

/******************************************************************************/

/**This function gets a priority and a UniqueOrderedList citizen_candidates
 and the citizen id of the citizen who owns this UniqueOrderedList
 and searches if he had given this particular priority to a candidate
 if this happens then it returns true, else it returns false*/

bool PriorityExists(int,UniqueOrderedList);

/****************************************************************************/

/**This function gets a citizen and allocates the UniqueOrderedList
 citizen_candidates. If the allocation failed then it frees up all of the
 memory allocated to the citizen including the struct itself and return true
 Else it returns false.
 * This function we use in the function mtmElectionsAddCitizen in order to
 make the function shorter and easier to understand.*/
bool CitizenCandidatesFailed(Citizen);

/*****************************************************************************/

/**This function gets a citizen and checks the allocation of his name
 If the allocation failed then it frees up all of the
 memory allocated to the citizen including the struct itself and return true
 Else it returns false.This function we use in the function
 mtmElectionsAddCitizen in order to
 make the function shorter and easier to understand.*/

bool CitizenNameMallocFails (Citizen);

/******************************************************************************/

/**This function inserts the citizen in the UniqueOrderedList citizens.
 If the insertion failed then it frees up all of the
 memory allocated to the citizen including the struct itself and return true
 Else it returns false.This function we use in the function
  mtmElectionsAddCitizen in ordere to
 make the function shorter and easier to understand.*/

bool CitizenInsertFailed(UniqueOrderedList,Citizen);

/******************************************************************************/

/**This function gets a citizen a returns his age*/
int ReturnCitizenAge(Citizen);

/******************************************************************************/

/**This function gets a citizen a returns his name*/

char* ReturnCitizenName(Citizen);

/******************************************************************************/

/**This function gets a citizen a returns his id*/

int ReturnCitizenId(Citizen);

/******************************************************************************/

/**This function gets a citizen a returns a pointer the the
 UniqueOrderedList citizen_candidates*/

UniqueOrderedList ReturnCitizenCandidates(Citizen);

/******************************************************************************/

/**This function gets a citizen a returns his years of education*/
int ReturnCitizenEducationYears(Citizen);

/******************************************************************************/

/**This function gets a citizen and a const char* name and
 allocates the name of the citizen and copies the given name to the
 allocated name */
void SetCitizenName(Citizen,const char*);

/******************************************************************************/

/**This function gets a citizen and an int age and changes the citizen's
 age to hold the value of the given age*/
void SetCitizenAge(Citizen,int);

/******************************************************************************/

/**This function gets a citizen and an int id and changes the citizen's
 id to hold the value of the given id*/
void SetCitizenID(Citizen,int);

/******************************************************************************/

/**This function gets a citizen and an int years of education
 and changes the citizen's int years of education
 to hold the value of the given int years of education*/

void SetCitizenEduactionYears(Citizen,int);

#endif /* CITIZEN_H_ */
