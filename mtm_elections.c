#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "mtm_print.h"
#include "mtm_elections.h"
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "City.h"
#include "Citizen.h"
#include "CitizenCandidate.h"
#include "CityCandidate.h"

#define VOTER_AGE 17
#define CANDIDATE_AGE 21

struct citycandidate_t {
	char* name;
	int candidateID;
	int votes;
};

struct city_t {
	UniqueOrderedList city_candidates;
	UniqueOrderedList candidates;
	UniqueOrderedList citizens;
	char* cityName;
	int cityId;
};

struct citizen_t {
	char *name;
	int age;
	int unique_id;
	int education_years;
	UniqueOrderedList citizen_candidates;
};

struct citizencandidate_t {
	int candidateId;
	int priority;
};
bool MayorElementsEqual( Citizen, Citizen);
bool MayorElementGreaterThan( Citizen, Citizen);
Citizen MayorCopyElements( Citizen);
void MayorFreeElements( Citizen);

/**This function is used in the insertion of elements in the uniqueorderedList
 created and returned in the function MtmElectionsPerformElections,
 it checks if two mayor ids are the same, if this happens then it returns true
 else it returns false*/
bool MayorIdElementsEqual(int* mayor1, int* mayor2) {
	if (*mayor1 == *mayor2) {
		return true;
	}
	return false;
}
/**This function is used in the insertion of elements in the uniqueorderedList
 *  created and returned in the function
 MtmElectionsPerformElections, it gets a UniqueOrderedList mayor
 which is already ordered as it should be (which
 we created in the mtmElections)
 that's why this function always returns false*/

bool MayorIdElementsGreaterThan(int* mayor1, int* mayor2) {
	return false;
}

/**This function frees the id of the mayor in the UniqueOrderedList mayors_ids*/

void MayorIdFreeElements(int* mayor) {
	free(mayor);
}

/**This function copies the id of the mayor in the UniqueOrderedList mayors_ids
 and returns a copy of this id*/

int* MayorIdCopyElements(int* mayor) {

	int* copy = malloc(sizeof(int));
	*copy = *mayor;
	return copy;
}

/**This function gets two mayors and checks if both of their ids are the same
 If this happens then it returns true.
 Else it returns false*/

bool MayorElementsEqual(Citizen mayor1, Citizen mayor2) {
	if (mayor1->unique_id == mayor2->unique_id) {
		return true;
	}
	return false;
}

/**This function gets two mayors and compares which one should be higher in the
 uniqueorderedlist mayors in the mtmElections.
 Firstly it compares their names lexicographically, if the name of the
 first mayor comes first lexicographically then he is lower in the list,
 if the name of the first mayor comes last lexicographically then he is higher
 in the list. If the names are the same then the mayor with the smallest id
 is higher in the list */

bool MayorElementGreaterThan(Citizen mayor1, Citizen mayor2) {

	int compare_result = strcmp(mayor1->name, mayor2->name);
	if (compare_result < 0) {
		return false;
	}
	if (compare_result > 0) {
		return true;
	}
	if (mayor1->unique_id > mayor2->unique_id) {
		return false;
	}
	return true;
}

/**This function gets one mayor and copies all of the fields in the mayor
 using the function
 CitizenCopyElements*/

Citizen MayorCopyElements(Citizen mayor) {
	return CitizenCopyElements(mayor);
}

/**This function gets one mayor and frees  up all of the fields in the mayor
 *  using the function
 CitizenFreeElements*/

void MayorFreeElements(Citizen mayor) {
	CitizenFreeElements(mayor);
}

/**This strcut is where everything gets connected together.
 It has three UniqueOrderedLists:
 1-Cities which has all of the cities in the system. Which are all struct of the
 data type City.
 2-Mayors which includes all of the citizens that are mayors of all of the
 cities in the system
 */
struct mtm_elections_t {
	UniqueOrderedList cities;
	UniqueOrderedList mayors;
};

/**This functions calculates the votes given to a certain candidate.
 Note that here we don't use the RankFunc.
 At the end it creates a CityCandidate and inserts it in the UniqueOrderedList
 city_candidates.
 */
void VotesForEachCandidate(MtmElections mtmElections, int candidateId,
		UniqueOrderedList citizens, int cityId) {
	Citizen temporary = uniqueOrderedListGetLowest(citizens);
	int size = uniqueOrderedListSize(citizens);
	int counter = 0;
	while (size > 0) {
		CitizenCandidate candidate = uniqueOrderedListGetLowest(
				ReturnCitizenCandidates(temporary));
		if (candidate != NULL) {
			if (CitizenCandidateGetID(candidate) == candidateId
					&& (ReturnCitizenAge(temporary) >= VOTER_AGE)) {
				counter++;
			}
		}
		temporary = uniqueOrderedListGetNext(citizens);
		size--;
	}
	CityCandidate candidate_to_add = malloc(sizeof(*candidate_to_add));
	if (candidate_to_add == NULL) {
		return;
	}
	City this_city = GetCityThroughID(cityId, mtmElections->cities);
	Citizen this_candidate = GetCitizen(candidateId,
			GetCitizensInCity(this_city));
	CityCandidateSetName(candidate_to_add, ReturnCitizenName(this_candidate));
	if (CityCandidateGetName(candidate_to_add) == NULL) {
		free(candidate_to_add);
		return;
	}
	CityCandidateSetID(candidate_to_add, candidateId);
	CityCandidateSetVotes(candidate_to_add, counter);
	uniqueOrderedListInsert(GetCityCandidates(this_city), candidate_to_add);
	CityCandidateFreeElements(candidate_to_add);
}

