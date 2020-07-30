#include "hash.h"
#include <iostream>

using namespace std;

unsigned int calculateHash(string mystring){

	string password = mystring;

	//translate the string to an unsigned long long
	//do not use pow()
	unsigned long long first_step = 0;

	for(unsigned int i = 0; i < password.size(); i++){
		first_step = (int)password[i] + (128*first_step);
	}

	//convert the number to base 65521
	//Mod your number by 65521 to get the least significant digit. 
	//Divide the original number by 65521, and mod the result by 65521 to 
	//get the next digit. Repeat this until you are finished. 
	//This will produce no more than 4 digits w1 w2 w3 w4, where w1 is 
	//the most significant digit. Store these values in an unsigned int array. 
	//Place zeros in the leading positions if the password was short enough to 
	//only require 3 or less digits.
	unsigned long long second_step = first_step;

	unsigned int w4 = second_step % 65521;
	second_step = second_step/65521;

	unsigned int w3 = second_step % 65521;
	second_step = second_step/65521;

	unsigned int w2 = second_step % 65521;
	second_step = second_step/65521;

	unsigned int w1 = second_step % 65521;

	//We will now encrypt the password to one unsigned int. Use the 
	//following formula to produce the encrypted result.
	//(45912 * w1 + 35511 * w2 + 65169 * w3 + 4625 * w4) % 65521

	unsigned int third_step = (45912 * w1 + 35511 * w2 + 65169 * w3 + 4625 * w4) % 65521;
	return third_step;
}