#include <Factory.h>
#include <Logger.h>

#include <Qsolver.h>
#include <Qop.h>
#include <Qadd.h>
#include <QderivedOps.h>
#include <Qmultiply.h>
#include <QnaryOps.h>

using namespace std;
using namespace dann5;


/**** A global instance of dann5 Logger ****/
unique_ptr<Logger> Logger::gpSingleton;

/**** A global instance of  active Q solver ****/
Qsolver::Sp Qsolver::gpActive = nullptr;

/*** Quantum Cell Operations Factory ***/
#ifdef _WINDOWS
Factory<string, QcellOp> Factory<string, QcellOp>::gFactory;
#else
template class dann5::Factory<string, QcellOp>;
template class dann5::Factory<string, QcellOp>::Creator<string, QcellOp>;
#endif

/**** Quantum invert operator identifiers  ****/
const string Qinvert::cMark = "~";
const string Qinvert::cName = "invert";

/**** Factory Creator's of quantum invert operator  ****/
DefaultCreator<string, QcellOp, Qinvert> gInvertOperatorCreator_m(Qinvert::cMark);
DefaultCreator<string, QcellOp, Qinvert> gInvertOperatorCreator(Qinvert::cName);

/**** Quantum equal operator identifiers  ****/
const string Qeq::cMark = "==";
const string Qeq::cName = "equal";

/**** Factory Creator's of quantum equal operator  ****/
DefaultCreator<string, QcellOp, Qeq> gEqualOperatorCreator_m(Qeq::cMark);
DefaultCreator<string, QcellOp, Qeq> gEqualOperatorCreator(Qeq::cName);

/**** Quantum not equal operator identifiers  ****/
const string Qneq::cMark = "!=";
const string Qneq::cName = "notequal";

/**** Factory Creator's of quantum not equal operator  ****/
DefaultCreator<string, QcellOp, Qneq> gNeOperatorCreator_m(Qneq::cMark);
DefaultCreator<string, QcellOp, Qneq> gNeOperatorCreator(Qneq::cName);

/**** Quantum less than operator identifiers  ****/
const string Qlt::cMark = "<";
const string Qlt::cName = "lessthan";

/**** Factory Creator's of quantum less than operator  ****/
DefaultCreator<string, QcellOp, Qlt> gLtOperatorCreator_m(Qlt::cMark);
DefaultCreator<string, QcellOp, Qlt> gLtOperatorCreator(Qlt::cName);

/**** Quantum less than or equal operator identifiers  ****/
const string Qle::cMark = "<=";
const string Qle::cName = "lessequal";

/**** Factory Creator's of quantum less than or equal operator  ****/
DefaultCreator<string, QcellOp, Qle> gLeOperatorCreator_m(Qle::cMark);
DefaultCreator<string, QcellOp, Qle> gLeOperatorCreator(Qle::cName);

/**** Quantum greater than operator identifiers  ****/
const string Qgt::cMark = ">";
const string Qgt::cName = "greaterthan";

/**** Factory Creator's of quantum greater than operator  ****/
DefaultCreator<string, QcellOp, Qgt> gGtOperatorCreator_m(Qgt::cMark);
DefaultCreator<string, QcellOp, Qgt> gGtOperatorCreator(Qgt::cName);

/**** Quantum greater than or equal operator identifiers  ****/
const string Qge::cMark = ">=";
const string Qge::cName = "greaterequal";

/**** Factory Creator's of quantum greater than or equal operator  ****/
DefaultCreator<string, QcellOp, Qge> gGeOperatorCreator_m(Qge::cMark);
DefaultCreator<string, QcellOp, Qge> gGeOperatorCreator(Qge::cName);

/**** Quantum and binary operation identifiers  ****/
const string Qand::cMark = "&";
const string Qand::cName = "and";

/**** Factory Creator's of quantum and binary operation ****/
DefaultCreator<string, QcellOp, Qand> gAndOpCreator_m(Qand::cMark);
DefaultCreator<string, QcellOp, Qand> gAndOpCreator(Qand::cName);

/**** Quantum nAnd binary operation identifiers  ****/
const string Qnand::cMark = "~&";
const string Qnand::cName = "nand";

/**** Factory Creator's of quantum nAnd binary operation ****/
DefaultCreator<string, QcellOp, Qnand> gNandOpCreator_m(Qnand::cMark);
DefaultCreator<string, QcellOp, Qnand> gNandOpCreator(Qnand::cName);

/**** Quantum or binary operation identifiers  ****/
const string Qor::cMark = "|";
const string Qor::cName = "or";

/**** Factory Creator's of quantum or binary operation ****/
DefaultCreator<string, QcellOp, Qor> gOrOpCreator_m(Qor::cMark);
DefaultCreator<string, QcellOp, Qor> gOrOpCreator(Qor::cName);

/**** Quantum nOr binary operation identifiers  ****/
const string Qnor::cMark = "~|";
const string Qnor::cName = "nor";

