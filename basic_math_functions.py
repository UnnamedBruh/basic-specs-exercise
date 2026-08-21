# This implementation of the following specification: https://unnamedbruh.github.io/basic-specs-exercise/basic_math_functions.html
# Is written in the Python programming language: https://docs.python.org/3.14/reference/index.html

# Copyright (C) UnnamedBruh 2026

# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 

##### DISCLAIMER #####

# This file is the official Python reference implementation of the Basic Mathematical Functions specification. As of August 20, 2026, it targets the current Python language specification and prioritizes conformance over portability to older implementations of Python.

##### UP TO THE IMPLEMENTATION #####

# 3. Global Edge Cases

"""
The functions above assume that x is a list of items, and that each item is a numerical value. This chapter specifies functionality that may be included in each defined function. The functionality is as follows:

* If x is not a list of items, raise an error.
* If any item of x is not any numerical type, raise an error.
* If any item of x is a numerical type, but does not represent any specific value (ex. when an item is NaN), raise an error.

The above functionality prevents unexpected behavior. For an example, if mean was passed a string value, it can raise an error instead of returning an unexpected value.
"""

# 4. Notes For Each Pseudocode Snippet

# On an empty list, whether the functions must return 0 or an empty list, or throw an error is intentionally ambiguous. Therefore, it is up to the implementation to decide between either functionality for that case.

allowed_types = (int, float)

import math

def validate(inputlist):
	if type(inputlist) is not list:
		raise "The argument is not a valid list"

	for item in inputlist:
		if isinstance(item, allowed_types):
			raise "The list contains items that are not numerical types"

		if not math.isfinite(item):
			raise "The list contains numerical types that do not represent a specific value"

def throwIfListIsEmpty(inputlist):
	if len(list) == 0:
		raise "The list is empty; it contains no items"


def mean(inputlist):
	validate(inputlist)
	throwIfListIsEmpty(inputlist)

	return sum(inputlist) / len(inputlist)

def median(inputlist):
	validate(inputlist)
	throwIfListIsEmpty(inputlist)

	sortedlist = sorted(inputlist)

	if len(sortedlist) % 2 == 0:
		return (sortedlist[length / 2 - 1] + sortedlist[length / 2]) / 2
	else:
		return sortedlist[length / 2]

function mode(list)
	validate(list)
	throwIfListIsEmpty(list)

	local counts = {}
	local keysOrder = {}

	local highestCount = 0

	for _, item in ipairs(list) do
		if counts[item] then
			counts[item] = counts[item] + 1
		else
			counts[item] = (counts[item] or 0) + 1
			keysOrder[#keysOrder + 1] = item
		end

		if counts[item] > highestCount then
			highestCount = counts[item]
		end
	end

	-- No repeated values
	if highestCount == 1 then
		return {}
	end

	local commonValues = {}

	for i = 1, #keysOrder do
		local key = keysOrder[i]
		if counts[key] == highestCount then
			table.insert(commonValues, key)
		end
	end

	return commonValues
end

function range(list)
	validate(list)
	throwIfListIsEmpty(list)

	local minimumValue = list[1]
	local maximumValue = list[1]

	for _, item in ipairs(list) do
		if item < minimumValue then
			minimumValue = item
		end

		if item > maximumValue then
			maximumValue = item
		end
	end

	return maximumValue - minimumValue
end
