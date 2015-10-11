#include <stdlib.h>
#include "warehouse.h"

Warehouse* create_warehouse(int height, int width) {
    Warehouse* w = malloc(sizeof(Warehouse));
    w->player_row = w->player_col = 0;
    w->num_of_crates = w->num_of_crates_on_dests = 0;

    w->height = height;
    w->width = width;

    w->tiles = malloc(sizeof(Tile*) * height);
    w->original_tiles = malloc(sizeof(Tile*) * height);
    for (int row = 0; row < height; row++) {
        w->tiles[row] = malloc(sizeof(Tile) * width);
        w->original_tiles[row] = malloc(sizeof(Tile) * width);
    }
    return w;
}

void delete_warehouse(Warehouse* w) {
    for (int row = 0; row < w->height; row++) {
        free(w->tiles[row]);
        free(w->original_tiles[row]);
    }
    free(w->tiles);
    free(w->original_tiles);
    free(w);
}

int is_loc_in_warehouse(Warehouse* w, int row, int col) {
    return row < w->height && row >= 0
        && col < w->width && col >= 0;
}

int is_loc_wall(Warehouse* w, int row, int col) {
    return w->tiles[row][col] == WALL;
}

int is_loc_crate(Warehouse* w, int row, int col) {
    return w->tiles[row][col] == CRATE 
        || w->tiles[row][col] == CRATE_ON_DEST;
}

int can_move_second_crate(Warehouse* w, int row, int col) {
    return !is_loc_crate(w, row, col)
        && !is_loc_wall(w, row, col)
        && is_loc_in_warehouse(w, row, col);
}

void remove_crate_from_loc(Warehouse* w, int row, int col) {
    if (w->tiles[row][col] == PLAYER_ON_DEST) {
        w->tiles[row][col] = DEST;
    } else if (w->tiles[row][col] == CRATE_ON_DEST) {
        w->tiles[row][col] = DEST;
        w->num_of_crates_on_dests--;
    } else {
        w->tiles[row][col] = EMPTY;
    }
}

void set_loc_player(Warehouse* w, int row, int col) {
    if (w->tiles[row][col] == DEST) {
        w->tiles[row][col] = PLAYER_ON_DEST;
    } else {
        w->tiles[row][col] = PLAYER;
    }
    w->player_row = row;
    w->player_col = col;
}

void set_loc_crate(Warehouse* w, int row, int col) {
    if (w->tiles[row][col] == DEST) {
        w->tiles[row][col] = CRATE_ON_DEST;
        w->num_of_crates_on_dests++;
    } else {
        w->tiles[row][col] = CRATE;
    }
}

void move_player_direction(Warehouse* w, int row_dir, int col_dir) {
    int new_player_row = w->player_row + row_dir;
    int new_player_col = w->player_col + col_dir;

    if (!is_loc_in_warehouse(w, new_player_row, new_player_col)
            || is_loc_wall(w, new_player_row, new_player_col))  
        return;

    if (is_loc_crate(w, new_player_row, new_player_col)) {
        if  (!can_move_second_crate(w, new_player_row + row_dir, new_player_col + col_dir)) {
            return;
        } else {
            remove_crate_from_loc(w, w->player_row, w->player_col);
            remove_crate_from_loc(w, new_player_row, new_player_col);
            set_loc_player(w, new_player_row, new_player_col);
            // Move the crate as well.
            set_loc_crate(w, new_player_row + row_dir, new_player_col + col_dir);
        }
    } else {
        remove_crate_from_loc(w, w->player_row, w->player_col);
        set_loc_player(w, new_player_row, new_player_col);
    }

}

void move_player_left(Warehouse* w) {
    move_player_direction(w, 0, -1);
}

void move_player_right(Warehouse* w) {
    move_player_direction(w, 0, 1);
}

void move_player_up(Warehouse* w) {
    move_player_direction(w, -1, 0);
}

void move_player_down(Warehouse* w) {
    move_player_direction(w, 1, 0);
}

void reset_warehouse(Warehouse* w) {
    w->num_of_crates_on_dests = 0;
    for (int row = 0; row < w->height; row++) {
        for (int col = 0; col < w->width; col++) {
            w->tiles[row][col] = w->original_tiles[row][col];
            if (w->original_tiles[row][col] == PLAYER) {
                w->player_row = row;
                w->player_col = col;
            }
        }
    }
}

int is_warehouse_won(Warehouse* w) {
    return w->num_of_crates == w->num_of_crates_on_dests;
}
