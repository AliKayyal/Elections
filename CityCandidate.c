#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "CityCandidate.h"

struct citycandidate_t {
	char* name;
	int candidateID;
	int votes;
};

bool CityCandidateElementsEqual(CityCandidate candidate1,
		CityCandidate candidate2) {
	if (candidate1 == NULL || candidate2 == NULL) {
		return false;
	}
	if (candidate1->candidateID == candidate2->candidateID) {
		return true;
	}
	return false;
}

bool CityCandidateElementGreaterThan(CityCandidate candidate1,
		CityCandidate candidate2) {
	if (candidate1 == NULL || candidate2 == NULL) {
		return false;
	}
	if ((candidate1->votes) > (candidate2->votes)) {
		return true;
	}
	if ((candidate2->votes) > (candidate1->votes)) {
		return false;
	}
	int compare_result = strcmp(candidate1->name, candidate2->name);
	if (compare_result < 0) {
		return true;
	}
	if (compare_result > 0) {
		return false;
	}
	if ((candidate1->candidateID) > (candidate2->candidateID)) {
		return true;
	}
	return false;
}

CityCandidate CityCandidateCopyElements(CityCandidate candidate) {
	if (candidate == NULL) {
		return NULL;
	}
	CityCandidate copy = malloc(sizeof(*copy));
	if (copy == NULL) {
		return NULL;
	}
	copy->name = malloc(sizeof(char) * (strlen(candidate->name) + 1));
	if (copy->name == NULL) {
		free(copy);
		return NULL;
	}
	strcpy(copy->name, candidate->name);
	copy->candidateID = candidate->candidateID;
	copy->votes = candidate->votes;
	return copy;
}

void CityCandidateFreeElements(CityCandidate candidate) {
	free(candidate->name);
	free(candidate);
}

CityCandidate GetCityCandidate(int candidateId,
		UniqueOrderedList city_candidates) {
	if (&candidateId == NULL || city_candidates == NULL) {
		return NULL;
	}
	int size = uniqueOrderedListSize(city_candidates);
	CityCandidate current_candidate = uniqueOrderedListGetLowest(
			city_candidates);
	while (size > 0) {
		if (current_candidate->candidateID == candidateId)
			return current_candidate;
		current_candidate = uniqueOrderedListGetNext(city_candidates);
		size--;
	}
	return NULL;
}

void CityCandidateSetName(CityCandidate candidate, char* name) {
	candidate->name = malloc(sizeof(char) * ((strlen(name)) + 1));
	if (candidate->name == NULL) {
		return;
	}
	strcpy(candidate->name, name);
}

int CityCandidateGetID(CityCandidate candidate) {
	return candidate->candidateID;
}
void CityCandidateSetID(CityCandidate candidate, int id) {
	candidate->candidateID = id;
}

char* CityCandidateGetName(CityCandidate candidate) {
	return candidate->name;
}

void CityCandidateSetVotes(CityCandidate candidate, int votes) {
	candidate->votes = votes;

}
