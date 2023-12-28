/**
 * @file constants.h
 * @author Joshua Beatty, Zhuowen Song, Keming Chen, Matthew Whitaker
 * @brief Header file that contains essential game data
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <vector>
#include <QString>

enum MapTile {
    start = 0,
    ground = 1,
    wall = 2,
    cheese = 3,
    block = 4,
    pit = 5,
};

enum ProgramBlock {
    beginBlock = 9,
    moveForward = 1,
    turnLeft = 2,
    turnRight = 3,
    ifStatement = 4,
    endIf = 5,
    whileLoop = 6,
    endWhile = 7,
    eatCheese = 8,
    conditionNot = 0,
    conditionFacingBlock = -1,
    conditionFacingWall = -2,
    conditionFacingPit = -3,
    conditionFacingCheese = -4,

    blank = 10
};

enum gameState {
    notEnded = 0,
    won = 1,
    lost = 2,
};

enum direction {
    north = 0,
    south = 1,
    east = 2,
    west = 3
};

/// Defines the levels for the game
const std::vector<std::vector<MapTile>> levels[] = {
    // Level 1
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{ground, start, ground,  ground,  cheese, ground}
    },

    // Level 2
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground,  wall,   cheese},
        std::vector<MapTile>{wall,  ground,  ground,  ground },
        std::vector<MapTile>{wall,  wall,  wall,  wall }
    },

    // Level 3
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground, block,  ground,  ground},
        std::vector<MapTile>{wall,  wall,  wall, ground,  wall},
        std::vector<MapTile>{wall,  wall,  wall, cheese,  wall},
        std::vector<MapTile>{wall,  wall,  wall, wall,  wall}
    },

    // Level 4
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground, ground,  ground,  ground,  ground, wall },
        std::vector<MapTile>{wall,  wall,   wall,    wall,    wall,    ground, wall },
        std::vector<MapTile>{wall,  ground, ground,  ground,  wall,    ground, wall },
        std::vector<MapTile>{wall,  ground, wall,    cheese,  wall,    ground, wall },
        std::vector<MapTile>{wall,  ground, wall,    wall,    wall,    ground, wall },
        std::vector<MapTile>{wall,  ground, ground,  ground,  ground,  ground, wall },
        std::vector<MapTile>{wall,  wall,   wall,    wall,    wall,    wall, wall }
    },

    // Level 5
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground, ground,  ground,  ground,  wall },
        std::vector<MapTile>{wall,  wall,   wall,    wall,    ground,  wall },
        std::vector<MapTile>{wall,  ground, ground,  ground,  ground ,  wall },
        std::vector<MapTile>{wall,  ground, wall,    wall,    wall,     wall },
        std::vector<MapTile>{wall,  ground, ground,  ground,  ground,  wall },
        std::vector<MapTile>{wall,  wall,   wall,    wall,    ground,  wall },
        std::vector<MapTile>{wall,  cheese, ground,  ground,  ground ,  wall },
        std::vector<MapTile>{wall,  wall, wall,    wall,    wall,     wall }
    },

    // Level 6
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground,  wall,   wall,    wall,   cheese, wall},
        std::vector<MapTile>{wall,  ground,  wall,   ground,  ground, ground, pit},
        std::vector<MapTile>{wall,  ground,  block, ground,  pit,    wall,   wall},
        std::vector<MapTile>{wall,  pit,     wall,   wall,    wall,   wall,   wall}
    },


    // Level 7
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground, ground, ground, ground, wall },
        std::vector<MapTile>{wall, wall,    wall,   wall,   block, wall },
        std::vector<MapTile>{wall, ground,  ground, ground, ground, wall },
        std::vector<MapTile>{wall, ground,  wall,   wall,   ground, wall },
        std::vector<MapTile>{wall, ground,  ground, ground, ground, cheese },
        std::vector<MapTile>{wall, wall, wall, wall, wall, wall }
    },


    // Level 8
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, wall,  cheese, wall, pit, wall,   wall},
        std::vector<MapTile>{ground, ground,  block, block, block, ground,   wall},
        std::vector<MapTile>{ground, ground,  ground, ground, ground, pit,   wall},
        std::vector<MapTile>{wall, wall,  wall, wall, wall, wall,   wall}
    },

    // Level 9
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground,  ground, ground, ground, wall,   wall,   wall,   wall,   wall},
        std::vector<MapTile>{wall,  wall,    wall,   wall,   block,  wall,   ground, ground, ground, wall },
        std::vector<MapTile>{wall,  ground,  ground, ground, ground, wall,   ground, wall,   ground, wall },
        std::vector<MapTile>{wall,  ground,  wall,   wall,   ground, wall,   ground, wall,   ground, wall },
        std::vector<MapTile>{wall,  ground,  ground, ground, ground, ground, block, ground, ground, wall },
        std::vector<MapTile>{wall,  wall,    wall,   wall,   wall,   wall,   wall,   wall,   cheese, wall }
    },

    // Level 10
    std::vector<std::vector<MapTile>>{
        std::vector<MapTile>{start, ground,  wall, wall, wall},
        std::vector<MapTile>{wall,  ground,  ground, wall, wall },
        std::vector<MapTile>{wall,  wall,  ground,   ground,   wall },
        std::vector<MapTile>{wall,  wall,  wall, ground, ground  },
        std::vector<MapTile>{wall,  wall,  wall,   wall,   cheese    }
    }

};

/// These messages are shown at the beginning of each level, to help the user learn and encourage them!
const auto educationalMessages = std::vector<QString>{
        //Level 1
        "Welcome!\n\nThe mice need your help.\nThey've built and designed a new robot to make cheese collection quick and efficient. But, without programming, the robot can't do anything!\n\nWill you write a program to help the robot reach the cheese? Just moving forward a few times should be a good way to start.",

        //Level 2
        "The robot's small size allows it to go around corners and reach areas the mice can't reach on their own.\n\nCan you write a program to help the robot reach the cheese without bumping into any walls?",


        //Level 3
        "The robot is even strong enough to move obstacles to access new cheese! Help move the crate and get to the cheese",

        //Level 4
        "Some journeys are a little long for this robot, but thanks to your excellent programming, it can make it.\n\nSometimes in programming it is helpful to use a loop, so you don't have to put the same thing over and over again. Try using the robot's sensors with a 'while' block this time to get to the cheese. Something like 'while not facing wall' might do the trick! Don't forget an 'end while' block.",

        //Level 5
        "Programming makes all sorts of tedious tasks easier. The mice are sure grateful for your help collecting cheese! This next piece of cheese is down a long winding corridor.\n\nYou might need to try using an 'if' block for this one. Note that every 'if' block needs an 'end' block as well.",

        //Level 6
        "Now that you've proven your programming prowess, the mice would like you to put your skills to the test. You're now ready to take the robot through a dangerous course now, full of pits! Whatever you do, don't let the robot fall in! Thankfully the robot is equipped with a pit sensor, so you should be able to write a program that safely avoids all the pits!",

        //Level 7
        "Could you give this one a try using some while loops? Just don't push the crate too far, or the robot won't be able to reach the cheese!",

        //Level 8
        "The mice would love to reach this cheese, but they're worried it isn't possible. Will you give it a go?",

        //Level 9
        "You're getting the hang of this! Great job! See how few blocks you can use to help the robot reach the cheese!",

        //Level 10
        "You'll definitely want loops on this one! Keep up the great work!",
};

#endif // CONSTANTS_H
