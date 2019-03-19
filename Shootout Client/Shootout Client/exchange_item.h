#ifndef EXCHANGE_ITEM_H
#define EXCHANGE_ITEM_H

#include <string>

struct E_Item {
	int type, id, x, y;
	E_Item() :
		type(-1),
		id(0),
		x(0),
		y(0)
	{}
	E_Item(int type, int id, int x, int y) :
		type(type),
		id(id),
		x(x),
		y(y)
	{}

};

#endif
