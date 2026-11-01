#pragma once
#include "CoreMinimal.h"
// HAND-WRITTEN STAND-IN (nightbuild02) for the console-variable declaration the movement trace uses. NOT the engine.
template <typename T> class TAutoConsoleVariable
{
public:
	TAutoConsoleVariable(const TCHAR* /*Name*/, const T& DefaultValue, const TCHAR* /*Help*/) : Value(DefaultValue) {}
	T GetValueOnGameThread() const { return Value; }
private:
	T Value;
};
