//
// Created by Mihai on 12/20/23.
//
#include <stdio.h>
#include <stdbool.h>

#include "ships.h"
#include "defines.h"
#include "func.h"

bool in_list(const char *list, const char item, const int length_list) {
    for (int i = 0; i < length_list; ++i) {
        if (item == *(list + i)) {
            return true;
        }
    }
    return false;
}

void init_map(char *map) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            *(map + i * ROWS + j) = ' ';
            if (i == 0) {
                *(map + i * ROWS + j) = BORDER_SYMBOL;
            }
            if (i == ROWS - 1) {
                *(map + i * ROWS + j) = BORDER_SYMBOL;
            }
            if (j == 0) {
                *(map + i * ROWS + j) = BORDER_SYMBOL;
            }
            if (j == COLUMNS - 1) {
                *(map + i * ROWS + j) = BORDER_SYMBOL;
            }
        }
    }
}

void print_map(const char *map) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            printf("%c ", *(map + i * ROWS + j));
        }
        printf("\n");
    }
}

void get_ship_coords(int *length, int *x, int *y, char *orientation) {
    *orientation = 'O';
    printf("Introdu lungimea corăbiei: ");
    scanf("%d", length);

    if (*length != 1) {
        printf("O - Orizontal\nV - Vertical\nIntrodu orientarea corăbiei: ");
        scanf(" %c", orientation);
    }
    printf("Introdu rândul: ");
    scanf("%d", x);
    printf("Introdu coloana: ");
    scanf("%d", y);
}

void set_ship_on_map(const ship ship_item, char *map) {
    if (ship_item.length == 1) {
        *(map + ship_item.pos_x * ROWS + ship_item.pos_y) = SHIP_SYMBOL;
        return;
    }
    if (ship_item.orientation == 'O') {
        for (int i = 0; i < ship_item.length; ++i) {
            *(map + ship_item.pos_x * ROWS + ship_item.pos_y + i) = SHIP_SYMBOL;
        }
        return;
    }
    if (ship_item.orientation == 'V') {
        for (int i = 0; i < ship_item.length; ++i) {
            *(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y) = SHIP_SYMBOL;
        }
        return;
    }
}

bool are_valid_ship_attributes(const ship ship_item) {
    if ((ship_item.length == 1) && (count_ships_1 >= count_max_ships_1)) {
        printf("Sunt amplasate maxim corăbii de lungimea 1\n");
        return false;
    }
    if ((ship_item.length == 2) && (count_ships_2 >= count_max_ships_2)) {
        printf("Sunt amplasate maxim corăbii de lungimea 2\n");
        return false;
    }
    if ((ship_item.length == 3) && (count_ships_3 >= count_max_ships_3)) {
        printf("Sunt amplasate maxim corăbii de lungimea 3\n");
        return false;
    }
    if ((ship_item.length == 4) && (count_ships_4 >= count_max_ships_4)) {
        printf("Sunt amplasate maxim corăbii de lungimea 4\n");
        return false;
    }
    if ((ship_item.length == 5) && (count_ships_5 >= count_max_ships_5)) {
        printf("Sunt amplasate maxim corăbii de lungimea 5\n");
        return false;
    }


    if ((ship_item.length < 1) || (ship_item.length > 5)) {
        printf("Ai introdus lungimea greșită\n");
        return false;
    }

    if ((ship_item.pos_x < 1) || (ship_item.pos_x >= ROWS - 1)) {
        printf("Corabia iese din zona de amplasare 1\n");
        return false;
    }
    if ((ship_item.pos_y < 1) || (ship_item.pos_y >= COLUMNS - 1)) {
        printf("Corabia iese din zona de amplasare 2\n");
        return false;
    }

    if (ship_item.orientation == 'O') {
        if (ship_item.length + ship_item.pos_y > COLUMNS - 1) {
            printf("Corabia iese din zona de amplasare 3\n");
            return false;
        }
    }
    if (ship_item.orientation == 'V') {
        if (ship_item.length + ship_item.pos_x > ROWS - 1) {
            printf("Corabia iese din zona de amplasare 4\n");
            return false;
        }
    }
    if ((ship_item.orientation != 'O') && (ship_item.orientation != 'V')) {
        printf("Ai introdus orientare greșită\n");
        return false;
    }

    return true;
}

void update_count_sets_ships(const ship ship_item) {
    count_set_ships++;

    if (ship_item.length == 1) {
        count_ships_1++;
        return;
    }
    if (ship_item.length == 2) {
        count_ships_2++;
        return;
    }
    if (ship_item.length == 3) {
        count_ships_3++;
        return;
    }
    if (ship_item.length == 4) {
        count_ships_4++;
        return;
    }
    if (ship_item.length == 5) {
        count_ships_5++;
        return;
    }
}

