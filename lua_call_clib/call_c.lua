print("C path:", package.cpath)
print("Lua path:", package.path)
package.cpath = 'clib/?.so;'

-- 这里"require"的参数对应C库中"luaopen_mylib()"中的"mylib"。
local mylib = require "mylib"

print(mylib.mysin(3.14 / 2))

tb=mylib.func_return_table()
print(type(tb))

print(unpack(tb))
for k,v in pairs(tb) do
	print (k)
	print (v)
end

tb=mylib.return_table()
print(type(tb))

print(unpack(tb))
for k,v in pairs(tb) do
	print (k)
	print (v)
end

print ("numindex_subtable")
tb=mylib.numindex_subtable()
print(type(tb))

print(unpack(tb))
for k,v in pairs(tb) do
	print (k)
	for sk,sv in pairs(v) do
		print (sk)
		print (sv)
	end
end
