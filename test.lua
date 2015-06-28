print("This is test.lua")
print("Lua version is", _VERSION)

local function test(val, expect, ...)
	if val ~= expect then
		error("Expected "..tostring(expect).." but got "..tostring(val), 2)
	end
	return val, expect, ...
end

test(tostring(1), "1")
test(tostring(20), "20")
test(('%20s'):format('abc'), '                 abc')
--test(1 << 1, 2)

print("Test OK")
