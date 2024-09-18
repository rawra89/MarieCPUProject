//
// Program: MARIE Computer Term Project
// Programmer: Raheem F.
// Purpose: Simulates a MARIE computer if one were to exist.
// main.cpp

#include <iostream>
#include <string>
#include "MarieComputer.h"

int main()
{
	std::string memory[28];

	memory[0] = "0001000000010001";   // Load Addr 
	memory[1] = "0010000000010010";   // Store Next 
	memory[2] = "0001000000010011";   // Load Num 
	memory[3] = "0100000000010110";   // Subt One 
	memory[4] = "0010000000010101";   // Store Ctr 
	memory[5] = "0001000000010100";   // LOOP:  Load Sum
	memory[6] = "1011000000010010";   // AddI Next
	memory[7] = "0010000000010100";   // Store Sum
	memory[8] = "0001000000010010";   // Load Next
	memory[9] = "0011000000010110";   // Add One
	memory[10] = "0010000000010010";  // Store Next
	memory[11] = "0001000000010101";  // Load Ctr
	memory[12] = "0100000000010110";  // Subt One
	memory[13] = "0010000000010101";  // Store  Ctr
	memory[14] = "1000000000000000";  // Skipcond 
	memory[15] = "1001000000000101";  // Jump
	memory[16] = "0111000000000000";  // Halt
	memory[17] = "0000000000010111";  // Addr
	memory[18] = "0000000000000000";  // Next
	memory[19] = "0000000000000101";  // Num
	memory[20] = "0000000000000000";  // Sum
	memory[21] = "0000000000000000";  // Ctr
	memory[22] = "0000000000000001";  // One
	memory[23] = "0000000000001010";  // Dec 10
	memory[24] = "0000000000001111";  // Dec 15
	memory[25] = "0000000000010100";  // Dec 20
	memory[26] = "0000000000011001";  // Dec 25
	memory[27] = "0000000000011110";  // Dec 30

	// Load memory into the CPU.
	MarieComputer* mCPU = new MarieComputer( memory, 28 );
	if ( !mCPU )
		return 1;

	mCPU->DisplayMemory(40);
	mCPU->StartProgramLoadedInMemory();
	mCPU->DisplayMemory(40);
	mCPU->DisplayMemoryValue( 20 );

	// Free everything in the computer.
	delete mCPU;
	system("pause");
	return 0;

}