/**** Factory Creator's of quantum nOr binary operation ****/
DefaultCreator<string, QcellOp, Qnor> gNorOpCreator_m(Qnor::cMark);
DefaultCreator<string, QcellOp, Qnor> gNorOpCreator(Qnor::cName);

/**** Quantum xOr addition identifiers  ****/
const string Qxor::cMark = "^";
const string Qxor::cName = "xor";

/**** Factory Creator's of quantum xOr binary operation ****/
DefaultCreator<string, QcellOp, Qxor> gXorOpCreator_m(Qxor::cMark);
DefaultCreator<string, QcellOp, Qxor> gXorOpCreator(Qxor::cName);

/**** Adder Carry operation as one of the outputs****/
const string Qaddition::Carry::cMark = "#";
const string Qaddition::Carry::cName = "carry";

/**** Quantum nXor addition identifiers  ****/
const string Qnxor::cMark = "*=";
const string Qnxor::cName = "nxor";

/**** Factory Creator's of quantum nXor binary operation ****/
DefaultCreator<string, QcellOp, Qnxor> gNxorOpCreator_m(Qnxor::cMark);
DefaultCreator<string, QcellOp, Qnxor> gNxorOpCreator(Qnxor::cName);

/**** Quantum half-adder addition identifiers  ****/
const string QhalfAdder::cMark = "h+";
const string QhalfAdder::cName = "halfadder";

/**** Factory Creator's of quantum half-adder addition ****/
DefaultCreator<string, QcellOp, QhalfAdder> gHalfAdderOpCreator_m(QhalfAdder::cMark);
DefaultCreator<string, QcellOp, QhalfAdder> gHalfAdderOpCreator(QhalfAdder::cName);

/**** Quantum adder addition identifiers  ****/
const string Qadder::cMark = "+";
const string Qadder::cName = "adder";

/**** Factory Creator's of quantum adder addition ****/
DefaultCreator<string, QcellOp, Qadder> gAdderOpCreator_m(Qadder::cMark);
DefaultCreator<string, QcellOp, Qadder> gAdderOpCreator(Qadder::cName);

/*** Quantum-nary Operations Factory ***/
#ifdef _WINDOWS
Factory<string, QnaryOp> Factory<string, QnaryOp>::gFactory;
#else
template class dann5::Factory<string, QnaryOp>;
template class dann5::Factory<string, QnaryOp>::Creator<string, QnaryOp>;
#endif

/**** Quantum invert nary-operator identifiers  ****/
const string QnaryInvert::cMark = Qinvert::cMark;
const string QnaryInvert::cName = Qinvert::cName;

/**** Factory Creator's of quantum invert nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryInvert> gNaryInvertOperatorCreator_m(Qinvert::cMark);
DefaultCreator<string, QnaryOp, QnaryInvert> gNaryInvertOperatorCreator(Qinvert::cName);

/**** Quantum equal nary-operator identifiers  ****/
const string QnaryEq::cMark = Qeq::cMark;
const string QnaryEq::cName = Qeq::cName;

/**** Factory Creator's of quantum equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryEq> gNaryEqualOperatorCreator_m(QnaryEq::cMark);
DefaultCreator<string, QnaryOp, QnaryEq> gNaryEqualOperatorCreator(QnaryEq::cName);

/**** Quantum not equal nary-operator identifiers  ****/
const string QnaryNeq::cMark = Qneq::cMark;
const string QnaryNeq::cName = Qneq::cName;

/**** Factory Creator's of quantum not equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryNeq> gNaryNeOperatorCreator_m(QnaryNeq::cMark);
DefaultCreator<string, QnaryOp, QnaryNeq> gNaryNeOperatorCreator(QnaryNeq::cName);

/**** Quantum less than nary-operator identifiers  ****/
const string QnaryLt::cMark = Qlt::cMark;
const string QnaryLt::cName = Qlt::cName;

/**** Factory Creator's of quantum less than nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryLt> gNaryLtOperatorCreator_m(QnaryLt::cMark);
DefaultCreator<string, QnaryOp, QnaryLt> gNaryLtOperatorCreator(QnaryLt::cName);

/**** Quantum less than or equal nary-operator identifiers  ****/
const string QnaryLe::cMark = Qle::cMark;
const string QnaryLe::cName = Qle::cName;

/**** Factory Creator's of quantum less than or equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryLe> gNaryLeOperatorCreator_m(QnaryLe::cMark);
DefaultCreator<string, QnaryOp, QnaryLe> gNaryLeOperatorCreator(QnaryLe::cName);

/**** Quantum greater than nary-operator identifiers  ****/
const string QnaryGt::cMark = Qgt::cMark;
const string QnaryGt::cName = Qgt::cName;

/**** Factory Creator's of quantum greater than nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryGt> gGtNaryOperatorCreator_m(QnaryGt::cMark);
DefaultCreator<string, QnaryOp, QnaryGt> gGtNaryOperatorCreator(QnaryGt::cName);

/**** Quantum greater than or equal nary-operator identifiers  ****/
const string QnaryGe::cMark = Qge::cMark;
const string QnaryGe::cName = Qge::cName;