/**This functions calculates the votes given to a certain candidate.
 Note that here we use the RankFunc.
 At the end it creates a CityCandidate and inserts it in the UniqueOrderedList
 city_candidates.
 If a null argument ws passed or if a certain fault was detected then it returns
 null
 */
void VotesForEachCandidateWithRankFunc(MtmElections mtmElections,
		int candidateId, UniqueOrderedList citizens, int cityId, RankFunc rank,
		void* auxilaryData) {
	Citizen temporary = uniqueOrderedListGetLowest(citizens);
	int size = uniqueOrderedListSize(citizens);
	int counter = 0;
	while (size > 0) {
		CitizenCandidate candidate = uniqueOrderedListGetLowest(
				ReturnCitizenCandidates(temporary));
		if (candidate != NULL) {
			if (CitizenCandidateGetID(candidate) == candidateId) {
				counter += rank(mtmElections, ReturnCitizenId(temporary),
						auxilaryData);
			}
		}
		temporary = uniqueOrderedListGetNext(citizens);
		size--;

	}
	CityCandidate candidate_to_add = malloc(sizeof(*candidate_to_add));
	if (candidate_to_add == NULL) {
		return;
	}
	City this_city = GetCityThroughID(cityId, mtmElections->cities);
	Citizen this_candidate = GetCitizen(candidateId,
			GetCitizensInCity(this_city));
	CityCandidateSetName(candidate_to_add, ReturnCitizenName(this_candidate));
	if (CityCandidateGetName(candidate_to_add) == NULL) {
		free(candidate_to_add);
		return;
	}
	CityCandidateSetID(candidate_to_add, candidateId);
	CityCandidateSetVotes(candidate_to_add, counter);
	uniqueOrderedListInsert(GetCityCandidates(this_city), candidate_to_add);
	CityCandidateFreeElements(candidate_to_add);
}

/**This function calculates the votes for every candidate in a certain city
 using the function VotesForEachCandidate.Note that here we don't use the
 RankFunc At the end it returns the mayor of the city as a Citizen
 (all of the fields of the citizen).
 If a null argument ws passed or if a certain fault was detected then
 it returns null*/

Citizen WinnerInCity(MtmElections mtmElections, int cityId, int* mayor) {
	if (mtmElections == NULL || &cityId == NULL || mayor == NULL) {
		return NULL;
	}
	if (cityId < 0) {
		return NULL;
	}
	City current = GetCityThroughID(cityId, mtmElections->cities);
	if (current == NULL) {
		return NULL;
	}
	uniqueOrderedListClear(GetCityCandidates(current));
	int size = uniqueOrderedListSize(GetCandidates(current));
	if (size == 0) {
		return NULL;
	}
	Citizen candidate = uniqueOrderedListGetLowest(GetCandidates(current));
	while (size > 0) {
		VotesForEachCandidate(mtmElections, ReturnCitizenId(candidate),
				GetCitizensInCity(current), cityId);
		candidate = uniqueOrderedListGetNext(GetCandidates(current));
		size--;
	}
	CityCandidate winner = uniqueOrderedListGetGreatest(
			GetCityCandidates(current));
	if (winner != NULL) {
		*mayor = CityCandidateGetID(winner);
		Citizen winner_info = GetCitizen(CityCandidateGetID(winner),
				GetCitizensInCity(current));
		return winner_info;
	}
	return NULL;
}

/**This function calculates the votes for every candidate in a certain city
 using the function VotesForEachCandidate.Note that here we use the RankFunc
 At the end it returns the mayor of the city as a Citizen (all of the fields of
 the citizen)
 If a null argument ws passed or if a certain fault was detected then it returns
 null*/

Citizen WinnerInCityWithRankFunc(MtmElections mtmElections, int cityId,
		int* mayor, RankFunc rank, void* auxilaryData) {
	if (mtmElections == NULL || &cityId == NULL || &mayor == NULL) {
		return NULL;
	}
	if (cityId < 0) {
		return NULL;
	}
	City current = GetCityThroughID(cityId, mtmElections->cities);
	if (current == NULL) {
		return NULL;
	}
	uniqueOrderedListClear(GetCityCandidates(current));
	int size = uniqueOrderedListSize(GetCandidates(current));
	if (size == 0) {
		return NULL;
	}
	Citizen candidate = uniqueOrderedListGetLowest(GetCandidates(current));
	while (size > 0) {
		VotesForEachCandidateWithRankFunc(mtmElections,
				ReturnCitizenId(candidate), GetCitizensInCity(current), cityId,
				rank, auxilaryData);
		candidate = uniqueOrderedListGetNext(GetCandidates(current));
		size--;
	}
	CityCandidate winner = uniqueOrderedListGetGreatest(
			GetCityCandidates(current));
	if (winner != NULL) {
		*mayor = CityCandidateGetID(winner);
		Citizen winner_info = GetCitizen(CityCandidateGetID(winner),
				GetCitizensInCity(current));
		return winner_info;
	}
	return NULL;
}

