#include <iostream>
#include "board.h"
#include "movegen.h"

using namespace std;

int main()
{
	Position pos[1];
	MoveGenerator movegen[1];
	
	pos->parseFen(startPosition);
	pos->print();

	return 0;
}
