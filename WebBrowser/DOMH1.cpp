#include "DOMH1.h"

DOMH1::DOMH1()
{
	body = "";
	fontSize = (Pixels)30;
	width.get().emplace<Percetage>(100);
	height.get().emplace<Percetage>(100);
	x = (Pixels)0;
	y = (Pixels)0;
}
