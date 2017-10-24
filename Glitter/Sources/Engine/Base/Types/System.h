#pragma once

class System {
public:
	System();
	virtual ~System() = 0;

private:
	// Disable copying
	System& operator=(const System&) = delete;
	System(const System&) = delete;
};