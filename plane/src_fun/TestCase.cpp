
#include <gtest/gtest.h>
#include "plane.h"
TEST(AirCraftTest, cmd_up)
{
    Plane plane(0,0,5, EAST);
    EXPECT_EQ(plane.ExecCmd(CMD_UP),Position(0,0,6,EAST));
}
TEST(AirCraftTest, cmd_down)
{
    Plane plane(0,0,0, EAST);
    EXPECT_EQ(plane.ExecCmd(CMD_DOWN),Position(0,0,0,EAST));
}
TEST(AirCraftTest, cmd_down_move)
{
    Plane plane(0,0,1, EAST);
    EXPECT_EQ(plane.ExecCmd(CMD_DOWN),Position(0,0,0,EAST));
}

TEST(AirCraftTest, cmd_LEFT_N2W)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.ExecCmd(CMD_LEFT),Position(0,0,0,WEST));
}
TEST(AirCraftTest, cmd_LEFT_S2E)
{
    Plane plane(0,0,0, SOUTH);
    EXPECT_EQ(plane.ExecCmd(CMD_LEFT),Position(0,0,0,EAST));
}

TEST(AirCraftTest, cmd_RIGHT)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.ExecCmd(CMD_RIGHT),Position(0,0,0,EAST));
}
TEST(AirCraftTest, cmd_ROUND_N2S)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.ExecCmd(CMD_ROUND),Position(0,0,0,SOUTH));
}
TEST(AirCraftTest, cmd_ROUND_E2W)
{
    Plane plane(0,0,0, EAST);
    EXPECT_EQ(plane.ExecCmd(CMD_ROUND),Position(0,0,0,WEST));
}
TEST(AirCraftTest, cmd_Mutli_N2W2E2S)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.ExecCmd(CMD_LEFT), Position(0,0,0,WEST));
    EXPECT_EQ(plane.ExecCmd(CMD_ROUND), Position(0,0,0,EAST));
    EXPECT_EQ(plane.ExecCmd(CMD_RIGHT), Position(0,0,0,SOUTH));
}
//3
TEST(AirCraftTest, cmd_FORWARD_10)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.ExecCmd(CMD_FORWARD, 10), Position(0,10,0,NORTH));
    EXPECT_EQ(plane.ExecCmd(CMD_UP, 10), Position(0,10,10,NORTH));
    EXPECT_EQ(plane.ExecCmd(CMD_DOWN, 5), Position(0,10,5,NORTH));
}

//4
TEST(AirCraftTest, cmd_repeat_10)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.RepeatCmd(CMD_FORWARD, 5), Position(0,5,0,NORTH));
    EXPECT_EQ(plane.RepeatCmd(CMD_LEFT, 1), Position(0,5,0, WEST));
    EXPECT_EQ(plane.RepeatCmd(CMD_FORWARD, 5), Position(-5,5,0, WEST));
    EXPECT_EQ(plane.RepeatCmd(CMD_UP, 5), Position(-5,5,5, WEST));
}
TEST(AirCraftTest, cmd_Mutli_array)
{
    Plane plane(0,0,0, NORTH);
    EXPECT_EQ(plane.CmdArray({{CMD_UP, 10},{CMD_RIGHT,1},{CMD_FORWARD,10},
{CMD_LEFT,1},{CMD_FORWARD,5},{CMD_ROUND}}), Position(10,5,10,SOUTH));

}
