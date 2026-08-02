// This implementation of the following specification: https://unnamedbruh.github.io/basic-specs-exercise/basic_math_functions.html
// Is written in the C programming language. Specifically, C23.

// Copyright (C) UnnamedBruh 2026

// The software is provided "AS IS", WITHOUT WARRANTY OF ANY KIND.
// The authors or copyright holders are NOT LIABLE for any damages.

// Please read the MIT License for more details: https://opensource.org/license/mit

// ----- DISCLAIMER ----- //

// This file is the official C reference implementation of the Basic Mathematical Functions specification. As of August 1, 2026, it targets the current C language specification and prioritizes conformance over portability to older versions of C.

// ----- UP TO THE IMPLEMENTATION ----- //

/* 3. Global Edge Cases

The functions above assume that x is a list of items, and that each item is a numerical value. This chapter specifies functionality that may be included in each defined function. The functionality is as follows:

* If x is not a list of items, raise an error.
* If any item of x is not any numerical type, raise an error.
* If any item of x is a numerical type, but does not represent any specific value (ex. when an item is NaN), raise an error.

The above functionality prevents unexpected behavior. For an example, if mean was passed a string value, it can raise an error instead of returning an unexpected value. */

/* 4. Notes For Each Pseudocode Snippet

On an empty list, whether the functions must return 0 or an empty list, or throw an error is intentionally ambiguous. Therefore, it is up to the implementation to decide between either functionality for that case.

Importantly, how the functions are implemented is not a concern. What mainly matters is their functional correctness. For an example, an implementation of the mean function can use SIMD optimizations while retaining technical correctness. */

// ----- IMPLEMENTATION DETAILS ----- //

// C pointers use zero-indexed systems. They are compatible with the lists specified in the reference document.
// However, C lacks maps that the specification implies are supported, so we have to define our own hash-map system.
// C also lacks template systems, so we must either choose one type, or define near-identical functions for different types.

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

const int MATHFUNCTIONS_ERROR_FAILED = 1;
const int MATHFUNCTIONS_ERROR_SUCCESS = 0;

typedef double desiredType;

int validate(const desiredType* ptr, const size_t len) {
	for (size_t i = 0; i < len; i++) {
		// Note: isfinite checks if a floating point type is finite, and represents a specific number.
		if (!isfinite(ptr[i])) return MATHFUNCTIONS_ERROR_FAILED;
	}
	return MATHFUNCTIONS_ERROR_SUCCESS;
}

bool isListEmpty(const size_t len) {
	return len == 0;
}

int mean(const desiredType* list, const size_t len, desiredType* result) {
	if (list == NULL || result == NULL) return MATHFUNCTIONS_ERROR_FAILED;
	if (validate(list, len)) return MATHFUNCTIONS_ERROR_FAILED;
	if (isListEmpty(len)) return MATHFUNCTIONS_ERROR_FAILED;

	desiredType accumulator = 0;
	for (size_t i = 0; i < len; i++) {
		accumulator += list[i];
	}
	*result = accumulator / (desiredType)len;
	return MATHFUNCTIONS_ERROR_SUCCESS;
}

int m__compare_desiredType(const void *a, const void *b) {
	desiredType arg1 = *(const desiredType *)a;
	desiredType arg2 = *(const desiredType *)b;

	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;
	return 0;
}

int median(const desiredType* list, const size_t len, desiredType* result) {
	if (list == NULL || result == NULL) return MATHFUNCTIONS_ERROR_FAILED;
	if (validate(list, len)) return MATHFUNCTIONS_ERROR_FAILED;
	if (isListEmpty(len)) return MATHFUNCTIONS_ERROR_FAILED;

	desiredType *allocatedMemory = (desiredType *)malloc(len * sizeof(desiredType));
	if (allocatedMemory == NULL) return MATHFUNCTIONS_ERROR_FAILED;

	memcpy(allocatedMemory, list, len * sizeof(desiredType));

	qsort(allocatedMemory, len, sizeof(desiredType), m__compare_desiredType);
	// Note: " ... there is a negligible performance overhead for when it attempts to sort an already sorted list. However, it produces the correct result regardless of whether x is or is not sorted."

	if (len % 2 == 0) // If the list's length is even
		*result = (allocatedMemory[len / 2 - 1] + allocatedMemory[len / 2]) / (desiredType)2;
	else // If the list's length is odd
		*result = allocatedMemory[(len - 1) / 2];

	return MATHFUNCTIONS_ERROR_SUCCESS;
}

