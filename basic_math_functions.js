// This implementation of the following specification: https://unnamedbruh.github.io/basic-specs-exercise/basic_math_functions.html
// Is written in the ECMAScript programming language: https://tc39.es/ecma262/

// Copyright (C) UnnamedBruh 2026

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// ----- DISCLAIMER ----- //

// This file is the official ECMAScript reference implementation of the Basic Mathematical Functions specification. As of August 1, 2026, it targets the current ECMAScript language specification and prioritizes conformance over portability to older runtimes.

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

// But since SIMD optimizations are unavailable in ECMAScript, the implementation of ECMAScript may handle SIMD optimizations where possible.

// ----- IMPLEMENTATION DETAILS ----- //

// ECMAScript Arrays and TypedArrays use zero-indexed systems, and they are compatible with the lists specified in the reference document.

// In this implementation, BigUint64Array and BigInt64Array are deliberately not handled. This is not a requirement of the original document.

function validate(list) {
	let isEachItemNumerical, isEachNumberSpecific;
	let isNotList = true;
	if (Array.isArray(list)) {
		isEachItemNumerical = false;
		isEachNumberSpecific = false;
		isNotList = false;
	} else if (list instanceof Float32Array || list instanceof Float64Array || list instanceof Float16Array) {
		isEachItemNumerical = true;
		isEachNumberSpecific = false;
		isNotList = false;
	} else if (list instanceof Uint8Array || list instanceof Uint16Array || list instanceof Uint32Array || list instanceof Int8Array || list instanceof Int16Array || list instanceof Int32Array) {
		isEachItemNumerical = true;
		isEachNumberSpecific = true;
		isNotList = false;
	}
	// If x is not a list of items, raise an error.
	if (isNotList) throw new TypeError("The argument is not a valid list");
	if (isEachItemNumerical && isEachNumberSpecific) return;
	for (const item of list) {
		if (!isEachItemNumerical && typeof item !== "number") throw new TypeError("The list contains items that are not numerical types"); // If any item of x is not any numerical type, raise an error.
		if (!isEachNumberSpecific && (Number.isNaN(item) || !Number.isFinite(item))) throw new TypeError("The list contains numerical types that do not represent a specific value"); // If any item of x is a numerical type, but does not represent any specific value (ex. when an item is NaN), raise an error.
	}
}

function throwIfListIsEmpty(list) {
	if (list.length === 0) throw new TypeError("The list is empty; it contains no items");
}

function mean(list) {
	validate(list);
	throwIfListIsEmpty(list);

	let accumulator = 0;
	for (const item of list) accumulator += item;
	return accumulator / list.length;
}

function median(list) {
	validate(list);
	throwIfListIsEmpty(list);

	const sortedList = list.toSorted((x, y) => x - y); // The default sorts the items of an Array by lexicographical order.
	// This tells ECMAScript's %TypedArray%.prototype.toSorted function to sort from least to greatest,
	// although the order doesn't truly matter. What matters is that the list is sorted by value.

	// Note: " ... there is a negligible performance overhead for when it attempts to sort an already sorted list. However, it produces the correct result regardless of whether x is or is not sorted."

	if (sortedList.length % 2 === 0) // If the list's length is even
		return (sortedList[sortedList.length / 2 - 1] + sortedList[sortedList.length / 2]) / 2;
	else // If the list's length is odd
		return sortedList[(sortedList.length - 1) / 2];
}

function mode(list) {
	validate(list);
	throwIfListIsEmpty(list);

	let counts = new Map();
	let highestCount = 0;

	for (const item of list) {
		let value = counts.get(item);
		if (!value) counts.set(item, (value = 1)); else counts.set(item, ++value);
		if (highestCount < value) highestCount = value;
	}

	if (highestCount === 1) return []; // Although Arrays do not correspond to TypedArrays, they are able to be converted into TypedArrays.

	let commonValues = [];

	for (const [key, value] of counts) {
		if (value === highestCount) commonValues.push(key);
	}

	return commonValues;
}

function range(list) {
	validate(list);
	throwIfListIsEmpty(list);

	let minimumValue = list[0], maximumValue = list[0];

	for (const item of list) {
		if (item < minimumValue) minimumValue = item;
		if (item > maximumValue) maximumValue = item;
	}

	return maximumValue - minimumValue;
}
