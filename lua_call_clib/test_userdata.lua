
package.cpath = 'userdata/?.so;'

local array = require "array"

a = array.new(1000)
print(a)               --> userdata: 0x8064d48
print(array.size(a))   --> 1000
for i=1,1000 do
  array.set(a, i, 1/i)
end
print(array.get(a, 1))  --> 1
print(array.get(a, 10))  --> 0.1
print(array.get(a, 100))  --> 0.01

-- second
a = array.new_s(1000)
print(a)               --> userdata: 0x8064d48
print(array.size_s(a))   --> 1000
for i=1,1000 do
  array.set_s(a, i, 1/i)
end
print(array.get_s(a, 1))  --> 1
print(array.get_s(a, 10))  --> 0.1
print(array.get_s(a, 100))  --> 0.01
