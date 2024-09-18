//
// Program: MARIE Computer Term Project
// Programmer: Raheem F.
// Purpose: Simulates a MARIE computer if one were to exist.
// MarieComputer.h

#pragma once
#include <iostream>
#include <string> 
#include <bitset> // For representing a value as binary.

class MarieComputer
{
public:
	// Default constructor. Initialize everything.
	MarieComputer()
	{
		// Initalize registers.
		Input = 0;
		Output = 0;
		AC = 0;
		MBR = 0;
		MAR = 0;
		IR = 0;
		PC = 0;
		Halt = false;
		Inputing = false;
		// Set everything in the CPU memory to 0.
		// This is different from ClearMemory as it defines them as 0s,
		// rather than null strings.
		for ( int i = 0; i < 4096; i++ )
			memory[i] = "0000000000000000";
	}

	// Constructor with memory and its size loaded.
	// MAKE SURE SIZE IS CORRECT OR IT WILL CRASH!
	MarieComputer( std::string mem[], unsigned short size ) : MarieComputer()
	{
		LoadMemory( mem, size );
	}

	// Load memory into the computer. Specify memory and its size.
	// Memory will be overwritten!
	// MAKE SURE SIZE IS CORRECT OR IT WILL CRASH!
	void LoadMemory( std::string mem[], unsigned short size )
	{
		// Memory can't be higher than 2 KB
		if ( size > 4096 )
			size = 4096;

		// Start at 0
		int linestart = 0;

		// Allocate memory into the computer.
		while ( linestart < size )
		{
			memory[linestart] = mem[linestart];
			linestart++;
		}
	}

	// Clear on destruction.
	~MarieComputer() 
	{
		ClearMemory();
	}

	// Clear the computer's memory.
	void ClearMemory()
	{
		// Make every value null.
		for ( int i = 0; i < 4096; i++ )
			memory[i] = '\0';

		std::cout << "Memory cleared." << std::endl;
	}

	// Start reading what's in memory!
	void StartProgramLoadedInMemory()
	{
		// Keep going until the program halts.
		// The Program Counter is a 12 bit value, break if it's higher than 4096
		while ( !Halt && PC < 4096 )
		{
			IR = BinToDecFromStr( memory[PC] );
			switch ( DoCommand( IR ) )
			{
				default:			// Increment the Program Counter.
					PC++;
					break;
				case OP_HALT:		// Program was stopped, break
				case OP_JUMP:		// Overrides the Program Counter already, don't increment.
				case OP_JUMPI:
					break;
			}
		}
	}


	// Print all of the memory in the computer with line numbers.
	void DisplayMemory()
	{
		std::cout << "\tMARIE MEMORY" << std::endl;

		for ( int i = 0; i < 4096; i++ )
		{
			std::cout << (i+1) << "\t" << memory[i] << std::endl;
		}
	}

	// Print memory up to a point, with the addr variable.
	void DisplayMemory( unsigned short addr )
	{
		std::cout << "\tMARIE MEMORY" << std::endl;

		for ( int i = 0; i < addr; i++ )
		{
			std::cout << ( i+1 ) << "\t" << memory[ i ] << std::endl;
		}
		std::cout << std::endl;
	}

	// Print a specific value in memory.
	void DisplayMemoryValue( unsigned short addr, bool bBinary = false )
	{
		if ( bBinary )
			std::cout << memory[addr] << std::endl;
		else
			std::cout << BinToDecFromStr( memory[addr] ) << std::endl;
	}

private:
	
	// Global definitions: memory[MAR] means go to the line number in the program in memory.
	// Since the memory table is a string, we have to convert it to a decimal number, using
	// BinToDecFromStr.

	//
	// CPU Instructions
	//

