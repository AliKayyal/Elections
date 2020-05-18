#ifndef CITY_H_
#define CITY_H_
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "Citizen.h"

/*********************************************************************/

/**The typdef struct City contains the information of a specific
 city. It has a CityId and a name.
 Three special fields in it are three UniqueOrderedLists:
 1-City_candidates has all of the the ids of the candidates and ther votes that
 everyone got.
 2-Candidates has all of the citizens in the cty that are also candidates
 3-Citizens has all of the citizens in the city*/

typedef struct city_t *City;

/*********************************************************************/

/**The function CityElementsEqual gets two cities and checks if both of the
 ids of the two cities are equal, if yes then it return true. Else it returns
 false*/

bool CityElementsEqual(City, City);

/*********************************************************************/

/**The function CityElementGreaterThan gets two cities and checks if the id
 of the first city is greater than the second one, if yes then it return true.
 Else it returns false*/

bool CityElementGreaterThan(City, City);

/*********************************************************************/

/**The function CityCopyElements gets a city and copies all of the fields in it
 it returns a copy of the city*/

City CityCopyElements(City);

/*********************************************************************/

/**The function CityFreeElements gets a city and frees up all of the elements
 in it. Because there are unique orderedlists in the city then it uses the
 function UniqueOrderedListDestroy for every one of them*/

void CityFreeElements(City);

/*********************************************************************/

/**This function searches between all of the cities for a particular
 city in which the citizen with a certain id exists.
 It uses the function GetCitizen implemented in the c file Citizen.
 If the citizen holding the id does not exist, or if there ar no cities
 then it returns NuLL
 If the citizen exists then it returns a pointer to the city*/

City GetCityWhereCitizenExists(int, UniqueOrderedList);

/*********************************************************************/

/**This function gets a city id and return the city holding that id.
 If it doesn't exist or if the UniqueOrderedList cities are NULL then it returns
 NULL
 We Use this function so that we can access certain fields of a city in other
 functions*/

City GetCityThroughID(int, UniqueOrderedList);

/*********************************************************************/

/**This function uses the function UniqueOrderedListContains
 and checks if the city already exists.If it exists then it returns true
 Else it returns false.
 Note that we use this function in the function MtmElectionAddCity
 implemented in mtm_elections.c. If the city does already exist then
 it frees up allof the storage that we allocated for it until the moment we
 called it*/

bool CityContained( UniqueOrderedList, City);

/*********************************************************************/

/**The function CityNameMallocFails gets a city and checks if the allocated
 name for it is NULL (The malloc failed).
 If yes then it returns true and frees up all of the allocated fields for it
 until the moment of its call.
 We use this function only in the function MtmElectionAddCity as a helper
 so that we save some lines and make the code more understandable*/

bool CityNameMallocFails(City);

/*********************************************************************/

/**This function gets a city and checks if the allocation its internal
 UniqueOrderedList city_candidates failed.
 If yes then it free up all of the fields allocated for it until the
 moment of its call and return true.
 If the allocation succeeded then it returns false
 We use this function only in the function MtmElectionAddCity as a helper
 so that we save some lines and make the code more understandable*/

bool CityCandidatesFailed(City);

/*********************************************************************/

/**This function gets a city and checks if the allocation its internal
 UniqueOrderedList candidates failed.
 If yes then it free up all of the fields allocated for it until the
 moment of its call and return true.
 If the allocation succeeded then it returns false
 We use this function only in the function MtmElectionAddCity as
 a helper so that we save some lines and make the code more understandable*/

bool CandidatesFailed(City);

/*********************************************************************/

/**This function gets a city and checks if the allocation its internal
 UniqueOrderedList citizens failed.
 If yes then it free up all of the fields allocated for it until the
 moment of its call and return true.
 If the allocation succeeded then it returns false
 We use this function only in the function MtmElectionAddCity
 as a helper so that we save some lines and make the code more understandable*/

bool CitizensFailed(City);

/*********************************************************************/

/**This function gets the  UniqueOrderedList cities and a city  and
 tries to insert the city in the UniqueOrderedList cities
 If the insertion failed then it frees up all of the allocated fields
 for it and returns true.
 Else it insert the city in the list and returns false
 We use this function only in the function MtmElectionAddCity
 as a helper so that we save some lines and make the code more understandable*/

bool CityInsertFailed( UniqueOrderedList, City);

/*********************************************************************/

/**This function gets a city and returns a char* pointing to its name
 If the city is null then it returns null*/

char* GetCityName(City);

/*********************************************************************/

/**This function gets a city and returns its city id*/

int GetCityID(City);

/*********************************************************************/

/**This function gets a city and returns the UniqueOrderedList citizens
 which is a field in the city.
 If the city is null or if UniqueOrderedList is null then it returns null*/

UniqueOrderedList GetCitizensInCity(City);

/*********************************************************************/

/**This function gets a city and returns the UniqueOrderedList citizens
 which is a field in the city.
 If the city is null or if UniqueOrderedList is null then it returns null*/

/*********************************************************************/

/**This function gets a city and returns the UniqueOrderedList city_candidates
 which is a field in the city.
 If the city is null or if UniqueOrderedList is null then it returns null*/

UniqueOrderedList GetCityCandidates(City);

/*********************************************************************/

/**This function gets a city and returns the UniqueOrderedList candidates
 which is a field in the city.
 If the city is null or if UniqueOrderedList is null then it returns null*/

UniqueOrderedList GetCandidates(City);

/**This function gets a city and a const char* name and grants the name to the
 city . We use this function only in the function MtmElectionAddCity
 and until we reach this function we would have already checked if the
 name is null. So there is no need to check that in the function*/

void SetCityName(City, const char*);

/**This function gets a city and a const char* name and grants the name to the
 city . We use this function only in the function MtmElectionAddCity*/
void SetCityID(City, int);

#endif /* CITY_H_ */

