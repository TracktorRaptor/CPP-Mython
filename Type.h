#pragma once

#include <string>

class Type
{
public:
	Type();
	~Type();

	bool getIsTemp() const;
	void setIsTemp(bool newValue);

	virtual bool isPrintable() const = 0;
	virtual std::string toString() const = 0;

private:
	bool _isTemp;
};
