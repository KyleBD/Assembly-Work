
int main(void){
	
	volatile int *KEYs = 0xff200050;
	volatile int *LEDs = 0xff200000;
	volatile int *edge_cap = 0xFF20005C;
		
	*LEDs = 0x0; //reset LEDs
	*edge_cap = 1;

	while(1){
		int LED = 0x3FF;
		if(*edge_cap){
			if(*KEYs & 0x1){ //KEY 0 Pressed
				*LEDs = 0x3FF;
				//Reset edge
				*edge_cap = 1;
			}

			else if( ((*KEYs) >> 1) & 0x1){ //KEY 1 Pressed
				*LEDs = 0x0;
				*edge_cap = 1;
			}
		}
	}
	return 1;
}