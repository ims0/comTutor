fin = open("CMakeLists.txt", "r")
fout = open("ycmCfg.txt", "w")

cxt = fin.readlines()
ckFile = "/CMakeLists.txt"
dict = { }
dict['CMAKE_CURRENT_SOURCE_DIR'] = './'

def procSubDir(line):
    print(line)
    subPath = line[line.find("(")+1 : -1]
    print(subPath + ckFile)


def procIncludeDir(line, cxt):
    print(line)

def procSet(line):
    if(line.strip().find("set") == 0 or line.strip().find("SET") == 0):
        print(line)
        inbrace = line[line.find("(")+1 : -1].split()
        var = inbrace[1]
        if(inbrace[1].find("$") != -1):
            var = var[ var.find("{")+1 : var.find("}")]
            print(var)
            print(inbrace[1][ inbrace[1].find("}")+1 :-1])
            var = dict[var] + inbrace[1][ inbrace[1].find("}")+1 :-1]
            print(var)
        dict[inbrace[0]] = var.strip().strip('"') 


cnt = 0

while cnt < len(cxt):
    line = cxt[cnt].strip()
    if(line.strip().find("add_subdirectory") == 0 or line.strip().find("ADD_SUBDIRECTORY") == 0):
        procSubDir(line)
    if(line.strip().find("include_directories") == 0 or line.strip().find("INCLUDE_DIRECTORIES") == 0):
        procIncludeDir(line, cxt)
    procSet(line)
    cnt = cnt+1
print(dict)
