//
// Created by Mihai on 12/20/23.
//

#ifndef MORSKOI_BOI_FUNC_H
#define MORSKOI_BOI_FUNC_H

#include "ships.h"

bool in_list(const char *list, char item, int length_list);

void init_map(char *map);

void print_map(const char *map);

void get_ship_coords(int *length, int *x, int *y, char *orientation);

void set_ship_on_map(ship ship_item, char *map);

bool are_valid_ship_attributes(const ship ship_item);

void update_count_sets_ships(const ship ship_item);

bool check_close_coords(const char *map, ship ship_item);

void set_shot_coords(int *x, int *y);

bool are_valid_shot_coords(int *x, int *y);

bool is_hit(const char *map, int *x, int *y);

void update_map_after_shot(char *map_to_shot, int *x, int *y, bool status_shot);

bool is_ubit(char *map, int x, int y);


#endif //MORSKOI_BOI_FUNC_H
