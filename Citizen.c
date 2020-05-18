#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "Citizen.h"
#include "City.h"
#include "CitizenCandidate.h"

struct citizen_t {
	char *name;
	int age;
	int unique_id;
	int education_years;
	UniqueOrderedList citizen_candidates;
};

bool CitizenElementsEqual(Citizen citizen1, Citizen citizen2) {
	if (citizen1 == NULL || citizen2 == NULL) {
		return false;
	}
	if (citizen1->unique_id == citizen2->unique_id) {
		return true;
	}
	return false;
}

bool CitizenElementGreaterThan(Citizen citizen1, Citizen citizen2) {
	if (citizen1 == NULL || citizen2 == NULL) {
		return false;
	}
	if (citizen1->unique_id > citizen2->unique_id) {
		return true;
	}
	return false;
}

Citizen CitizenCopyElements(Citizen citizen) {
	if (citizen == NULL) {
		return NULL;
	}
	Citizen copy = malloc(sizeof(*copy));
	if (copy == NULL) {
		return NULL;
	}
	copy->name = malloc(sizeof(char) * (strlen(citizen->name) + 1));
	if (copy->name == NULL) {
		free(copy);
		return NULL;
	}
	strcpy(copy->name, citizen->name);
	copy->unique_id = citizen->unique_id;
	copy->age = citizen->age;
	copy->education_years = citizen->education_years;
	copy->citizen_candidates = uniqueOrderedListCopy(
			citizen->citizen_candidates);
	if (copy->citizen_candidates == NULL) {
		free(copy->name);
		free(copy);
		return NULL;
	}
	return copy;
}

void CitizenFreeElements(Citizen citizen) {
	free(citizen->name);
	uniqueOrderedListDestroy(citizen->citizen_candidates);
	free(citizen);
}

bool CandidateElementsEqual(Citizen candidate1, Citizen candidate2) {
	if (candidate1 == NULL || candidate2 == NULL) {
		return false;
	}
	if (candidate1->unique_id == candidate2->unique_id) {
		return true;
	}
	return false;
}

bool CandidateElementGreaterThan(Citizen candidate1, Citizen candidate2) {
	return CitizenElementGreaterThan(candidate1, candidate2);
}

Citizen CandidateCopyElements(Citizen candidate) {
	return CitizenCopyElements(candidate);
}

void CandidateFreeElements(Citizen candidate) {
	CitizenFreeElements(candidate);
}

Citizen GetCitizen(int citizenId, UniqueOrderedList citizens) {
	if (citizens == NULL) {
		return NULL;
	}
	if (&citizenId == NULL) {
		return NULL;
	}
	int size = uniqueOrderedListSize(citizens);
	Citizen current_citizen = uniqueOrderedListGetLowest(citizens);
	while (size > 0) {
		if (current_citizen->unique_id == citizenId) {
			return current_citizen;
		}
		current_citizen = uniqueOrderedListGetNext(citizens);
		size--;
	}
	return NULL;
}

bool PriorityExists(int priority, UniqueOrderedList citizen_candidates) {
	if (&priority == NULL || citizen_candidates == NULL) {
		return false;
	}
	int size = uniqueOrderedListSize(citizen_candidates);
	CitizenCandidate candidate = uniqueOrderedListGetLowest(citizen_candidates);
	while (size > 0) {
		if (candidate != NULL) {
			if (GetPriority(candidate) == priority) {
				return true;
			}
		}
		candidate = uniqueOrderedListGetNext(citizen_candidates);
		size--;
	}
	return false;
}

Citizen CitizenExistInSystem(int citizenId, UniqueOrderedList cities) {
	if (cities == NULL || &citizenId == NULL) {
		return NULL;
	}
	int size = uniqueOrderedListSize(cities);
	City current = uniqueOrderedListGetLowest(cities);
	while (size > 0) {
		Citizen temporary = GetCitizen(citizenId, GetCitizensInCity(current));
		if (temporary != NULL) {
			return temporary;
		}
		current = uniqueOrderedListGetNext(cities);
		size--;
	}
	return NULL;
}

Citizen GetCandidate(int candidateId, UniqueOrderedList candidates) {
	return GetCitizen(candidateId, candidates);
}

Citizen CandidateExistsInSystem(int candidateId, UniqueOrderedList candidates) {
	return GetCitizen(candidateId, candidates);
}

bool CitizenCandidatesFailed(Citizen citizentoadd) {
	citizentoadd->citizen_candidates = uniqueOrderedListCreate(
			(copyElements) & CitizenCandidateCopyElements,
			(freeElements) & CitizenCandidateFreeElements,
			(elementsEquals) & CitizenCandidateElementsEqual,
			(elementGreaterThan) & CitizenCandidateElementGreaterThan);
	if (citizentoadd->citizen_candidates == NULL) {
		free(citizentoadd);
		return true;
	}
	return false;
}

bool CitizenNameMallocFails(Citizen citizentoadd) {
	if (citizentoadd->name == NULL) {
		uniqueOrderedListDestroy(citizentoadd->citizen_candidates);
		free(citizentoadd);
		return true;
	}
	return false;
}

bool CitizenInsertFailed(UniqueOrderedList citizens, Citizen citizentoadd) {
	if (uniqueOrderedListInsert(citizens, citizentoadd)
			!= UNIQUE_ORDERED_LIST_SUCCESS) {
		uniqueOrderedListDestroy(citizentoadd->citizen_candidates);
		free(citizentoadd->name);
		free(citizentoadd);
		return true;
	}
	return false;
}

int ReturnCitizenAge(Citizen citizen) {
	return citizen->age;
}

char* ReturnCitizenName(Citizen citizen) {
	return citizen->name;
}

int ReturnCitizenId(Citizen citizen) {
	return citizen->unique_id;
}
UniqueOrderedList ReturnCitizenCandidates(Citizen citizen) {
	return citizen->citizen_candidates;
}
int ReturnCitizenEducationYears(Citizen citizen) {
	return citizen->education_years;
}
void SetCitizenName(Citizen citizen, const char *name) {
	citizen->name = malloc(sizeof(const char) * (strlen(name) + 1));
	if (citizen->name == NULL) {
		return;
	}
	strcpy(citizen->name, name);
}

void SetCitizenAge(Citizen citizen, int age) {
	citizen->age = age;
}

void SetCitizenID(Citizen citizen, int id) {
	citizen->unique_id = id;
}

void SetCitizenEduactionYears(Citizen citizen, int educationyears) {
	citizen->education_years = educationyears;
}
