#ifndef CITYCANDIDATE_H_
#define CITYCANDIDATE_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"

/***************************************************************************/

/**This typdef struct has the id and the name of a certain candidate and how
 many votes he got. This element would be inserted in the UniqueOrderedList
 city_candidates*/

typedef struct citycandidate_t *CityCandidate;

/******************************************************************************/

/**This function gets a CityCandidate and frees up all of the fields inside it
 and frees it also from the memory*/

void CityCandidateFreeElements(CityCandidate candidate);

/******************************************************************************/

/**This function gets a CityCandidate and copies all of the fields inside
 it and returns a copy of the candidate passed to it*/

CityCandidate CityCandidateCopyElements(CityCandidate);

/******************************************************************************/

/**This function gets two CityCandidates and checks if they are the same
 according to their ids
 If their ids are the same then it returns true.
 Else it return false*/

bool CityCandidateElementsEqual(CityCandidate, CityCandidate);

/******************************************************************************/

/**This function gets two CityCandidates compares if one of the
 candidates has a greater value in the order in the unique UniqueOrderedList
 city_candidates.
 Firstly it compares the votes given to each one of them, the candidate
 granted more votes sets higher in the list.
 If the votes are equal then it compares their name lexicographically.
 If the candidate has a smaller name lexicographically then
 he sits higher in the list.
 If the name are the same then it compares their ids. If the candidate has
 a greater id then he sits higher in the list.*/

bool CityCandidateElementGreaterThan(CityCandidate, CityCandidate);

/*****************************************************************************/

/**This function gets a candidate id an the UniqueOrderedList city_candidates
 and searches or the candidate in the list holding the particular id and
 returns it. If he does not exist then it returns null*/

CityCandidate GetCityCandidate(int, UniqueOrderedList);

/*****************************************************************************/

/**This function gets a CityCandidate an a char* name and allocates a name for
 the CityCandidate and copies the given name into it */

void CityCandidateSetName(CityCandidate, char*);

/*****************************************************************************/

/**This function  gets a CityCandidate and returns the id of the CityCandidate*/

int CityCandidateGetID(CityCandidate);

/*****************************************************************************/

/**This function  gets a CityCandidate and an int id and sets the candidate id
 to hold that given id*/

void CityCandidateSetID(CityCandidate, int);

/*****************************************************************************/

/**This function  gets a CityCandidate and returns his name as a char**/

char* CityCandidateGetName(CityCandidate);

/*****************************************************************************/

/**This function  gets a CityCandidate and an int votes an sets the field
 of the votes that he got in the election to hold the value of the
 parameter votes*/

void CityCandidateSetVotes(CityCandidate, int votes);

#endif /* CITYCANDIDATE_H_ */