	// Execute an opcode based on the code.
	unsigned short DoCommand( unsigned short IRVal )
	{
		// Get the opcode and the address.
		OpcodeID opcode = (OpcodeID)GetOpcode( IRVal );
		unsigned short num = GetAddress( IRVal );

		// 0-14 is used
		switch ( opcode )
		{
			default: printf( "Illegal operation. Program was halted." ); HALT(); break;
			case OP_JNS:		JnS( num ); break;
			case OP_LOAD:		LOAD( num ); break;
			case OP_STORE:		STORE( num ); break;
			case OP_ADD:		ADD( num ); break;
			case OP_SUBT:		SUBT( num ); break;
			case OP_INPUT:		Inputing = true; INPUT(); Inputing = false; break;
			case OP_OUTPUT:		OUTPUT(); break;
			case OP_HALT:		HALT(); break;
			case OP_SKIPCOND:	SKIPCOND( num ); break;	
			case OP_JUMP:		JUMP( num ); break;
			case OP_CLEAR:		CLEAR(); break;
			case OP_ADDI:		ADDI( num ); break;
			case OP_JUMPI:		JUMPI( num ); break;
			case OP_LOADI:		LOADI( num ); break;
			case OP_STOREI:		STOREI( num ); break;
		}

		return opcode;
	}

	//
	// Opcodes
	//

	// Jump and Save: Process a value in memory.
	void JnS( unsigned short addr ) // 0000
	{
		// Set the Memory Buffer Register to the Program Counter's number.
		MBR = PC;
		// Put the address in the Memory Address Register.
		MAR = addr;
		// Put MBR in memory.
		memory[MAR] = std::bitset<16>(MBR).to_string();
		// Set MBR to the value.
		MBR = addr;
		// Set Accumulator to 1.
		AC = 1;
		// Add Accumulator to the MBR value and store the result.
		AC = AC + MBR;
		// Set the Program Counter to the Accumulator's value.
		PC = AC;
	}

	// Load a value into the Accumulator.
	void LOAD( unsigned short addr ) // 0001
	{
		// Load the value into the Memory Address Register.
		MAR = addr;
		// Load whatever's in memory into the Memory Buffer Register.
		MBR = BinToDecFromStr( memory[MAR] );
		// Put that value into the Accumulator.
		AC = MBR;
	}

	// Store values in memory.
	void STORE( unsigned short val ) // 0010
	{
		// Modifying memory now.
		// Put the value in the Memory Address Register.
		MAR = val;
		// Put whatever's in the Accumlator into the Memory Buffer Register.
		MBR = AC;
		// Now modify the value in memory, using the value from the Memory Buffer Register.
		memory[MAR] = std::bitset<16>(MBR).to_string();
	}

	// Add numbers. The result will be in the Accumulator.
	void ADD( unsigned short val ) // 0011
	{
		// Put the value in the Memory Buffer Register.
		MAR = val;
		// Get the value from memory.
		MBR = BinToDecFromStr( memory[MAR] );
		// Add the value in memory to the Accumulator.
		AC = AC + MBR;
	}

	// Subtract numbers. The result will be in the Accumlator.
	void SUBT( unsigned short val ) // 0100
	{
		// Put the value in the Memory Buffer Register.
		MAR = val;
		// Get the value from memory.
		MBR = BinToDecFromStr( memory[MAR] );
		// Subtract the value in memory to the Accumulator.
		AC = AC - MBR;
	}

	// Input value into the Accumulator.
	void INPUT() // 0101
	{
		// ASCII value, from 0-255
		AC = Input = (unsigned char)getchar();
	}

	// Output value from the Accumulator.
	void OUTPUT() // 0110
	{
		Output = ( unsigned char )AC;
		std::cout << Output << std::endl;
	}

	// Stop program execution.
	void HALT() // 0111
	{
		Halt = true;
	}

	// Skip a line if special conditions are met.
	void SKIPCOND( unsigned short num ) // 1000
	{
		// We can just move the bits up to get the numbers we need to compare.
		int MSBAddr = num >> 10;
		
		// if IR[11-10]
		switch ( MSBAddr ) 
		{
			default: 
				break;
			case 0: // 00
				if ( AC < 0 )
					PC++;
				break;
			case 1: // 01
				if ( AC == 0 )
					PC++;
				break;
			case 2: // 10
				if ( AC > 0 )
					PC++;
				break;
		}
	}

	// Jump to a place in memory.
	void JUMP( unsigned short addr ) // 1001
	{
		PC = addr;
	}

