#include <stdio.h>

void toBinary(int n)
{
	int c, k;

	for (c = 31; c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
		  printf("1");
		else
		  printf("0");
	}

	printf("\n");
}

//Shift Left
void bit_man()
{
	int start = 6;
	printf("Start var is %d.\n", start);
	//Multiply by 2
	//101 -> 1010
	printf("var shifted left by 1: %d.\n", start << 1);
	//Divide by 2
	//101 -> 010
	printf("var shifted right by 1: %d.\n", start >> 1);
	//Divide by 2 twice
	printf("var shifted right by 2: %d.\n\n", start >> 2);

	printf("var in binary is ");
	toBinary(start);	

	//Set a bit
	int x = 0;
	printf("Setting the %dth bit\n", x);
	//The 1 << x shift the 1 over x positions: 1000
	//Creates a mask
	//'|' is the bitwise OR operator
	toBinary(start |= 1 << x);
	
	printf("Clearing the %dth bit\n", x);
	//The 1 << x shift the 1 over x positions: 1000
	//Creates a mask
	//'&' is the bitwise AND operator
	toBinary(start &= ~(1 << x));
	
	printf("Toggling the %dth bit\n", x);
	//The 1 << x shift the 1 over x positions: 1000
	//Creates a mask
	//'^' is the bitwise XOR operator
	toBinary(start ^= 1 << x);

	printf("Checking the %dth bit\n", x);
	//The 1 << x shift the 1 over x positions: 1000
	//Creates a mask
	//'&' is the bitwise AND operator
	printf("Returns a : %d\n\n", (start >> x) & 1);	
	
}

int main()
{
	bit_man();
}
