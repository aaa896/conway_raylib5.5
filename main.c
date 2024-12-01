#include <stdio.h>
#include "raylib.h"

#define COLS  50
#define ROWS  50

const int window_width  = 1000;
const int window_height = 1000;
const int cell_width = window_width / COLS;
const int cell_height = window_height / ROWS;

bool cells[ROWS][COLS] = {0};
bool cells_next_state[ROWS][COLS] = {0};


void draw_grid(bool cells[ROWS][COLS]) {
    for (int col = 0; col <COLS; ++col) {
        for (int row = 0; row < ROWS; ++row) {
            DrawRectangleLines(col * cell_width, row * cell_height, cell_width, cell_height, RED);
            if (cells[row][col]) {
                DrawRectangle(col * cell_width, row * cell_height, cell_width, cell_height, BLUE);
            }

        }
    }
}

void copy_source_cells_to_dest(bool source[ROWS][COLS] , bool dest[ROWS][COLS]) {
    for (int row = 0; row <ROWS; ++row) {
        for (int col = 0; col <COLS; ++col) {
            dest[row][col] = source[row][col];
        }
    }
}

typedef struct {
    int x;
    int y;
}V2int;

void update_cells(bool cells[ROWS][COLS]) {
    for (int col = 0; col <COLS; ++col) {
        for (int row = 0; row < ROWS; ++row) {
            bool left_cell  = cells[row][col -1];
            bool right_cell = cells[row][col +1];
            bool up_cell = cells[row -1][col ];
            bool up_right_cell = cells[row -1][col +1];
            bool up_left_cell = cells[row -1][col -1];
            bool down_cell = cells[row +1][col ];
            bool down_right_cell = cells[row +1][col +1];
            bool down_left_cell = cells[row +1][col -1];

            if (row == 0) {
                up_cell = cells[ROWS -1][col];
                up_right_cell = cells[ROWS -1][col +1];
                up_left_cell = cells[ROWS -1][col -1];
            }
            if (row == ROWS -1) {
                down_cell = cells[0][col];
                down_right_cell = cells[0][col +1];
                down_left_cell = cells[0][col -1];
            }
            if (col == (COLS -1) ) {
                right_cell = cells[row][0];
                up_right_cell = cells[row -1][0];
                down_right_cell = cells[row +1][0];
            }
            if (col == 0) {
                left_cell = cells[row][COLS -1];
                up_left_cell = cells[row -1][COLS -1];
                down_left_cell = cells[row +1][COLS -1];
            }


            int neighbors = 0;
            if (left_cell  ) ++ neighbors;
            if (right_cell ) ++ neighbors;
            if (up_cell ) ++ neighbors;
            if (up_right_cell ) ++ neighbors;
            if (up_left_cell ) ++ neighbors;
            if (down_cell ) ++ neighbors;
            if (down_right_cell) ++ neighbors;
            if (down_left_cell)  ++ neighbors;

            if (cells[row][col] ) {
                if (neighbors <2)          cells_next_state[row][col] = false;
                else if (neighbors > 3)    cells_next_state[row][col] = false;
            }
            else if (neighbors == 3)  {
                cells_next_state[row][col] = true;
            }
        }
    }
}


int main() {
    InitWindow(window_width, window_height, "conway");

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();
    SetTargetFPS(15);

    typedef enum {
        select,
        running,
    }State;
    State state = select;
    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (state) {
            case select: {
                             if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                                 mouse_x = GetMouseX();
                                 mouse_y = GetMouseY();
                                 int cell_x = mouse_x / cell_width;
                                 int cell_y = mouse_y / cell_height;
                                 cells[cell_y][cell_x] = true;
                                 copy_source_cells_to_dest(cells, cells_next_state);
                             }
                         }break;
            case running:{
                             update_cells(cells);
                                 copy_source_cells_to_dest(cells_next_state, cells);
                         }break;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (state == select) state = running;
            else state = select;
        }

        ClearBackground(BLACK);
        draw_grid(cells);

        EndDrawing();
    }
    return 0;
}
