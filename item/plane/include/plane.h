#include<iostream>
#include<string>
#include<vector>
using namespace std;


typedef enum{
    NORTH = 0, //y
    EAST = 1,  //X
    SOUTH = 2,//-Y
    WEST = 3,// -X
}Direct;
typedef enum{
    CMD_UP,
    CMD_DOWN,
    CMD_FORWARD,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_ROUND,
}CMD;
typedef enum{
    POS_X = 0,
    POS_Y,
    POS_Z,
}POS;
/*
class Instruction{
    public:
        Instruction(Direct direct, int step);

}
*/
class Plane{
public:
    Plane(int x, int y, int z, Direct dire );
    ~Plane();


    vector<int> CmdArray(vector<vector<int>>);
    vector<int> ExecCmd(CMD cmd, int step = 1);
    vector<int> RepeatCmd(CMD cmd, int step = 1, int times = 1);
private:
    Direct  direction;
    vector<int>  position;


};

vector<int> Position(int x, int y, int z, Direct  dire);