/**** Factory Creator's of quantum greater than or equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryGe> gGeNaryOperatorCreator_m(QnaryGe::cMark);
DefaultCreator<string, QnaryOp, QnaryGe> gGeNaryOperatorCreator(QnaryGe::cName);

/**** Quantum and binary nary-operation identifiers  ****/
template<> const string QnaryBiOp<Qand>::cMark = Qand::cMark;
template<> const string QnaryBiOp<Qand>::cName = Qand::cName;

/**** Factory Creator's of quantum and binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qand> > gAndNaryOpCreator_m(QnaryBiOp<Qand>::cMark);
DefaultCreator<string, QnaryOp, QnaryBiOp<Qand> > gAndNaryOpCreator(QnaryBiOp<Qand>::cName);

/**** Quantum nAnd binary nary-operation identifiers  ****/
template<> const string QnaryBiOp<Qnand>::cMark = Qnand::cMark;
template<> const string QnaryBiOp<Qnand>::cName = Qnand::cName;

/**** Factory Creator's of quantum nAnd binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnand> > gNandNaryOpCreator_m(QnaryBiOp<Qnand>::cMark);
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnand> > gNandNaryOpCreator(QnaryBiOp<Qnand>::cName);

/**** Quantum or binary nary-operation identifiers  ****/
template<> const string QnaryBiOp<Qor>::cMark = Qor::cMark;
template<> const string QnaryBiOp<Qor>::cName = Qor::cName;

/**** Factory Creator's of quantum or binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qor> > gOrNaryOpCreator_m(QnaryBiOp<Qor>::cMark);
DefaultCreator<string, QnaryOp, QnaryBiOp<Qor> > gOrNaryOpCreator(QnaryBiOp<Qor>::cName);

/**** Quantum nOr binary nary-operation identifiers  ****/
template<> const string QnaryBiOp<Qnor>::cMark = Qnor::cMark;
template<> const string QnaryBiOp<Qnor>::cName = Qnor::cName;

/**** Factory Creator's of quantum nOr binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnor>> gNorNaryOpCreator_m(QnaryBiOp<Qnor>::cMark);
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnor>> gNorNaryOpCreator(QnaryBiOp<Qnor>::cName);

/**** Quantum xOr nary-addition identifiers  ****/
template<> const string QnaryBiOp<Qxor>::cMark = Qxor::cMark;
template<> const string QnaryBiOp<Qxor>::cName = Qxor::cName;

/**** Factory Creator's of quantum xOr binary nary-addition ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qxor>> gXorNaryOpCreator_m(QnaryBiOp<Qxor>::cMark);
DefaultCreator<string, QnaryOp, QnaryBiOp<Qxor>> gXorNaryOpCreator(QnaryBiOp<Qxor>::cName);

/**** Quantum nXor nary-addition identifiers  ****/
template<> const string QnaryBiOp<Qnxor>::cMark = Qnxor::cMark;
template<> const string QnaryBiOp<Qnxor>::cName = Qnxor::cName;

/**** Factory Creator's of quantum nXor binary nary-addition ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnxor>> gNxorNaryOpCreator_m(QnaryBiOp<Qnxor>::cMark);
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnxor>> gNxorNaryOpCreator(QnaryBiOp<Qnxor>::cName);

/**** Addition binary operation ****/

const string Qadd::cMark = "+";
const string Qadd::cName = "add";

/**** Factory Creator's of Addition ****/
DefaultCreator<string, QnaryOp, Qadd> gAdditionCreator_m(Qadd::cMark);
DefaultCreator<string, QnaryOp, Qadd> gAdditionCreator(Qadd::cName);

/**** Subtraction binary operation ****/

const string Qsubtract::cMark = "-";
const string Qsubtract::cName = "subtract";

/**** Factory Creator's of Subtraction ****/
DefaultCreator<string, QnaryOp, Qsubtract> gSubtractionCreator_m(Qsubtract::cMark);
DefaultCreator<string, QnaryOp, Qsubtract> gSubtractionCreator(Qsubtract::cName);

/**** Multiplication binary operation ****/

const string Qmultiply::cMark = "*";
const string Qmultiply::cName = "multiply";

/**** Factory Creator's of Multiplication ****/
DefaultCreator<string, QnaryOp, Qmultiply> gMultiplicationCreator_m(Qmultiply::cMark);
DefaultCreator<string, QnaryOp, Qmultiply> gMultiplicationCreator(Qmultiply::cName);

/**** Division binary operation ****/

const string Qdivide::cMark = "/";
const string Qdivide::cName = "divide";

/**** Factory Creator's of Multiplication ****/
DefaultCreator<string, QnaryOp, Qdivide> gDivisionCreator_m(Qdivide::cMark);
DefaultCreator<string, QnaryOp, Qdivide> gDivisionCreator(Qdivide::cName);
