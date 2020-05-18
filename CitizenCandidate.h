#ifndef CITIZENCANDIDATE_H_
#define CITIZENCANDIDATE_H_
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"

/**The typdef struct CitizenCandidate contains the information of a specific
 candidate that a specific citizen supports.
 this struct has two fields candidateId and priority.
 priority describes how much a citizen suports this candidate
 (the lower the better) and candidateId is the Id of the candidate supported
 Note that this is an element of the uniqueOrderedList citizen_candidates*/

typedef struct citizencandidate_t *CitizenCandidate;

/*****************************************************************************/

/**All of the function: CitizenCandidateElementsEqual,
 CitizenCandidateElementGreaterThan, CitizenCandidateCopyElements and
 CitizenCandidateFreeElements are "helpers"
 that we use to compare,insert and remove elements from the
 UniqueOrderedListcities
 according to certain standard fixed to each struct and how they
 should be ordered in the UniqueOrderedLists.*/

/**This function gets a Citizen Candidate and frees all of its fields
 accordingly (the fields are mentioned in the C file) */

void CitizenCandidateFreeElements(CitizenCandidate);

/**this function gets two candidates that a certain citizen supports and
 then compares between two candidates that the citizen supports,
 if the priority of the first candidate.
 is higher than the priority of the second candidate then it returns true,
 otherwise its false (we wanted the candidates to be organized from highest
 (the least preferred)  priority to the lowest (highest preferred)
 */
bool CitizenCandidateElementGreaterThan(CitizenCandidate, CitizenCandidate);

/**this function get a candidate that the citizen supports and then
 copies all the fields (mentioned in c file) inside to a new copy of the
 structure CitizenCandidate
 */
CitizenCandidate CitizenCandidateCopyElements(CitizenCandidate);

/**gets two candidates that the citizen supports and then checks if they have
 the same id number, if they do it return true otherwise it returns false */
bool CitizenCandidateElementsEqual(CitizenCandidate, CitizenCandidate);

/*****************************************************************************/

/**This function searches for a candidate that is supported by a certain
 citizen. It searches in the UniqueOrderedList citizen_candidates
 (belongs to the certain citizen).If the candidate exists in the list then it
 returns it. Else it return NULL.(The difference between this function and
 CandidateSupported is that it returns a CitizenCandidate which we use in other
 functions) */

CitizenCandidate GetCitizenCandidate(int, UniqueOrderedList);

/*****************************************************************************/

/**This function checks if a certain candidate is already supported by a certain
 citizen.It searches in the UniqueOrderedList citizen_candidates
 (belongs to the certain citizen) and finds if the candidate is already
 supported or not*/

bool CandidateSupported(int, UniqueOrderedList);

/**this function gets a candidate that the citizen prefers and returns the
 priority (how much he supports him,the lower the better) of this candidate
 for him */
int GetPriority(CitizenCandidate);

/**this function gets a candidate that the citizen prefers and sets the
 priority (how much he supports him,the lower the better) of this candidate
 in the struct mentioned above in the appropriate priority field */
void SetPriority(CitizenCandidate, int);

/**this function gets a candidate that the citizen prefers and sets the
 id  of this candidate  in the struct mentioned above in the appropriate
 id field */
void SetCitizenCandidateId(CitizenCandidate, int);

/**this function gets a candidate that the citizen prefers and returns the
 id number of this candidate */
int CitizenCandidateGetID(CitizenCandidate);

#endif /* CITIZENCANDIDATE_H_ */
