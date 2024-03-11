int pixel_buffer_start; // global variable
#include <stdbool.h>
#include <stdlib.h>

short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];
int boxSize = 8;
short int line_colours[10] = {0xFFFF,0x0001,0x07E0, 0x3456, 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF};

void plot_pixel(int x, int y, short int line_color);

void getRand(int xCoord[8], int yCoord[8], int xStep[8], int yStep[8], short int line_colour[10]){
    for (int i = 0; i < 8; i++){
        xCoord[i] = rand() % 317;
        yCoord[i] = rand() % 237;
        xStep[i] = ((rand() % 2) * 2) - 1;
        yStep[i] = ((rand() % 2) * 2) - 1;

        line_colour[i] = line_colours[rand() % 10];
    }
}

void swap( int * x, int * y){
    int i = *x;
    *x = *y;
    *y = i;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color)
{
    bool is_steep = false ; 
	int ytemp = abs(y1 - y0);
	int xtemp = abs(x1 - x0);
	if(ytemp > xtemp){
		is_steep = true;
	}
    if (is_steep){
        swap(&x1, &y1);
        swap(&x0, &y0);
    }
    if (x0 > x1){
        swap(&y0, &y1);
        swap(&x0, &x1);
    }


    int deltaY = abs(y1 - y0);
    int deltaX = x1 - x0;
    int error = - (deltaX / 2);
    int y = y0;
    int y_step;
    if (y0 < y1)
        y_step = 1;
    else
        y_step = -1;

    for (int x = x0; x <= x1; x++)
    {
        if (is_steep){
            plot_pixel(y, x, line_color);

        }
        else{
            plot_pixel(x, y, line_color);

        }
        error = error + deltaY;

        if (error > 0){
            y = y + y_step;
            error = error - deltaX;
        }
    }
}


void plot_box(int startX, int startY, short int boxColour) {
    for (int i = 0; i < boxSize; i++){
        for(int j =0; j < boxSize; j++){
            plot_pixel(startX, startY, boxColour);
            startX = startX + 1;
        }
        startY = startY + 1;
    }
}

void wait_for_vsync()
{
    volatile int * pixel_ctrl_ptr = (int *) 0xff203020; // base address
    int status;
    *pixel_ctrl_ptr = 1; // start the synchronization process
    // write 1 into front buffer address register
    status = *(pixel_ctrl_ptr + 3); // read the status register
    while ((status & 0x01) != 0) // polling loop waiting for S bit to go to 0
    {
        status = *(pixel_ctrl_ptr + 3);
    } // loop/function exits when status bit goes to 0
}

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

void draw_box(int xCoord[8], int yCoord[8], int line_colour[10]){
    for(int i = 0; i< 8; i++){
    plot_pixel(xCoord[i], yCoord[i], line_colour[i]);
    plot_pixel(xCoord[i] + 1, yCoord[i], line_colour[i]);
    plot_pixel(xCoord[i], yCoord[i] + 1, line_colour[i]);
    plot_pixel(xCoord[i] + 1, yCoord[i] + 1, line_colour[i]);
}
}
int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    int xCoord[8];
    int yCoord[8];
    int xStep[8];
    int yStep[8];
    short int line_colour[10];
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    for (int i = 0; i < 8; i++){
        xCoord[i] = rand() % 318;
        yCoord[i] = rand() % 238;
        xStep[i] = ((rand() % 2) * 2) - 1;
        yStep[i] = ((rand() % 2) * 2) - 1;

        line_colour[i] = line_colours[rand() % 10];
    }

    while (1)
    {
        clear_screen();

        draw_box(xCoord, yCoord, line_colour);
    
        for(int i = 0; i< boxSize - 1; i++){
            draw_line(xCoord[i], yCoord[i], xCoord[i+1], yCoord[i+1], line_colour[i]);
        }

        for(int i = 0; i < boxSize; i++){
            xCoord[i] = xCoord[i] + xStep[i];
            yCoord[i] = yCoord[i] + yStep[i];

            if(yCoord[i] == 239){
                yCoord[i] = 237;
                yStep[i] = -1 * yStep[i];
                line_colour[i] = line_colours[rand() % 10];

            }
            else if(yCoord[i] == -1){
                yCoord[i] = 0;
                yStep[i] = -1 * yStep[i];
                line_colour[i] = line_colours[rand() % 10];
            }
            if(xCoord[i] == 239){
                xCoord[i] = 237;
                xStep[i] = -1 * xStep[i];
                line_colour[i] = line_colours[rand() % 10];

            }
            else if(xCoord[i] == -1){
                xCoord[i] = 0;
                xStep[i] = -1 * xStep[i];
                line_colour[i] = line_colours[rand() % 10];
            }
        }

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}