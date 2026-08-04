// This implementation of the following specification: https://unnamedbruh.github.io/basic-specs-exercise/basic_math_functions.html
// Is written in the C++ programming language. Specifically, C++26.

// Copyright (C) UnnamedBruh 2026

// The software is provided "AS IS", WITHOUT WARRANTY OF ANY KIND.
// The authors or copyright holders are NOT LIABLE for any damages.

// Please read the MIT License for more details: https://opensource.org/license/mit

// ----- DISCLAIMER ----- //

// This file is the official C++ reference implementation of the Basic Mathematical Functions specification. As of August 1, 2026, it targets the current C++ language specification and prioritizes conformance over portability to older C++ runtime libraries.

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

// C-style pointers, and many array-like C++ containers use zero-indexed systems. They are compatible with the lists specified in the reference document.

// Although integers would truncate or overflow, this is technically allowed by the specification. It only specifies that the types must be "numeric."

#include <unordered_map>
#include <vector>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <utility>
#include <algorithm>

template <typename T>
void validate(const std::vector<T>& list) {
	if constexpr (std::is_floating_point_v<T>) {
		for (const T& item : list) {
			// Note: std::isfinite checks if a floating point type is finite, and represents a specific number.
			if (!std::isfinite(item)) throw std::invalid_argument("The list contains numerical types that do not represent a specific value");
		}
	}

	static_assert(std::is_arithmetic_v<T>, "The types contained by the list must be numerical");
}

template <typename T>
void throwIfListIsEmpty(const std::vector<T>& list) {
	if (list.empty()) throw std::invalid_argument("The list is empty; it contains no items");
}

template <typename T>
T mean(const std::vector<T>& list) {
	validate(list);
	throwIfListIsEmpty(list);

	T accumulator = T(0);
	for (const T& item : list) accumulator += item;
	return accumulator / T(list.size());
}

template <typename T>
T median(std::vector<T> list) { // Do not pass by reference to keep the original list unmodified
	validate(list);
	throwIfListIsEmpty(list);

	std::sort(list.begin(), list.end());
	// Note: " ... there is a negligible performance overhead for when it attempts to sort an already sorted list. However, it produces the correct result regardless of whether x is or is not sorted."

	if (list.size() % 2 == 0) // If the list's length is even
		return (list[list.size() / 2 - 1] + list[list.size() / 2]) / T(2);
	else // If the list's length is odd
		return list[(list.size() - 1) / 2];
}

template <typename T>
std::vector<T> mode(const std::vector<T>& list) {
	validate(list);
	throwIfListIsEmpty(list);

	std::unordered_map<T, size_t> counts;
	std::vector<T> keysOrder;
	size_t highestCount = 0;

	for (const T& item : list) {
		auto foundPair = counts.find(item);
		size_t count = 0;
		if (foundPair == counts.end()) {
			count = counts[item] = 1;
			keysOrder.push_back(item);
		} else count = ++foundPair->second;
		if (highestCount < count) highestCount = count;
	}

	if (highestCount == 1) return {};

	std::vector<T> commonValues;

	for (const T& key : keysOrder) {
		if (counts[key] == highestCount) commonValues.push_back(key);
	}

	return commonValues;
}

template <typename T>
T range(const std::vector<T>& list) {
	validate(list);
	throwIfListIsEmpty(list);

	T minimumValue = list[0], maximumValue = list[0];

	for (const T& item : list) {
		if (item < minimumValue) minimumValue = item;
		if (item > maximumValue) maximumValue = item;
	}

	return maximumValue - minimumValue;
}