typedef int (*m__compare_function)(const void *a, const void *b);

int m__cmp_desiredType(const void *a, const void *b) {
	desiredType da = *(const desiredType *)a;
	desiredType db = *(const desiredType *)b;
	return da == db ? 0 : 1; // handles -0.0 == 0.0 and NaN != NaN correctly
}

size_t m__map_find(void *hash_table, const size_t len, const size_t elementSize, const void *candidate, m__compare_function cmp) {
	const unsigned char *base = (const unsigned char *)hash_table;
	for (size_t i = 0; i < len; i++) {
		const void *element = base + i * elementSize;
		if (cmp(element, candidate) == 0) return i;
	}
	return len;
}

int mode(const desiredType* list, const size_t len, desiredType** result, size_t* lenOfResult) {
	if (list == NULL || result == NULL) return MATHFUNCTIONS_ERROR_FAILED;
	if (validate(list, len)) return MATHFUNCTIONS_ERROR_FAILED;
	if (isListEmpty(len)) return MATHFUNCTIONS_ERROR_FAILED;

	const size_t mapLength = len;

	// calloc automatically initializes the memory to 0, unlike malloc
	desiredType *countsKeys = (desiredType *)calloc(mapLength, sizeof(desiredType));
	if (countsKeys == NULL) return MATHFUNCTIONS_ERROR_FAILED;
	size_t *countsValues = (size_t *)calloc(mapLength, sizeof(size_t));
	if (countsValues == NULL) {
		free(countsKeys);
		return MATHFUNCTIONS_ERROR_FAILED;
	}

	size_t highestCount = 0;
	size_t nextAvailableIndexForMap = 0;

	for (size_t i = 0; i < len; i++) {
		size_t index = m__map_find(countsKeys, nextAvailableIndexForMap + 1, sizeof(desiredType), &list[i], m__cmp_desiredType);
		size_t count = 0;
		if (index == nextAvailableIndexForMap + 1) {
			countsKeys[nextAvailableIndexForMap] = list[i];
			count = countsValues[nextAvailableIndexForMap] = 1;
			nextAvailableIndexForMap++;
		} else {
			count = ++countsValues[index];
		}
		if (highestCount < count) highestCount = count;
	}

	if (highestCount == 1) {
		*result = NULL;
		*lenOfResult = 0;
		goto FREE_MAP_SUCCESS;
	}

	desiredType *listOfItems = (desiredType *)calloc(len, sizeof(desiredType));
	if (listOfItems == NULL) goto FREE_HASH_FAIL;

	size_t nextAvailableIndexForList = 0;

	for (size_t i = 0; i < len; i++) {
		if (countsValues[i] == highestCount) {
			listOfItems[nextAvailableIndexForList] = countsKeys[i];
			nextAvailableIndexForList++;
		}
	}

	*result = listOfItems;
	*lenOfResult = nextAvailableIndexForList;

	FREE_MAP_FAIL:
	free(countsKeys);
	free(countsValues);

	return MATHFUNCTIONS_ERROR_FAILED;

	FREE_MAP_SUCCESS:
	free(countsKeys);
	free(countsValues);

	return MATHFUNCTIONS_ERROR_SUCCESS;
}

int range(const desiredType* list, const size_t len, desiredType* result) {
	if (list == NULL || result == NULL) return MATHFUNCTIONS_ERROR_FAILED;
	if (validate(list, len)) return MATHFUNCTIONS_ERROR_FAILED;
	if (isListEmpty(len)) return MATHFUNCTIONS_ERROR_FAILED;

	desiredType minimumValue = list[0], maximumValue = list[0];

	for (size_t i = 1; i < len; i++) {
		if (list[i] < minimumValue) minimumValue = list[i];
		if (list[i] > maximumValue) maximumValue = list[i];
	}

	*result = maximumValue - minimumValue;

	return MATHFUNCTIONS_ERROR_SUCCESS;
}
