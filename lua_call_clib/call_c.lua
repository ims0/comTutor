print("C path:", package.cpath)
print("Lua path:", package.path)
package.cpath = 'clib/?.so;'

-- 这里"require"的参数对应C库中"luaopen_mylib()"中的"mylib"。
local mylib = require "mylib"

print(mylib.mysin(3.14 / 2)) --> 0.99999968293183
