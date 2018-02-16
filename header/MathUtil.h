#ifndef MATH_UTIL_H
#define MATH_UTIL_H


#include <cmath>
#include <type_traits>


template <typename t = double> 
struct Trigo {
	static_assert(std::is_floating_point<t>::value, "Invalid template argument for Trigo : t must be a floating point type.");
	
	template <int num = 1, int den = 1> 
	static t pi() {
		static_assert(den != 0, "Invalid template argument for Trigo::pi : den must be different of zero.");
		return t(3.1415926535897932384626433832795 * t(num) / t(den));
	}
	template <int num = 1, int den = 1>
	static t piInv() {
		static_assert(den != 0, "Invalid template argument for Trigo::piInv : den must be different of zero.");
		return t(1.0 / pi<>() * t(num) / t(den));
	}

	static t deg2rad(t deg) { return deg * deg2radRatio; }
	static t rad2deg(t rad) { return rad * rad2degRatio; }

private:
	static const t deg2radRatio;
	static const t rad2degRatio;
};

template <typename t> const t Trigo<t>::deg2radRatio{ Trigo<t>::pi() / t(180.0) };
template <typename t> const t Trigo<t>::rad2degRatio{ Trigo<t>::piInv() * t(180.0) };




class MathUtil
{
public:
	MathUtil() = delete;
	~MathUtil() = delete;
	MathUtil(MathUtil const & m) = delete;
	MathUtil& operator=(MathUtil const & m) = delete;
	MathUtil(MathUtil const && m) = delete;
	MathUtil& operator=(MathUtil const && m) = delete;

};



#endif // MATH_UTIL_H
