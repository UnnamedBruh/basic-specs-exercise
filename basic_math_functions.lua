-- This implementation of the following specification: https://unnamedbruh.github.io/basic-specs-exercise/basic_math_functions.html
-- Is written in the Lua programming language: https://www.lua.org/manual/5.4/manual.html

-- Copyright (C) UnnamedBruh 2026

-- The software is provided "AS IS", WITHOUT WARRANTY OF ANY KIND.
-- The authors or copyright holders are NOT LIABLE for any damages.

-- Please read the MIT License for more details: https://opensource.org/license/mit

----- DISCLAIMER -----

-- This file is the official Lua reference implementation of the Basic Mathematical Functions specification. As of August 1, 2026, it targets the current Lua language specification and prioritizes conformance over portability to older implementations of Lua.

----- UP TO THE IMPLEMENTATION -----

-- 3. Global Edge Cases

--[[ The functions above assume that x is a list of items, and that each item is a numerical value. This chapter specifies functionality that may be included in each defined function. The functionality is as follows:

* If x is not a list of items, raise an error.
* If any item of x is not any numerical type, raise an error.
* If any item of x is a numerical type, but does not represent any specific value (ex. when an item is NaN), raise an error.

The above functionality prevents unexpected behavior. For an example, if mean was passed a string value, it can raise an error instead of returning an unexpected value. ]]

--[[ 4. Notes For Each Pseudocode Snippet

On an empty list, whether the functions must return 0 or an empty list, or throw an error is intentionally ambiguous. Therefore, it is up to the implementation to decide between either functionality for that case.

Importantly, how the functions are implemented is not a concern. What mainly matters is their functional correctness. For an example, an implementation of the mean function can use SIMD optimizations while retaining technical correctness. ]]

-- But since SIMD optimizations are unavailable in Lua, the implementation of Lua may handle SIMD optimizations where possible.

----- IMPLEMENTATION DETAILS -----

-- Since tables in Lua are one-indexed, the median implementation must adapt to this system. The specification only specified the pseudocode for zero-indexed environments.

local function validate(list)
	if type(list) ~= "table" then
		error("The argument is not a valid list", 2)
	end

	for _, item in ipairs(list) do
		if type(item) ~= "number" then
			error("The list contains items that are not numerical types", 2)
		end

		-- Since Lua lacks a standard isnan or isfinite function (to keep the interpreter concise),
		-- checking a number against itself, or checking a number against Infinity/-Infinity will have to do.
		if item ~= item or item == math.huge or item == -math.huge then
			error("The list contains numerical types that do not represent a specific value", 2)
		end
	end
end

local function throwIfListIsEmpty(list)
	if #list == 0 then
		error("The list is empty; it contains no items", 2)
	end
end

function mean(list)
	validate(list)
	throwIfListIsEmpty(list)

	local accumulator = 0

	for _, item in ipairs(list) do
		accumulator = accumulator + item
	end

	return accumulator / #list
end

function median(list)
	validate(list)
	throwIfListIsEmpty(list)

	local sortedList = {}

	-- Copy the list so the original is not modified
	for i, item in ipairs(list) do
		sortedList[i] = item
	end

	table.sort(sortedList)

	local length = #sortedList

	if length % 2 == 0 then
		return (sortedList[length / 2] + sortedList[length / 2 + 1]) / 2
	else
		return sortedList[(length + 1) / 2]
	end
end

function mode(list)
	validate(list)
	throwIfListIsEmpty(list)

	local counts = {}
	local highestCount = 0

	for _, item in ipairs(list) do
		counts[item] = (counts[item] or 0) + 1

		if counts[item] > highestCount then
			highestCount = counts[item]
		end
	end

	-- No repeated values
	if highestCount == 1 then
		return {}
	end

	local commonValues = {}

	for key, value in pairs(counts) do
		if value == highestCount then
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