/**This function is used in the function MtmElectionsPerformElections it
 inserts the ids of all of the mayors of the cities in the system
 to the UniqueOrderedList mayors_ids which we create in that function*/
void ReturnMayorsOfCities(MtmElections mtmElections,
		UniqueOrderedList mayors_ids) {
	int nr_of_mayors = uniqueOrderedListSize(mtmElections->mayors);
	Citizen mayor_to_add = uniqueOrderedListGetLowest(mtmElections->mayors);
	while (nr_of_mayors > 0) {
		int to_add = ReturnCitizenId(mayor_to_add);
		uniqueOrderedListInsert(mayors_ids, &to_add);
		mayor_to_add = uniqueOrderedListGetNext(mtmElections->mayors);
		nr_of_mayors--;
	}
}

/**This function describes the behaviour of MtmElectionsPerformElections
 *  in the case that both the Rankfunc was null
 and the opening of the file failed.
 It uses the function WinnerInCity because rankfunc is null*/
void FileIsNullAndRankFuncIsNull(MtmElections mtmElections, int size,
		City current) {
	int mayor_of_city;
	while (size > 0) {
		Citizen temporary = WinnerInCity(mtmElections, GetCityID(current),
				&mayor_of_city);
		uniqueOrderedListInsert(mtmElections->mayors, temporary);
		current = uniqueOrderedListGetNext(mtmElections->cities);
		size--;
	}
}

/**This function describes the behaviour of MtmElectionsPerformElections
 * in the case that the Rankfunc is not null
 but the opening of the file failed.
 It uses the function WinnerInCityWithRankFunc*/
void FileIsNullWithRankFunc(MtmElections mtmElections, int size, City current,
		RankFunc rank, void* auxilaryData) {
	int mayor_of_city;
	while (size > 0) {
		Citizen temporary = WinnerInCityWithRankFunc(mtmElections,
				GetCityID(current), &mayor_of_city, rank, auxilaryData);
		uniqueOrderedListInsert(mtmElections->mayors, temporary);
		current = uniqueOrderedListGetNext(mtmElections->cities);
		size--;
	}
}

/**This function describes the behaviour of MtmElectionsPerformElections
 * in the case that the Rankfunc is  null
 but the opening of the file succeeds.
 It uses the function WinnerInCity*/

void FileOpensAndRankFuncIsNull(MtmElections mtmElections, int size,
		City current, FILE*fp) {
	int mayor_of_city;
	while (size > 0) {
		Citizen temporary = WinnerInCity(mtmElections, GetCityID(current),
				&mayor_of_city);
		uniqueOrderedListInsert(mtmElections->mayors, temporary);
		current = uniqueOrderedListGetNext(mtmElections->cities);
		size--;

	}
	int size2 = uniqueOrderedListSize(mtmElections->mayors);
	Citizen current_mayor = uniqueOrderedListGetLowest(mtmElections->mayors);
	while (size2 > 0) {
		City city_of_mayor = GetCityWhereCitizenExists(
				ReturnCitizenId(current_mayor), mtmElections->cities);
		mtmPrintMayorDetails(ReturnCitizenId(current_mayor),
				ReturnCitizenAge(current_mayor),
				ReturnCitizenEducationYears(current_mayor),
				ReturnCitizenName(current_mayor), GetCityName(city_of_mayor),
				GetCityID(city_of_mayor), fp);
		current_mayor = uniqueOrderedListGetNext(mtmElections->mayors);
		size2--;
	}
}

/**This function describes the behaviour of MtmElectionsPerformElections
 * in the case that the Rankfunc is not null
 and the opening of the file succeeds.
 It uses the function WinnerInCityWithRankFunc*/

void FileOpensWithRankFunc(MtmElections mtmElections, int size, City current,
		RankFunc rank, void* auxilaryData, FILE*fp) {
	int mayor_of_city;
	while (size > 0) {
		Citizen temporary = WinnerInCityWithRankFunc(mtmElections,
				GetCityID(current), &mayor_of_city, rank, auxilaryData);
		uniqueOrderedListInsert(mtmElections->mayors, temporary);
		current = uniqueOrderedListGetNext(mtmElections->cities);
		size--;
	}
	int size2 = uniqueOrderedListSize(mtmElections->mayors);
	Citizen current_mayor = uniqueOrderedListGetLowest(mtmElections->mayors);
	while (size2 > 0) {
		City city_of_mayor = GetCityWhereCitizenExists(
				ReturnCitizenId(current_mayor), mtmElections->cities);
		mtmPrintMayorDetails(ReturnCitizenId(current_mayor),
				ReturnCitizenAge(current_mayor),
				ReturnCitizenEducationYears(current_mayor),
				ReturnCitizenName(current_mayor), GetCityName(city_of_mayor),
				GetCityID(city_of_mayor), fp);
		current_mayor = uniqueOrderedListGetNext(mtmElections->mayors);
		size2--;
	}
}

/**This function creates a whole MtmElections system
 Here we create the uniqueorderedlists cities and mayors
 In each step we check if the creation of a certain argument.
 In case one of the arguments is null then it returns NULL.
 If the creation of a UniqueOrderedList fails after a successfull creation
 of another UniqueOrderedList then we destroy the first one at the
 beginning and afterwards free the struct and return null*/
