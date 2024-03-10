int pixel_buffer_start; // global variable



// code not shown for clear_screen() and draw_line() subroutines


void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

        *one_pixel_address = line_color;
}


void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
            plot_pixel(x, y, 0); // Set pixel color to black (0x0000)
        }
    }
}


void draw_line(int x0, int y0, int x1, int y1, short int line_color) {
    if (y0 == y1) {
        // Horizontal line
        int x_start = (x0 < x1) ? x0 : x1;
        int x_end = (x0 < x1) ? x1 : x0;
        for (int x = x_start; x <= x_end; x++) {
            plot_pixel(x, y0, line_color);
        }
    } else {
        // Invalid input, do nothing
    }
}



int main(void){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;
    *(volatile int*)(pixel_ctrl_ptr + 1) = pixel_buffer_start;

    clear_screen();
    int direction = -1;
    int row = 0;
    while(1){
        draw_line(0,row,319,row, 0x07E0); //Draw green line
        *pixel_ctrl_ptr = 0x1;
        while(*(volatile int*)(pixel_ctrl_ptr + 3) & 0x1){

        }
        draw_line(0,row,319,row,0x0000); //Draw black line on top 
        
        if(row == 0 || row == 239){
            direction = - direction;
        }
        row = row + direction;
    }

    
}