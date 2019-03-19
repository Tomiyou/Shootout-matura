#ifndef MOVEMENT_H
#define MOVEMENT_H

class Movement {
public:
	Movement();
	Movement(double speed);
	~Movement();

	void updateSpeed(double speed);
	void move(float x, float y);

	double getdX() const;
	double getdY() const;

private:
	double _x, _y, _speed, _rootSpeed;
	float _dx, _dy;

};

#endif
