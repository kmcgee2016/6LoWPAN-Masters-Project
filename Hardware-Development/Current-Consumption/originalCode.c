//////////////////////////////////////////////////////////////////////////////////////
////////////////// KMG: INA219 Linux Interface & Logging Code ////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*
	This is a simple C application to read and log data from the INA219 current sensor.
	Although this application could have been modified to follow OOP guidelines, this 
	part of the project was already behind schedule so this basic test code was used as
	the final product. In future development, a C++ class should be used to wrap around
	the INA219 device and its configuration/calibration settings. 
*/
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h> 
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<string.h> 
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#define MA_DIVISOR 20
#define SAMPLING_PERIOD_US 500


int main(int argc, char *argv[]){
	int file;
	int file2; //I 
	printf("Application start\n");
	//open the I2C interface
	if((file=open("/dev/i2c-1", O_RDWR))< 0){
		perror("Failed to open file\n");
		return 1;
	}
	//connect to device at address 0x40
	if(ioctl(file, I2C_SLAVE, 0x40) < 0){
		perror("Failed to connect to device\n");
		return 1;
	}
	//reset register pointer and perform initial read check
	char writeBuffer[1] = {0x00};
	char readPointer[1] = {0x04}; 
	if(write(file,writeBuffer,1) != 1){
		perror("Failed to reset register pointer\n");
		return 1;
	}
	char readBuffer[2];
	if(read(file, readBuffer,2) != 2){
		perror("Failed to read data\n");
		return 1;
	}
	//write the desired config. to the device(NOTE: "i2c-tools" will display the 16-bit register as 2 8-bit registers, in the opposite order)
	char configData[3] = {0x00,0x01,0x9f};
	if(write(file,configData,3) != 3){
		perror("Error setting the device config\n");
		return 1;
	}
	//set the calibration register(0x05)
	char calibData[3] = {0x05,0x20,0x00};
	if(write(file, calibData, 3) != 3){
		perror("Calib issue\n");
	}
	
	close(file);
	usleep(1000000);//10s delay(hopefully that is enough!)
	
	//re-open the I2C interface(an issue arose that required the file to be closed, otherwise all subsequent operations failed)
	if((file2=open("/dev/i2c-1", O_RDWR)) < 0){
		perror("Failed to open device\n");
	}
	//connect to the device @ 0x40
	if(ioctl(file2,I2C_SLAVE,0x40) < 0){
		perror("Error connecting to the device\n");
		return 1;
	}
	//reset the register pointer back to 0x00
	if(write(file2, readPointer, 1) != 1){
		perror("Failed to reset register pointer\n");
		return 1;
	}
	//variables for conversion, ect.
	char * muck;//useless variable(but needed for string parsing function)
	long total_mA = 0;
	int measurementCount;
	char output[10];
	long result;
	int i;
	float fin_result, avg_result;
	float max_result, min_result;
	int exeMinutes = 60; //1 min
	//open the output file(the one to print the data out to)
	FILE *fp;
	if(argc < 2){
		fp = fopen("current_logging.txt", "w");
	}else{
		//there is a custom filename supplied:
		fp = fopen(argv[1], "w");
	}
	if(fp == NULL){
		printf("Error opening the desired O/P file\n");
		return 1; //no point in doing anything else
	}

	//custom minutes:
	if(argc > 2){
		exeMinutes = 60 * strtol(argv[2],&muck,10);
	}
	
	
	
	///////////////////////////////////// Main Loop /////////////////////////////////////
	//print out the general format:
	fprintf(fp,"measurementCount, Avg , Min, Max, |\n");
	for(measurementCount = 0; measurementCount < exeMinutes; measurementCount++){
		//
		avg_result = 0;
		max_result = 0;
		min_result = 1000;
		for(i = 0; i < 2000; i++){
			//the lines below can be changed
			if(read(file2,readBuffer,2) == 2){
				sprintf(output,"%02x%02x", readBuffer[0],readBuffer[1]);
				result = strtol(output, &muck, 16);//convert out of hex
				//write this data to the file:
				fin_result = result;
				fin_result = fin_result/MA_DIVISOR;//stops round off(L-1)
				avg_result += fin_result;

				if(fin_result > max_result) max_result = fin_result;
				if(fin_result < min_result) min_result = fin_result;
				usleep(SAMPLING_PERIOD_US);//0.5ms delay between measurements(min)
				
			}else{
				//some issue in reading - the device may come back(issue with calib/config getting lost due to sharp transient voltage)
				usleep(10*SAMPLING_PERIOD_US);
				break;
			}
			
			
		}
		fprintf(fp, "%d, %.4f, %0.4f, %0.4f, |\n",measurementCount, avg_result/2000, min_result, max_result);
//		printf("%d, %.2f, %0.2f, %0.2f, |\n", measurementCount,avg_result/2000, min_result, max_result); // For Debugging only
	}
	////////////////////////////////////// Clean up ////////////////////////////////////
	

	close(file2);//close I2C connection
	fclose(fp);//close the logging file
	return 0;
}
