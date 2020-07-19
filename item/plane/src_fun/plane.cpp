#include "plane.h"
#include <vector>
using namespace std;



Plane::Plane(int x, int y, int z, Direct  dire)
{
    position = {x, y, z, dire};
    direction = dire;
    for (int pos : position) //单个字符
        cout <<  pos << "," ;
    cout<<endl;
}

Direct GetDire(Direct direction, CMD cmd)
{
    int dire;
    if(cmd == CMD_LEFT)
    {
        dire = direction - 1;
        dire = (Direct)dire < NORTH ? WEST : dire;
    }
    if(cmd == CMD_RIGHT)
    {
        dire = direction + 1;
        dire = (Direct)dire > WEST ? NORTH : dire;
    }
    if(cmd == CMD_ROUND)
    {
         switch (direction)
        {
        case NORTH:
            return SOUTH;
                 break;
        case SOUTH:
            return NORTH;
                 break;
        case EAST:
            return WEST;
                 break;
        case WEST:
            return EAST;
                 break;
        }
    }

    switch (dire)
    {
    case NORTH:
        return NORTH;
             break;
    case EAST:
        return EAST;
             break;
    case SOUTH:
        return SOUTH;
             break;
    case WEST:
        return WEST;
             break;
    }
    return WEST;
}


vector<int>Plane::ExecCmd(CMD cmd, int step)
{
    switch (cmd)
    {
        case CMD_UP:
             position[POS_Z] = position[POS_Z] + step;
             break;

        case CMD_DOWN:
            {
                position[POS_Z] = position[POS_Z] - step > 0 ? position[POS_Z] - step : 0;
                break;
            }
        case CMD_FORWARD:
             {
                switch (direction)
                {
                    case NORTH:
                         position[POS_Y] = position[POS_Y] + step;
                         break;
                    case EAST:
                         position[POS_X] = position[POS_X] + step;
                         break;
                    case SOUTH:
                         position[POS_Y] = position[POS_Y] - step;
                         break;
                    case WEST:
                         position[POS_X] = position[POS_X] - step;
                         break;
                }
             }
             break;
        case CMD_LEFT:
             direction = GetDire( direction, cmd);
             break;
        case CMD_RIGHT:
             direction = GetDire( direction, cmd);
             break;
        case CMD_ROUND:
             direction = GetDire( direction, cmd);
             break;

    }
    position[3] = direction;
    return position;
}

vector<int>Plane:: RepeatCmd(CMD cmd, int step , int times)
{
    while(times>0)
    {
        ExecCmd(cmd, step);
        times--;
    }

    return position;
}
vector<int>Plane:: CmdArray(vector<vector<int>>cmdArray)
{

    for (vector<vector<int>>::iterator it = cmdArray.begin() ; it != cmdArray.end(); ++it)
    {
        vector<int> vect =*it;
        switch (vect[0])
        {
            case  CMD_UP :
                ExecCmd(CMD_UP, vect[1]);
                break;
            case  CMD_DOWN:
                ExecCmd(CMD_DOWN, vect[1]);
                break;
            case  CMD_FORWARD:
                ExecCmd(CMD_FORWARD, vect[1]);
                break;

            case  CMD_LEFT:
                ExecCmd( CMD_LEFT, vect[1]);
                break;

            case  CMD_RIGHT:
                ExecCmd(CMD_RIGHT, vect[1]);
                break;

            case  CMD_ROUND:
                ExecCmd(CMD_ROUND, vect[1]);
                break;
        }
     }

    return position;
}

vector<int> Position(int x, int y, int z, Direct  dire)
{
    vector<int> pos = {x, y, z, dire};
    return pos;
}

Plane::~Plane()
{
}

