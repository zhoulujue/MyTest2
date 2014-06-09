// AmazonTestSection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#define IN
#define OUT
struct Interval
{
	int upper_bound;
	int lower_bound;
	void operator=(Interval& interval)
	{
		upper_bound = interval.upper_bound;
		lower_bound = interval.lower_bound;
	}
};
/**
*	Check the given interval to see whether it is valid.
*	@param set: The interval to be checked.
*	@return : true if valid, false if invalid.
*/
bool IsValid(IN Interval& interval)
{
	return interval.lower_bound < interval.upper_bound;
}
/**
*	Check whether the given two intervals overlap.
*	Notice that two intervals overlap when bounds meet. e.g. [6,7] overlaps with [7,8]
*	@param intervalA : The interval to be checked.
*	@param intervalB : The interval to be checked.
*	@return : true if overlap, false not.
*/
bool IsOverlap(IN Interval& intervalA, IN Interval& intervalB)
{
	return  (intervalA.lower_bound > intervalB.lower_bound ? intervalA.lower_bound : intervalB.lower_bound) 
		<= (intervalA.upper_bound < intervalB.upper_bound ? intervalA.upper_bound : intervalB.upper_bound);
}
/**
*	Compare two intervals.
*	Notice that you SHOULD NOT compare two overlapping intervals.
*	e.g. [6,7] < [13,27] , [6,7] == [6,7] , [23,37] > [7,9]
*	@param intervalA : The interval to be compared.
*	@param intervalA : The interval to be compared.
*	@return : -1 if intervalA < intervalB; 1 if intervalA > intervalB; 0 when equal.
*/
int Compare(const void * intervalA, const void * intervalB)
{
	if ((*(Interval*)intervalA).upper_bound < (*(Interval*)intervalB).lower_bound)
		return -1;
	else if ((*(Interval*)intervalB).upper_bound < (*(Interval*)intervalA).lower_bound)
		return 1;
	else 
		return 0;
}
/**
*	Sort all the intervals in the given array, and remove the duplicated intervals.
*	Notice that all intervals SHOULD NOT be overlap with each other.
*	@param array : The interval array to be sorted.
*	@param size : The size of the array.
*/
void sortIntervalArray(IN OUT int& size, IN OUT Interval intervalArray[])
{
	Interval* temp = new Interval[size];
	qsort(intervalArray, size, sizeof(Interval), Compare);
	temp[0] = intervalArray[0];
	int sortSize = 1;
	for (int index = 1; index < size; index++)
		if (Compare(&intervalArray[index], &intervalArray[index-1]) != 0)
			temp[sortSize++] = intervalArray[index];
	size = sortSize;
	memcpy(intervalArray, temp, sizeof(Interval) * size);
}
/**
*	Solve the intersection of the given two intervals, then fill upper and lower bounds in the out parameter.
*	@param intervalA : The given interval to be intersected.
*	@param intervalB : The given interval to be intersected.
*	@param outIntersection : Result of the intersection. It is a set itself.
*	@return : true if existing a intersection, false not.
*/
bool Intersection(IN Interval& intervalA, IN Interval& intervalB, OUT Interval& outIntersection)
{
	outIntersection.lower_bound = intervalA.lower_bound > intervalB.lower_bound ? intervalA.lower_bound : intervalB.lower_bound;
	outIntersection.upper_bound = intervalA.upper_bound < intervalB.upper_bound ? intervalA.upper_bound : intervalB.upper_bound;
	if (IsValid(outIntersection))
		return true;
	else
		return false;
}
/**
*	Solve the union of the given two intervals.
*	Notice that if the given intervals overlap, then 
*	this function return true indicating that the outUnion is the only union interval.
*   On the other hand if they don't overlap, return false and the union of the given 
*	two intervals is a simply collection of both.
*	@param intervalA : The given interval to be aggregated.
*	@param intervalB : The given interval to be aggregated.
*	@param outUnion : Result of the union. It is a set itself.
*	@return : true if outUnion is the only union interval, false not. 
*/
bool Union(IN Interval& intervalA, IN Interval& intervalB, OUT Interval& outUnion)
{
	if (IsOverlap(intervalA, intervalB))
	{
		outUnion.lower_bound = intervalA.lower_bound < intervalB.lower_bound ? intervalA.lower_bound : intervalB.lower_bound;
		outUnion.upper_bound = intervalA.upper_bound > intervalB.upper_bound ? intervalA.upper_bound : intervalB.upper_bound;
		return true;
	}
	else
		return false;
}

/**
*	Handle the chars that read from stdin. Resolve them into objects.
*	@param input : The chars read from stdin.
*	@param intervals : The array containing objects resolved.
*	@param size : Counts of the objects.
*	@return : success if true, otherwise not.
*/
bool splitIntervals(IN char* input, OUT Interval intervals[], OUT int& size)
{
	if (*input == '\0' || input == NULL)
		return false;
	size = 0;
	int upper_bound = 0, lower_bound = 0;
	char interval[512] = {0};
	int index = 0;
	while (*input != '\0')
	{
		if (*input == '[')
		{
			interval[index] = *input;
			index++;
		}
		else if (*input == ']')
		{
			interval[index] = *input;
			interval[index + 1] = '\0';
			sscanf_s(interval, "[%d,%d]", &lower_bound, &upper_bound);
			intervals[size].lower_bound = lower_bound;
			intervals[size].upper_bound = upper_bound;
			if (IsValid(intervals[size]))
				return false;
			size++;
		}
		else if (*input == ' ')
			index = 0;
		else
		{
			interval[index] = *input;
			index++;
		}
		input++;
	}
	return true;
}

/**
*	Print a interval array into stdout.
*	@param array : The array to be printed.
*	@param size : The size of given array.
*/
void printArray(const Interval array[], const int size)
{
	for (int i = 0; i < size - 1; i++ )
		printf("[%d,%d] ", array[i].lower_bound, array[i].upper_bound);
	printf("[%d,%d]\n", array[size-1].lower_bound, array[size-1].upper_bound);
}

int _tmain(int argc, _TCHAR* argv[])
{
	char input[512];
	Interval aInterval[512];
	int nIntervalSize = 0, nIntersectionSize = 0, nUnionSize = 0;
	while (gets_s(input) != NULL)
	{
		if (!splitIntervals(input, aInterval, nIntervalSize)) 
		{
			printf("invalid\n");
			return 0;
		}
		Interval* aIntersection = new Interval[nIntervalSize];
		Interval* aUnion = new Interval[nIntervalSize];
		// Intersection
		for (int i =1; i < nIntervalSize; i++)
		{
			if (Intersection(aInterval[0], aInterval[i], aIntersection[nIntersectionSize]))
				nIntersectionSize++;
		}
		// Union
		for (int i = 0; i < nIntersectionSize; i++)
		{
			for (int j = i + 1; j < nIntersectionSize; j++)
			{
				if (Union(aIntersection[i], aIntersection[j], aUnion[nUnionSize])) 
					nUnionSize ++;
				else
				{
					aUnion[nUnionSize] = aIntersection[i];
					aUnion[nUnionSize + 1] = aIntersection[i];
					nUnionSize += 2;
				}
			}
		}
		if (nUnionSize < 1) {printf("empty\n");return 0;}
		// Sort
		sortIntervalArray(nUnionSize, aUnion);
		printArray(aUnion, nUnionSize);
	}
	return 0;
}