bool check_close_coords(const char *map, const ship ship_item) {
    if (ship_item.length == 1) {
        for (int i = -1; i < 2; ++i) {
            if (*(map + ship_item.pos_x * ROWS + ship_item.pos_y + i) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii oriz\n");
                return false;
            }
            if (*(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii vert\n");
                return false;
            }
            if (*(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y + i) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii diag princ\n");
                return false;
            }
            if (*(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y - i) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii diag sec\n");
                return false;
            }
        }
    }

    if ((ship_item.orientation == 'O') && (ship_item.length > 1)) {
        for (int i = -1; i < ship_item.length + 1; ++i) {
            if (*(map + ship_item.pos_x * ROWS + ship_item.pos_y + i) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii oriz\n");
                return false;
            }
            if (*(map + (ship_item.pos_x - 1) * ROWS + ship_item.pos_y + i) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii oriz - 1\n");
                return false;
            }
            if (*(map + (ship_item.pos_x + 1) * ROWS + ship_item.pos_y + i) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii oriz + 1\n");
                return false;
            }
        }
    }

    if ((ship_item.orientation == 'V') && (ship_item.length > 1)) {
        for (int i = -1; i < ship_item.length + 1; ++i) {
            if (*(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii\n");
                return false;
            }
            if (*(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y - 1) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii - 1\n");
                return false;
            }
            if (*(map + (ship_item.pos_x + i) * ROWS + ship_item.pos_y + 1) == SHIP_SYMBOL) {
                printf("Corabia se află in zona altor corăbii + 1\n");
                return false;
            }
        }
    }

    return true;
}

void get_shot_coords(int *x, int *y) {
    printf("Introdu rândul: ");
    scanf("%d", x);
    printf("Introdu coloana: ");
    scanf("%d", y);
}

bool are_valid_shot_coords(int *x, int *y) {
    if ((*x < 1) || (*x > ROWS - 2)) {
        return false;
    }
    if ((*y < 1) || (*y > COLUMNS - 2)) {
        return false;
    }
    return true;
}

bool is_hit(const char *map, int *x, int *y) {
    if (*(map + *x * ROWS + *y) == SHIP_SYMBOL) {
        return true;
    }
    return false;
}

void update_map_after_shot(char *map_to_shot, int *x, int *y, const bool status_shot) {
    if (status_shot == true) {
        *(map_to_shot + *x * ROWS + *y) = HIT_SYMBOL;
        return;
    }
    if (*(map_to_shot + *x * ROWS + *y) == HIT_SYMBOL) {
        return;
    }

    *(map_to_shot + *x * ROWS + *y) = MIMO_SYMBOL;
}

bool is_ubit(char *map, int x, int y) {
    bool is_ship_N = false;
    bool is_ship_S = false;
    bool is_ship_E = false;
    bool is_ship_W = false;

    char list_symbols_no_ship[] = {' ', BORDER_SYMBOL, MIMO_SYMBOL};

    int length_list;
    length_list = sizeof(list_symbols_no_ship) / sizeof(list_symbols_no_ship[0]);


    /*
     * X X if == H return true else if == % or - or ' ' -> return false
     */

    // Check is_ship_E
    for (int i = 1; i < 5; ++i) {
        printf("Proverka is_ship_E\t %c\tx = %d\ty + i = %d\n", *(map + x * ROWS + y + i), x, y + i);
        if (in_list(list_symbols_no_ship, *(map + x * ROWS + y + i), length_list)) {
            is_ship_E = false;
            break;
        }
        if (*(map + x * ROWS + y + i) == SHIP_SYMBOL) {
            is_ship_E = true;
            break;
        }
    }

    // Check is_ship_W
    for (int i = 1; i < 5; ++i) {
        printf("Proverka is_ship_W\t%c\tx = %d\ty - i = %d\n", *(map + x * ROWS + y - i), x, y - i);
        if (in_list(list_symbols_no_ship, *(map + x * ROWS + y - i), length_list)) {
            is_ship_W = false;
            break;
        }
        if (*(map + x * ROWS + y - i) == SHIP_SYMBOL) {
            is_ship_W = true;
            break;
        }
    }

    // Check is_ship_N
    for (int i = 1; i < 5; ++i) {
        printf("Proverka is_ship_N\t %c\tx - i = %d\ty = %d\n", *(map + (x - i) * ROWS + y), x - i, y);
        if (in_list(list_symbols_no_ship, *(map + (x - i) * ROWS + y), length_list)) {
            is_ship_N = false;
            break;
        }
        if (*(map + (x - i) * ROWS + y) == SHIP_SYMBOL) {
            is_ship_N = true;
            break;
        }
    }

    // Check is_ship_S
    for (int i = 1; i < 5; ++i) {
        printf("Proverka is_ship_S\t %c\tx + i = %d\ty = %d\n", *(map + (x + i) * ROWS + y), x + i, y);
        if (in_list(list_symbols_no_ship, *(map + (x + i) * ROWS + y), length_list)) {
            is_ship_S = false;
            break;
        }
        if (*(map + (x + i) * ROWS + y) == SHIP_SYMBOL) {
            is_ship_S = true;
            break;
        }
    }

    if (is_ship_N) {
        return false;
    }
    if (is_ship_S) {
        return false;
    }
    if (is_ship_E) {
        return false;
    }
    if (is_ship_W) {
        return false;
    }

    return true;
}