	// Clear Accumulator.
	void CLEAR() // 1010
	{
		AC = 0;
	}

	// Add a value pointing to a number using an address.
	void ADDI( unsigned short addr ) // 1011
	{
		// Put addr in Memory Address Register.
		MAR = addr;
		// Put whatever's in memory in the Memory Buffer Register.
		MBR = BinToDecFromStr( memory[MAR] );
		// Put MBR's contents in the MAR.
		MAR = MBR;
		// Put whatever's in memory into MBR.
		MBR = BinToDecFromStr( memory[MAR] );
		// Put the result in the Accumlator.
		AC = AC + MBR;
	}

	// Jump to a value using its address.
	void JUMPI( unsigned short addr ) // 1100
	{
		// Store the address in MAR.
		MAR = addr;
		// Grab the value from memory and place it in MBR.
		MBR = BinToDecFromStr( memory[MAR] );
		// Jump using the value from MBR.
		PC = MBR;
	}

	// Load a value into the Accumulator using its address.
	void LOADI( unsigned short addr ) // 1101
	{
		// Put address in MAR.
		MAR = addr;
		// Grab the value from memory and place it in MBR.
		MBR = BinToDecFromStr( memory[MAR] );
		// Put that in MAR.
		MAR = MBR;
		// Grab it again, and put it in MBR.
		MBR = BinToDecFromStr( memory[MAR] );
		// Load the value from MBR.
		AC = MBR;
	}

	// Store a value into memory by using its address.
	void STOREI( unsigned short addr ) // 1110
	{
		// Put address in MAR.
		MAR = addr;
		// Grab the value from memory and place it in MBR.
		MBR = BinToDecFromStr( memory[MAR] );
		// Put that in MAR.
		MAR = MBR;
		// Store what's in the accumlator into MBR.
		MBR = AC;
		// Place whatever's in the MBR into memory.
		memory[MAR] = std::bitset<16>( MBR ).to_string();
	}

	//
	// Helper functions
	//

	// C function to convert a string to a long value,
	// but can choose which base to convert it from, too.
	// Casted to unsigned short for convienence.
	unsigned short BinToDecFromStr( std::string &bin )
	{
		return ( unsigned short )strtol( bin.c_str(), nullptr, 2 );
	}

	// Get the opcode from a number.
	unsigned short GetOpcode( unsigned short val )
	{
		// Bit shift the value to the right to get the opcode
		// for example,
		// 0001 0000 0000 0011
		// will be bit shifted by 12 to get:
		// 0000 0000 0000 0001
		return val >> 12;
	}

	// Get the address.
	unsigned short GetAddress( unsigned short val )
	{
		// Get the 12 rightmost bits. 
		// & the value by 2^n-1, where n is the number of bits you wish to select from the right
		// n is 12.
		// Based off an example from this website
		// http://marvin.cs.uidaho.edu/Teaching/CS504/bitOps.html
		return val & 0xfff; // (2^12) - 1
	}

private:
	// Halt flag.
	bool Halt;
	// Input flag.
	bool Inputing;

	// General I/O Registers
	unsigned char Input; // Input Register (8 bits)
	unsigned char Output; // Output Register (8 bits)

	// ALU Register
	short AC;			// Accumulator (16 bits)

	// Middle Registers
	unsigned short MBR;	// Memory Buffer Register (16 bits)
	unsigned short MAR;	// Memory Address Register (12 bits)

	// Control Unit Registers
	unsigned short IR;	// Instruction Register (16 bits)
	unsigned short PC;	// Program Counter (12 bits) 
	
	std::string memory[4096];	// 0-4095 addressable memory

	// Opcode IDs
	enum OpcodeID
	{
		OP_JNS = 0,
		OP_LOAD,
		OP_STORE,
		OP_ADD,
		OP_SUBT,
		OP_INPUT,
		OP_OUTPUT,
		OP_HALT,
		OP_SKIPCOND,
		OP_JUMP,
		OP_CLEAR,
		OP_ADDI,
		OP_JUMPI,
		OP_LOADI,
		OP_STOREI
	};
};