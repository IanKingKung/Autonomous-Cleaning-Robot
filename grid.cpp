#include "grid.h"

void GRID::set_cell(int x, int y, uint8_t value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        map_data[y * width + x] = value;
    }
}

uint8_t GRID::get_cell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return map_data[y * width + x];
    }
    return 0; // Return default value for out-of-bounds access
}

void GRID::mark_occupied_point(const POINT2D& pt) {
    // Sensor sits at the center cell of the local grid
    int sensor_cell_x = width / 2;
    int sensor_cell_y = height / 2;

    // Convert meters from sensor (0,0) into grid cell offsets
    int grid_x = sensor_cell_x + static_cast<int>(std::round(pt.x / resolution));
    int grid_y = sensor_cell_y + static_cast<int>(std::round(pt.y / resolution));

    // Mark as occupied if within local grid bounds
    set_cell(grid_x, grid_y, 1);
}

void GRID::raytrace_and_mark(const POINT2D& hit_pt) {
    // Sensor sits at the center cell of the local grid
    int sensor_cell_x = width / 2;
    int sensor_cell_y = height / 2;

    // Convert meters from sensor (0,0) into grid cell offsets
    int hit_cell_x = sensor_cell_x + static_cast<int>(std::round(hit_pt.x / resolution));
    int hit_cell_y = sensor_cell_y + static_cast<int>(std::round(hit_pt.y / resolution));

    // Use Bresenham's line algorithm to mark cells along the ray as free
    int dx = std::abs(hit_cell_x - sensor_cell_x);
    int dy = std::abs(hit_cell_y - sensor_cell_y);
    int sx = (sensor_cell_x < hit_cell_x) ? 1 : -1; //determines direction of ray in x
    int sy = (sensor_cell_y < hit_cell_y) ? 1 : -1; //determines direction of ray in y
    int err = dx - dy;  //error term, used to determine whether to step in x or y direction

    int cur_x = sensor_cell_x;
    int cur_y = sensor_cell_y;

    while (cur_x != hit_cell_x || cur_y != hit_cell_y) {

        if (cur_x >= 0 && cur_x < width && cur_y >= 0 && cur_y < height) {
            set_cell(cur_x, cur_y, 0); // 0 = Free Space
        }

        int e2 = err * 2;   //scaled error
        if (e2 > -dy) { //compare scaled error to -dy as stepping in x reduces error by dy
            err -= dy;
            cur_x += sx;
        }
        if (e2 < dx) {  //compare scaled error to dx as stepping in y increases error by dx
            err += dx;
            cur_y += sy;
        }
    }

    if (hit_cell_x >= 0 && hit_cell_x < width && hit_cell_y >= 0 && hit_cell_y < height) {
        set_cell(hit_cell_x, hit_cell_y, 1); // Mark the hit point as occupied
    }
}

void GRID::render(SDL_Renderer* renderer, int cell_pixel_size) const {
    
    //create a rectangle size of cell_size x cell_size
    SDL_Rect rect;
    rect.w = cell_pixel_size;
    rect.h = cell_pixel_size;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t state = get_cell(x, y);

            // Choose color based on cell state
            if (state == 1) {
                // Occupied (Red)
                SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            } else if (state == 0) {
                // Free space (Light Gray / Off-white)
                SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
            } else {
                // Unexplored/Unknown (Dark Gray)
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            }

            //set position of rectangle to x,y in the grid
            rect.x = x * cell_pixel_size;
            rect.y = y * cell_pixel_size;
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    //Sensor location as a green dot at center
    int center_x = (width / 2) * cell_pixel_size;
    int center_y = (height / 2) * cell_pixel_size;
    rect.x = center_x;
    rect.y = center_y;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    SDL_RenderFillRect(renderer, &rect);
}