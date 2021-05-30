print("C path:", package.cpath)
print("Lua path:", package.path)
package.cpath = package.cpath..';clib/?.so;'
local socket = require "socket"
--[[
use this command on your linux system :
#luarocks install luasocket

the use the next command to see the paths CONFIGURATION
#luarocks 
]]
-- 这里"require"的参数对应C库中"luaopen_mylib()"中的"mylib"。

local mylib = require "mylib"

function print_tb(tb)
    if type(v) == type({}) then
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
end

print ("---- numindex_subtable------")
tb=mylib.numindex_subtable(3,5)
print_tb(tb)

print ("---- get table------")

local tb = {"hello", "world", "end"}
local newtable, index, element = mylib.get_tb(tb, 2)

print(" newtable:",newtable,"element:",index)

print ("---- set  sess ------")
local start_time = socket.gettime()
local rt1, rt2, rt3 = mylib.set_sess("filed", tb)
os.execute('.ls')
local end_time= socket.gettime()

print("rt1:",rt1,"rt2:",rt2,"rt3:",rt3)

local use_time = (end_time - start_time )*1000
print("time:")
print(use_time)
