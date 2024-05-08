//
// Created by Mihai on 12/20/23.
//

#ifndef MORSKOI_BOI_SHIPS_H
#define MORSKOI_BOI_SHIPS_H
//
//typedef struct point {
//    int x;
//    int y;
//} point;
//
//
//typedef struct {
//    point point1;
//    bool is_hit;
//} ship_point;


typedef struct ship {
    int pos_x;
    int pos_y;
    int length;
    char orientation;
//    ship_point ship_points[5];
} ship;


int count_set_ships;

const int count_max_ships;

int count_successful_shots;

int max_count_successful_shots;

int count_ships_5;
int count_ships_4;
int count_ships_3;
int count_ships_2;
int count_ships_1;


int count_max_ships_5;
int count_max_ships_4;
int count_max_ships_3;
int count_max_ships_2;
int count_max_ships_1;

#endif //MORSKOI_BOI_SHIPS_H
