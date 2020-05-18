#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "CitizenCandidate.h"

struct citizencandidate_t {
	int candidateId;
	int priority;
};

bool CitizenCandidateElementsEqual(CitizenCandidate candidate1,
		CitizenCandidate candidate2) {
	if (candidate1 == NULL || candidate2 == NULL) {
		return false;
	}
	if (candidate1->candidateId == candidate2->candidateId) {
		return true;
	}
	return false;
}
void CitizenCandidateFreeElements(CitizenCandidate candidate) {
	free(candidate);
}
CitizenCandidate CitizenCandidateCopyElements(CitizenCandidate candidate) {
	if (candidate == NULL) {
		return NULL;
	}
	CitizenCandidate copy = malloc(sizeof(*copy));
	if (copy == NULL) {
		return NULL;
	}
	copy->candidateId = candidate->candidateId;
	copy->priority = candidate->priority;
	return copy;
}

bool CitizenCandidateElementGreaterThan(CitizenCandidate candidate1,
		CitizenCandidate candidate2) {
	if (candidate1 == NULL || candidate2 == NULL) {
		return false;
	}
	if (candidate1->priority > candidate2->priority) {
		return true;
	}
	return false;
}
bool CandidateSupported(int candidateId, UniqueOrderedList citizen_candidates) {
	if (&candidateId == NULL || citizen_candidates == NULL) {
		return false;
	}
	CitizenCandidate candidate = uniqueOrderedListGetLowest(citizen_candidates);
	int size = uniqueOrderedListSize(citizen_candidates);
	while (size > 0) {
		if (candidate != NULL) {
			if (CitizenCandidateGetID(candidate) == candidateId) {
				return true;
			}
		}
		candidate = uniqueOrderedListGetNext(citizen_candidates);
		size--;
	}
	return false;
}

CitizenCandidate GetCitizenCandidate(int candidateId,
		UniqueOrderedList citizen_candidates) {
	if (&candidateId == NULL || citizen_candidates == NULL) {
		return NULL;
	}
	CitizenCandidate temporary = uniqueOrderedListGetLowest(citizen_candidates);
	int size = uniqueOrderedListSize(citizen_candidates);
	while (size > 0) {
		if (temporary->candidateId == candidateId) {
			return temporary;
		}
		temporary = uniqueOrderedListGetNext(citizen_candidates);
		size--;
	}
	return NULL;
}

int GetPriority(CitizenCandidate candidate) {
	return candidate->priority;
}
void SetPriority(CitizenCandidate candidate, int priority) {
	candidate->priority = priority;
}
void SetCitizenCandidateId(CitizenCandidate candidate, int id) {
	candidate->candidateId = id;
}
int CitizenCandidateGetID(CitizenCandidate candidate) {
	return candidate->candidateId;
}
