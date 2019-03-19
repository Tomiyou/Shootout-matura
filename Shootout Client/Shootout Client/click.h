#ifndef CLICK_H
#define CLICK_H

struct Click {
private:
	Sint32 _x;
	Sint32 _y;
	Uint8 _btnID;
	bool isDefault;

public:
	Click() :
		_x(0),
		_y(0),
		_btnID(-1),
		isDefault(true)
	{}
	Click(Sint32 x, Sint32 y, Uint8 id) :
		_x(x),
		_y(y),
		_btnID(id),
		isDefault(false)
	{}

	Sint32 getX() const {
		return _x;
	}
	void setX(int x) {
		this->_x = x;
	}
	Sint32 getY() const {
		return _y;
	}
	void setY(int y) {
		this->_y = y;
	}
	Uint8 getID() const {
		return _btnID;
	}
	bool getDefault() const {
		return isDefault;
	}
};

#endif
