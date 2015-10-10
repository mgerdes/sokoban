#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "warehouse.h"
#include "level_reader.h"

Tile get_tile_for_char(char c) {
    Tile t;
    switch(c) {
        case ' ':
            t = EMPTY;
            break;
        case 'X':
            t = WALL;
            break;
        case '*':
            t = CRATE;
            break;
        case '@':
            t = PLAYER;
            break;
        case '.':
            t = DEST;
            break;
        default:
            t = EMPTY;
    }
    return t;
}

Warehouse* read_in_level(int level_num) {
    int height, width;
    int buffer_size = 255; // Effectively means levels can have a max width of 255.
    char* buffer = malloc(sizeof(char) * buffer_size);

    FILE* f = fopen("levels/level2", "r");
    if (!f) {
        printf("Error Opening Level File\n");
    }

    fgets(buffer, buffer_size, f);
    sscanf(buffer, "%d %d\n", &height, &width);

    Warehouse* w = create_warehouse(height, width);

    for (int row = 0; row < height; row++) {
        fgets(buffer, buffer_size, f);
        for (int col = 0; col < width; col++) {
            char c = buffer[col];
            w->tiles[row][col] = get_tile_for_char(c);
            if (w->tiles[row][col] == PLAYER) {
                w->player_row = row;
                w->player_col = col;
            }
        }
    }

    fclose(f);

    return w;
}
