#ifndef __PLAYER_H__
#define __PLAYER_H__

struct Player {

    Player() : num_knocked(0), num_off(0), score(0){};

    int num_knocked;
    int num_off;
    int score;
};
#endif