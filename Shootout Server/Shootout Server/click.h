#ifndef CLICK_H
#define CLICK_H

struct Click {
private:
	Sint32 _x;
	Sint32 _y;
	bool isDefault;

public:
	Click() :
		_x(0),
		_y(0),
		isDefault(true)
	{}
	Click(Sint32 x, Sint32 y) :
		_x(x),
		_y(y),
		isDefault(false)
	{}

	Sint32 getX() const {
		return _x;
	}
	Sint32 getY() const {
		return _y;
	}
	bool getDefault() const {
		return isDefault;
	}
};

#endif
