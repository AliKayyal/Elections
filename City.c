#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "Citizen.h"
#include "City.h"
#include "CityCandidate.h"

struct city_t {
	UniqueOrderedList city_candidates;
	UniqueOrderedList candidates;
	UniqueOrderedList citizens;
	char* cityName;
	int cityId;
};

bool CityElementsEqual(City city1, City city2) {
	if (city1 == NULL || city2 == NULL) {
		return false;
	}
	if (city1->cityId == city2->cityId) {
		return true;
	}
	return false;
}

bool CityElementGreaterThan(City city1, City city2) {
	if (city1 == NULL || city2 == NULL) {
		return false;
	}
	if (city1->cityId > city2->cityId) {
		return true;
	}
	return false;
}

City CityCopyElements(City city) {
	if (city == NULL) {
		return NULL;
	}
	City copy = malloc(sizeof(*copy));
	if (copy == NULL) {
		return NULL;
	}
	copy->cityName = malloc(sizeof(char) * (strlen(city->cityName) + 1));
	if (copy->cityName == NULL) {
		free(copy);
		return NULL;
	}
	strcpy(copy->cityName, city->cityName);
	copy->cityId = city->cityId;
	copy->citizens = uniqueOrderedListCopy(city->citizens);
	if (copy->citizens == NULL) {
		free(copy);
		return NULL;
	}
	copy->city_candidates = uniqueOrderedListCopy(city->city_candidates);
	if (copy->city_candidates == NULL) {
		uniqueOrderedListDestroy(copy->citizens);
		free(copy);
		return NULL;
	}
	copy->candidates = uniqueOrderedListCopy(city->candidates);
	if (copy->candidates == NULL) {
		uniqueOrderedListDestroy(copy->citizens);
		uniqueOrderedListDestroy(copy->city_candidates);
		free(copy);
		return NULL;
	}
	return copy;
}

void CityFreeElements(City city) {
	free(city->cityName);
	uniqueOrderedListDestroy(city->citizens);
	uniqueOrderedListDestroy(city->city_candidates);
	uniqueOrderedListDestroy(city->candidates);
	free(city);
}

City GetCityWhereCitizenExists(int citizenId, UniqueOrderedList cities) {
	if (cities == NULL) {
		return NULL;
	}
	if (&citizenId == NULL) {
		return NULL;
	}
	City iterator = uniqueOrderedListGetLowest(cities);
	int size = uniqueOrderedListSize(cities);
	while (size > 0) {
		if (GetCitizen(citizenId, iterator->citizens) != NULL) {
			return iterator;
		}
		iterator = uniqueOrderedListGetNext(cities);
		size--;
	}
	return NULL;
}
bool CityContained(UniqueOrderedList cities, City city) {
	if (uniqueOrderedListContains(cities, city)) {
		free(city->cityName);
		free(city);
		return true;
	}
	return false;

}

bool CityNameMallocFails(City city) {
	if (city->cityName == NULL) {
		free(city);
		return true;
	}
	return false;
}

bool CityCandidatesFailed(City city) {
	city->city_candidates = uniqueOrderedListCreate(
			(copyElements) & CityCandidateCopyElements,
			(freeElements) & CityCandidateFreeElements,
			(elementsEquals) & CityCandidateElementsEqual,
			(elementGreaterThan) & CityCandidateElementGreaterThan);
	if (city->city_candidates == NULL) {
		free(city->cityName);
		free(city);
		return true;
	}
	return false;
}

bool CandidatesFailed(City city) {
	city->candidates = uniqueOrderedListCreate(
			(copyElements) & CitizenCopyElements,
			(freeElements) & CitizenFreeElements,
			(elementsEquals) & CitizenElementsEqual,
			(elementsEquals) & CitizenElementGreaterThan);
	if (city->candidates == NULL) {
		uniqueOrderedListDestroy(city->city_candidates);
		free(city->cityName);
		free(city);
		return true;
	}
	return false;
}

bool CitizensFailed(City city) {
	city->citizens = uniqueOrderedListCreate(
			(copyElements) & CitizenCopyElements,
			(freeElements) & CitizenFreeElements,
			(elementsEquals) & CitizenElementsEqual,
			(elementsEquals) & CitizenElementGreaterThan);
	if (city->citizens == NULL) {
		uniqueOrderedListDestroy(city->city_candidates);
		uniqueOrderedListDestroy(city->candidates);
		free(city->cityName);
		free(city);
		return true;
	}
	return false;
}

bool CityInsertFailed(UniqueOrderedList cities, City city) {
	if (uniqueOrderedListInsert(cities, city) != UNIQUE_ORDERED_LIST_SUCCESS) {
		uniqueOrderedListDestroy(city->candidates);
		uniqueOrderedListDestroy(city->city_candidates);
		uniqueOrderedListDestroy(city->citizens);
		free(city->cityName);
		free(city);
		return true;
	}
	return false;
}

City GetCityThroughID(int cityId, UniqueOrderedList cities) {
	City current = uniqueOrderedListGetLowest(cities);
	int size = uniqueOrderedListSize(cities);
	while (size > 0) {
		if (current->cityId == cityId) {
			return current;
		}
		current = uniqueOrderedListGetNext(cities);
		size--;
	}
	return NULL;
}

char* GetCityName(City city) {
	if (city == NULL) {
		return NULL;
	}
	return city->cityName;
}

int GetCityID(City city) {
	return city->cityId;
}

UniqueOrderedList GetCitizensInCity(City city) {
	if (city == NULL) {
		return NULL;
	}
	if (city->citizens == NULL) {
		return NULL;
	}
	return city->citizens;
}

UniqueOrderedList GetCityCandidates(City city) {
	return city->city_candidates;
}

UniqueOrderedList GetCandidates(City city) {
	return city->candidates;
}
void SetCityName(City city, const char*name) {
	city->cityName = malloc(sizeof(char) * (strlen(name) + 1));
	if (city->cityName == NULL) {
		return;
	}
	strcpy(city->cityName, name);
}

void SetCityID(City city, int id) {
	city->cityId = id;

}