MtmElections mtmElectionsCreate() {
	MtmElections mtmelections = malloc(sizeof(*mtmelections));
	if (!mtmelections) {
		return NULL;
	}
	mtmelections->mayors = uniqueOrderedListCreate(
			(copyElements) & MayorCopyElements,
			(freeElements) & MayorFreeElements,
			(elementsEquals) & MayorElementsEqual,
			(elementGreaterThan) & MayorElementGreaterThan);
	if (mtmelections->mayors == NULL) {
		free(mtmelections);
	}
	mtmelections->cities = uniqueOrderedListCreate(
			(copyElements) & CityCopyElements,
			(freeElements) & CityFreeElements,
			(elementsEquals) & CityElementsEqual,
			(elementGreaterThan) & CityElementGreaterThan);
	if ((mtmelections->cities) == NULL) {
		uniqueOrderedListDestroy(mtmelections->mayors);
		free(mtmelections);
		return NULL;
	}
	return mtmelections;
}

/**This function destroys the whole elections system it goes to the
 UniqueOrderedLists and destroys both of them Through the FreeFunction
 of the different structs in the lists everything gets freed*/

void mtmElectionsDestroy(MtmElections mtmElections) {
	if (mtmElections == NULL) {
		return;
	}
	uniqueOrderedListDestroy(mtmElections->mayors);
	uniqueOrderedListDestroy(mtmElections->cities);
	free(mtmElections);
}

/**This function adds a city to a system.
 It gets the mtmelections system , a city name, and a cityid.
 It returns a message considering each and every varaiation that may occur.
 If the allocation of the city or of its name or of its UniqueOrderedList
 citizens /candidates/citycandidates
 fails then the function returns MTM_ELECTIONS_MEMORY_ERROR.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the cityid already exists then it returns MTM_ELECTIONS_CITY_ALREADY_EXISTS
 If the cityid is illegal then t returns MTM_ELECTIONS_ILLEGAL_ID*/
MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections,
		const char* cityName, int cityId) {
	if ((mtmElections == NULL) || (cityName == NULL) || (&cityId == NULL)) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (cityId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	City city = malloc(sizeof(*city));
	if (city == NULL) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	SetCityName(city, cityName);
	if (CityNameMallocFails(city) == true) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	SetCityID(city, cityId);
	if (CityContained(mtmElections->cities, city) == true) {
		return MTM_ELECTIONS_CITY_ALREADY_EXISTS;
	}
	if (CityCandidatesFailed(city) == true) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	if (CandidatesFailed(city) == true) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	if (CitizensFailed(city) == true) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	if (CityInsertFailed(mtmElections->cities, city)) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	CityFreeElements(city);
	return MTM_ELECTIONS_SUCCESS;
}

/**This function adds a citizen to a certain city according to its id.
 It gets the mtmelections system, a name,an age,a citizenid, years of education
 and a cityid.
 It returns a message considering each and every varaiation that may occur.
 If the allocation of the citizen or of his name or of his UniqueOrderedList
 citizen_candidates
 fails then the function returns MTM_ELECTIONS_MEMORY_ERROR.
 If the function got a null argument then it returns
 MTM_ELECTIONS_NULL_ARGUMENT
 If the citizenid already exists then it returns
 MTM_ELECTIONS_CITY_ALREADY_EXISTS
 If the citizen- or the cityid is illegal then t returns
 MTM_ELECTIONS_ILLEGAL_ID
 If the age is negative then it returns
 MTM_ELECTIONS_ILLEGAL_AGE
 If the cityid does not exist in the system the it returns
 MTM_ELECTIONS_CITY_DOES_NOT_EXIST*/

MtmElectionsResult mtmElectionsAddCitizen(MtmElections mtmElections,
		const char* citizenName, int citizenId, int citizenAge,
		int yearsOfEducation, int cityId) {
	if ((mtmElections == NULL) || (citizenName == NULL) || (&citizenId == NULL)
			|| (&citizenAge == NULL) || (&yearsOfEducation == NULL)
			|| (&cityId == NULL)) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0 || cityId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	if (citizenAge <= 0) {
		return MTM_ELECTIONS_ILLEGAL_AGE;
	}
	if (yearsOfEducation < 0) {
		return MTM_ELECTIONS_ILLEGAL_NUMBER_OF_YEARS;
	}
	if (CitizenExistInSystem(citizenId, mtmElections->cities) != NULL) {
		return MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS;
	}
	City current_city = GetCityThroughID(cityId, mtmElections->cities);
	if (current_city == NULL) {
		return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
	}
	if (GetCitizen(citizenId, GetCitizensInCity(current_city)) != NULL) {
		return MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS;
	}
	Citizen citizen_to_add = malloc(sizeof(*citizen_to_add));
	if (citizen_to_add == NULL) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	if (CitizenCandidatesFailed(citizen_to_add)) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	SetCitizenName(citizen_to_add, citizenName);
	if (CitizenNameMallocFails(citizen_to_add)) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	SetCitizenAge(citizen_to_add, citizenAge);
	SetCitizenID(citizen_to_add, citizenId);
	SetCitizenEduactionYears(citizen_to_add, yearsOfEducation);
	if (CitizenInsertFailed(GetCitizensInCity(current_city), citizen_to_add)) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	CitizenFreeElements(citizen_to_add);
	return MTM_ELECTIONS_SUCCESS;

}

/**This function returns the name of the citizen.
 It gets the mtmelections system, a citizen id and a char**
 and allocates memory for the char** within itself.
 It returns a message considering each and every varaiation that may occur.
 If the allocation of the name or if the strcpy fail then the function
 returns MTM_ELECTIONS_MEMORY_ERROR.
 If the function got a null argument then it returns
 MTM_ELECTIONS_NULL_ARGUMENT
 If the citizenid is illegal then t returns
 MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If it succeeds then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult MtmElectionsCitizenGetName(MtmElections mtmElections,
		int citizenId, char** name) {
	if ((mtmElections == NULL) || (&citizenId == NULL) || (name == NULL)) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	Citizen my_citizen = CitizenExistInSystem(citizenId, mtmElections->cities);
	if (my_citizen == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	*name = malloc(sizeof(char) * (strlen(ReturnCitizenName(my_citizen)) + 1));
	if (name == NULL) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	strcpy(*name, ReturnCitizenName(my_citizen));
	if (name == NULL) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	return MTM_ELECTIONS_SUCCESS;

}

/**This function returns the age of the citizen.
 It gets the mtmelections system, a citizen id and a int* age .
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the citizenid is illegal then t returns MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If it succeeds then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult MtmElectionsCitizenGetAge(MtmElections mtmElections,
		int citizenId, int* age) {
	if ((mtmElections == NULL) || (&citizenId == NULL) || (age == NULL)) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	Citizen my_citizen = CitizenExistInSystem(citizenId, mtmElections->cities);
	if (my_citizen == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	*age = ReturnCitizenAge(my_citizen);
	return MTM_ELECTIONS_SUCCESS;
}

/**This function returns the years of education of the citizen.
 It gets the mtmelections system, a citizen id and a int* yearsOfEducation .
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the citizenid is illegal then t returns MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If it succeeds then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult MtmElectionsCitizenGetEducation(MtmElections mtmElections,
		int citizenId, int* yearsOfEducation) {
	if ((mtmElections == NULL) || (&citizenId == NULL)
			|| (yearsOfEducation == NULL)) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	Citizen my_citizen = CitizenExistInSystem(citizenId, mtmElections->cities);
	if (my_citizen == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	*yearsOfEducation = ReturnCitizenEducationYears(my_citizen);
	return MTM_ELECTIONS_SUCCESS;

}

/**This function returns city id of education of the citizen.
 It gets the mtmelections system, a citizen id and a int* cityid .
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the citizenid is illegal then t returns MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If it succeeds then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult MtmElectionsCitizenGetCity(MtmElections mtmElections,
		int citizenId, int* cityId) {
	if ((mtmElections == NULL) || (&citizenId == NULL) || (cityId == NULL)) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	City current = GetCityWhereCitizenExists(citizenId, mtmElections->cities);
	if (current == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	*cityId = GetCityID(current);
	return MTM_ELECTIONS_SUCCESS;
}

/**This function adds a candidate to a certain city.
 It gets the mtmelections system, a candidateid and a  cityid .
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the candidateid or cityid are illegal then t returns
 MTM_ELECTIONS_ILLEGAL_ID
 If the age is <21 it returns MTM_ELECTIONS_AGE_NOT_APPROPRIATE
 If the cityid does not exist in the system the it returns
 MTM_ELECTIONS_CITY_DOES_NOT_EXIST
 If the candidateid does not exist in the system the it returns
 MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST
 If the insertion of the candidate in the UniqueOrderedList candidates
 fails then it returns MTM_ELECTIONS_MEMORY_ERROR
 If the candidate gets added successfully then it stops supporting everyone
 else and starts supporting himself
 If it succeeds then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult mtmElectionsAddCandidate(MtmElections mtmElections,
		int candidateId, int cityId) {
	if (mtmElections == NULL || &cityId == NULL || &candidateId == NULL) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (candidateId < 0 || cityId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	City current = GetCityThroughID(cityId, mtmElections->cities);
	if (current == NULL) {
		return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
	}
	Citizen candidate = GetCitizen(candidateId, GetCitizensInCity(current));
	if (current == NULL || GetCityID(current) != cityId || candidate == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	if (ReturnCitizenAge(candidate) < CANDIDATE_AGE) {
		return MTM_ELECTIONS_AGE_NOT_APPROPRIATE;
	}
	if (GetCandidate(candidateId, GetCandidates(current)) != NULL) {
		return MTM_ELECTIONS_CANDIDATE_ALREADY_EXISTS;
	}
	CitizenCandidate citizen_candidate = uniqueOrderedListGetLowest(
			ReturnCitizenCandidates(candidate));
	int size = uniqueOrderedListSize(ReturnCitizenCandidates(candidate));
	Citizen copy = CitizenCopyElements(candidate);
	while (size > 0) {
		mtmElectionsCancelSupport(mtmElections, candidateId,
				CitizenCandidateGetID(citizen_candidate));
		size--;
		citizen_candidate = uniqueOrderedListGetNext(
				ReturnCitizenCandidates(candidate));
	}
	if (uniqueOrderedListInsert(GetCandidates(current), candidate)
			!= UNIQUE_ORDERED_LIST_SUCCESS) {
		uniqueOrderedListRemove(GetCitizensInCity(current), candidate);
		mtmElectionsAddCitizen(mtmElections, ReturnCitizenName(copy),
				candidateId, ReturnCitizenAge(copy),
				ReturnCitizenEducationYears(copy), cityId);
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	CitizenFreeElements(copy);
	mtmElectionsSupportCandidate(mtmElections, candidateId, candidateId, 0);
	return MTM_ELECTIONS_SUCCESS;
}

/**This function withdraws a candidate from a certain city.
 It gets the mtmelections system, a candidateid and a  cityid .
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the candidateid or cityid are illegal then t returns
 MTM_ELECTIONS_ILLEGAL_ID
 If the cityid does not exist in the system the it returns
 MTM_ELECTIONS_CITY_DOES_NOT_EXIST
 If the candidateid does not exist in the system the it returns
 MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST
 If the removal of the candidate from the UniqueOrderedList candidates
 fails then it returns MTM_ELECTIONS_MEMORY_ERROR If the candidate
 gets removed successfully then it stops supporting himself
 If it succeeds then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult mtmElectionsWithdrawCandidate(MtmElections mtmElections,
		int candidateId, int cityId) {
	if (mtmElections == NULL || &cityId == NULL || &candidateId == NULL) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (candidateId < 0 || cityId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	City current = GetCityThroughID(cityId, mtmElections->cities);
	if (current == NULL || GetCityID(current) != cityId) {
		return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
	}
	Citizen candidate = CandidateExistsInSystem(candidateId,
			GetCandidates(current));
	if (candidate == NULL) {
		return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
	}
	CityCandidate city_candidate = GetCityCandidate(candidateId,
			GetCityCandidates(current));
	int size = uniqueOrderedListSize(GetCitizensInCity(current));
	Citizen current_citizen = uniqueOrderedListGetLowest(
			GetCitizensInCity(current));
	while (size > 0) {
		mtmElectionsCancelSupport(mtmElections,
				ReturnCitizenId(current_citizen), candidateId);
		if (ReturnCitizenId(current_citizen) == candidateId) {
			uniqueOrderedListClear(ReturnCitizenCandidates(current_citizen));
		}
		current_citizen = uniqueOrderedListGetNext(GetCitizensInCity(current));
		size--;
	}
	if (city_candidate != NULL) {
		if (uniqueOrderedListRemove(GetCityCandidates(current), city_candidate)
				!= UNIQUE_ORDERED_LIST_SUCCESS) {
			return MTM_ELECTIONS_MEMORY_ERROR;
		}
	}

	if (uniqueOrderedListRemove(GetCandidates(current), candidate)
			!= UNIQUE_ORDERED_LIST_SUCCESS) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	return MTM_ELECTIONS_SUCCESS;
}

/**This function makes a certain citizen support a certain candidate.
 It gets the mtmelections system, a candidateid and a citizenid and a priority.
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the candidateid or citizenid are illegal then t returns
 MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If the candidateid does not exist in the system the it returns
 MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST
 If they come from different cities the it returns
 MTM_ELECTIONS_NOT_SAME_CITY
 If the citizen already supports the candidate it returns
 MTM_ELECTIONS_ALREADY_SUPPORTED
 If the citizen is another candidate it returns
 MTM_ELECTIONS_CAN_NOT_SUPPORT
 If the citizen has the same priority for another candidate then it returns
 MTM_ELECTIONS_PRIORITY_EXISTS
 If the insertion of the candidate to the UniqueOrderedList citizen_candidates
 fails then it returns MTM_ELECTIONS_MEMORY_ERROR
 If the candidate gets added successfully then it returns
 MTM_ELECTIONS_SUCCESS */

MtmElectionsResult mtmElectionsSupportCandidate(MtmElections mtmElections,
		int citizenId, int candidateId, int priority) {
	if (mtmElections == NULL || &citizenId == NULL || &candidateId == NULL
			|| &priority == NULL) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0 || candidateId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	if (priority < 0) {
		return MTM_ELECTIONS_ILLEGAL_PRIORITY;
	}
	Citizen temporary = CitizenExistInSystem(citizenId, mtmElections->cities);
	if (temporary == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	City current1 = GetCityWhereCitizenExists(citizenId, mtmElections->cities);
	City current2 = GetCityWhereCitizenExists(candidateId,
			mtmElections->cities);
	if (current2 == NULL) {
		return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
	}
	Citizen candidate = CandidateExistsInSystem(candidateId,
			GetCandidates(current2));
	if (candidate == NULL) {
		return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
	}
	if (GetCityID(current1) != GetCityID(current2)) {
		return MTM_ELECTIONS_NOT_SAME_CITY;
	}
	if (CandidateSupported(candidateId, ReturnCitizenCandidates(temporary))) {
		return MTM_ELECTIONS_ALREADY_SUPPORTED;
	}
	if ((uniqueOrderedListContains(GetCandidates(current1), temporary))
			&& (candidateId != citizenId)) {
		return MTM_ELECTIONS_CAN_NOT_SUPPORT;
	}
	if (PriorityExists(priority, ReturnCitizenCandidates(temporary)) == true) {
		return MTM_ELECTIONS_PRIORITY_EXISTS;
	}
	CitizenCandidate to_insert = malloc(sizeof(*to_insert));
	if (to_insert == NULL) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	SetPriority(to_insert, priority);
	SetCitizenCandidateId(to_insert, candidateId);
	uniqueOrderedListInsert(ReturnCitizenCandidates(temporary), to_insert);
	free(to_insert);
	return MTM_ELECTIONS_SUCCESS;
}

/**This function makes a certain citizen cancel his support for a certain
 candidate.It gets the mtmelections system, a candidateid and a citizenid.
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns
 MTM_ELECTIONS_NULL_ARGUMENT
 If the candidateid or citizenid are illegal then t returns
 MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If the candidateid does not exist in the system the it returns
 MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST
 If the citizen is the candidate the it returns MTM_ELECTIONS_MUST_SUPPORT
 If the citizen does not support the candidate it returns
 MTM_ELECTIONS_NOT_SUPPORTED
 If the removal of the candidate from the UniqueOrderedList
 citizen_candidates fails then it returns MTM_ELECTIONS_MEMORY_ERROR
 If the candidate gets removed successfully then it returns
 MTM_ELECTIONS_SUCCESS */

MtmElectionsResult mtmElectionsCancelSupport(MtmElections mtmElections,
		int citizenId, int candidateId) {
	if (mtmElections == NULL || &citizenId == NULL || &candidateId == NULL) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0 || candidateId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	Citizen temporary = CitizenExistInSystem(citizenId, mtmElections->cities);
	if (temporary == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	City current = GetCityWhereCitizenExists(citizenId, mtmElections->cities);
	if (current == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	if (CandidateExistsInSystem(candidateId, GetCandidates(current)) == NULL) {
		return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
	}
	if (CandidateExistsInSystem(citizenId, GetCandidates(current)) != NULL
			&& candidateId == citizenId) {
		return MTM_ELECTIONS_MUST_SUPPORT;
	}
	if (CandidateSupported(candidateId,
			ReturnCitizenCandidates(temporary))==false) {
		return MTM_ELECTIONS_NOT_SUPPORTED;
	}
	CitizenCandidate citizen_candidate = GetCitizenCandidate(candidateId,
			ReturnCitizenCandidates(temporary));
	if (citizen_candidate == NULL) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	if (uniqueOrderedListRemove(ReturnCitizenCandidates(temporary),
			citizen_candidate) != UNIQUE_ORDERED_LIST_SUCCESS) {
		return MTM_ELECTIONS_MEMORY_ERROR;
	}
	return MTM_ELECTIONS_SUCCESS;

}

/**This function makes a certain citizen change his city.
 It gets the mtmelections system,a citizenid and a cityId .
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the  cityid or citizenid are illegal then t returns MTM_ELECTIONS_ILLEGAL_ID
 If the citizenid does not exist in the system the it returns
 MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST
 If the cityid does not exist in the system the it returns
 MTM_ELECTIONS_CITY_DOES_NOT_EXIST
 If the removal of the candidate from the UniqueOrderedList citizen
 (of the old city) fails then it returns MTM_ELECTIONS_MEMORY_ERROR
 If the insertion of the candidate from the UniqueOrderedList citizen
 (of the new city) fails then it  returns MTM_ELECTIONS_MEMORY_ERROR
 If the change was done successfully then it returns MTM_ELECTIONS_SUCCESS */

MtmElectionsResult mtmElectionsChangeAddress(MtmElections mtmElections,
		int citizenId, int cityId) {
	if (mtmElections == NULL || &citizenId == NULL || &cityId == NULL) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (citizenId < 0 || cityId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	Citizen temporary = CitizenExistInSystem(citizenId, mtmElections->cities);
	if (temporary == NULL) {
		return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
	}
	if (GetCityThroughID(cityId, mtmElections->cities) == NULL) {
		return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
	}
	City current = GetCityWhereCitizenExists(citizenId, mtmElections->cities);
	if (GetCandidate(citizenId, GetCandidates(current)) != NULL) {
		mtmElectionsWithdrawCandidate(mtmElections, citizenId,
				GetCityID(current));
	}
	Citizen copy = CitizenCopyElements(temporary);
	if (uniqueOrderedListRemove(GetCitizensInCity(current), temporary)
			!= UNIQUE_ORDERED_LIST_SUCCESS) {
		mtmElectionsAddCitizen(mtmElections, ReturnCitizenName(copy), citizenId,
				ReturnCitizenAge(copy), ReturnCitizenEducationYears(copy),
				GetCityID(current));
		return MTM_ELECTIONS_MEMORY_ERROR;
	}

	uniqueOrderedListDestroy(ReturnCitizenCandidates(copy));
	mtmElectionsAddCitizen(mtmElections, ReturnCitizenName(copy), citizenId,
			ReturnCitizenAge(copy), ReturnCitizenEducationYears(copy), cityId);
	free(ReturnCitizenName(copy));
	free(copy);
	return MTM_ELECTIONS_SUCCESS;
}

/**This function finds the mayor in a certain city.
 It gets the mtmelections system,a cityid and a pointer to which we are going to
 insert the mayorid and
 a filename. It uses the function WinnerInCity
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns MTM_ELECTIONS_NULL_ARGUMENT
 If the  cityid is illegal then t returns MTM_ELECTIONS_ILLEGAL_ID
 If the cityid does not exist in the system the it returns
 MTM_ELECTIONS_CITY_DOES_NOT_EXIST
 If there are no candidates in the city it returns
 MTM_ELECTIONS_NO_CANDIDATES_IN_CITY
 This function uses the function WinnerInCity
 If the file does open it prints the details of the mayor to the file and
 returns MTM_ELECTIONS_SUCCESS
 If it does not open or close successfully it returns MTM_ELECTIONS_FILE_ERROR
 */

MtmElectionsResult mtmElectionsMayorOfCity(MtmElections mtmElections,
		int cityId, int* mayor, const char* filename) {
	if (mtmElections == NULL || mayor == NULL || filename == NULL) {
		return MTM_ELECTIONS_NULL_ARGUMENT;
	}
	if (cityId < 0) {
		return MTM_ELECTIONS_ILLEGAL_ID;
	}
	City current = GetCityThroughID(cityId, mtmElections->cities);
	if (current == NULL) {
		return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
	}
	int size = uniqueOrderedListSize(GetCandidates(current));
	if (size == 0) {
		return MTM_ELECTIONS_NO_CANDIDATES_IN_CITY;
	}

	Citizen winner_info = WinnerInCity(mtmElections, cityId, mayor);
	FILE *fp = fopen(filename, "a");
	if (fp == NULL) {
		return MTM_ELECTIONS_FILE_ERROR;
	}
	mtmPrintMayorDetails(ReturnCitizenId(winner_info),
			ReturnCitizenAge(winner_info),
			ReturnCitizenEducationYears(winner_info),
			ReturnCitizenName(winner_info), GetCityName(current),
			GetCityID(current), fp);
	if (fclose(fp) == EOF) {
		return MTM_ELECTIONS_FILE_ERROR;
	}
	return MTM_ELECTIONS_SUCCESS;
}

/**This function ranks a citizen by his age.
 It gets a citizenid, the mtmelections system and a pAge
 and returns the ratio between the citizen's age and the pAge
 If one parameter has an invalid value then it returns 0*/
int mtmElectionsRankByAge(MtmElections mtmElections, int citizen, void* pAge) {
	if (mtmElections == NULL || pAge == NULL) {
		return 0;
	}
	int citizen_pAge = *(int*) pAge;
	if (citizen_pAge == 0) {
		return 0;
	}
	int citizen_age;
	if (MtmElectionsCitizenGetAge(mtmElections, citizen, &citizen_age)
			!= MTM_ELECTIONS_SUCCESS) {
		return 0;
	}
	return (citizen_age) / (citizen_pAge);

}

/**This returns all of the mayors in all of the cities in a UniqueOrderdList
 that contains the ids of the mayors.
 It gets the mtmelections system,a RankFunc, void* auxilaryData and
 a const char* filename.
 It returns a message considering each and every varaiation that may occur.
 If the function got a null argument then it returns NULL
 If the file does not open then it prints an error message in stderr
 This function uses the functions :
 1-FileIsNullAndRankFuncIsNull
 2-FileIsNullWithRankFunc
 3-FileOpensAndRankFuncIsNull
 4- FileOpensWithRankFunc
 which are explained above
 If the file does open it prints the details of the mayors to the file
 */

UniqueOrderedList mtmElectionsPerformElections(MtmElections mtmElections,
		RankFunc rank, void* auxilaryData, const char* filename) {
	if (mtmElections == NULL) {
		return NULL;
	}
	if (mtmElections->mayors == NULL) {
		return NULL;
	}
	City current = uniqueOrderedListGetLowest(mtmElections->cities);
	int size = uniqueOrderedListSize(mtmElections->cities);
	if (current == NULL) {
		return NULL;
	}
	uniqueOrderedListClear(mtmElections->mayors);
	UniqueOrderedList mayors_ids = uniqueOrderedListCreate(
			(copyElements) & MayorIdCopyElements,
			(freeElements) & MayorIdFreeElements,
			(elementsEquals) & MayorIdElementsEqual,
			(elementGreaterThan) & MayorIdElementsGreaterThan);
	FILE* fp;
	fp = fopen(filename, "a");
	if (fp == NULL) {
		mtmPrintFileError(filename, stderr);
		if (rank == NULL) {
			FileIsNullAndRankFuncIsNull(mtmElections, size, current);
		} else {
			FileIsNullWithRankFunc(mtmElections, size, current, rank,
					auxilaryData);
		}

		ReturnMayorsOfCities(mtmElections, mayors_ids);
		return mayors_ids;
	}
	if (rank == NULL) {
		FileOpensAndRankFuncIsNull(mtmElections, size, current, fp);
		ReturnMayorsOfCities(mtmElections, mayors_ids);
		fclose(fp);
		return mayors_ids;
	}

	FileOpensWithRankFunc(mtmElections, size, current, rank, auxilaryData, fp);
	ReturnMayorsOfCities(mtmElections, mayors_ids);
	fclose(fp);
	return mayors_ids;
}
