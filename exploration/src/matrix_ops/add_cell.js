export function add_cell(x,y,north_wall,west_wall,south_wall,west_wall) {
    matrix[2*x + 1][2*y + 1] = 5; //rover currently in position (x,y)
    matrix[2*x][2*y +1] = west_wall;
    matrix[2*x+1][2*y] = south_wall;
    matrix[2*x+2][2*y +1] = east_wall;
    matrix[2*x + 1][2*y + 2] = north_wall;
}