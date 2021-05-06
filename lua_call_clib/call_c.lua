print("C path:", package.cpath)
print("Lua path:", package.path)
package.cpath = 'clib/?.so;'

-- 这里"require"的参数对应C库中"luaopen_mylib()"中的"mylib"。
local mylib = require "mylib"

function print_tb(tb)
	print(unpack(tb))
	for k,v in pairs(tb) do
		if type(v) == type({}) then
			print ("table index:"..k)
			for sk,sv in pairs(v) do
				print ("map:"..sk.."->"..sv)
			end
		else
			print ("map:"..k.."->"..v)
	
		end
	end
end

print ("---- numindex_subtable------")
tb=mylib.numindex_subtable(3,5)
print_tb(tb)

print ("---- get table------")

local tb = {"hello", "world", "end"}
local newtable, index, element = mylib.get_tb(tb, 2)

print(" newtable:",newtable,"element:",index)

print ("---- set  sess ------")

local rt1, rt2, rt3 = mylib.set_sess("filed", tb)

print("rt1:",rt1,"rt2:",rt2,"rt3:",rt3)
