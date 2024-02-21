#include <Qdef.h>

using namespace dann5;

ostream& dann5::operator << (std::ostream& out, const Qdef& right) {
	out << right.toString();
	return out;
};