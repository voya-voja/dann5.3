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

/**** Factory Creator's of quantum invert operator  ****/
DefaultCreator<string, QcellOp, Qinvert> gInvertOperatorCreator_m(Qinvert::cMark());
DefaultCreator<string, QcellOp, Qinvert> gInvertOperatorCreator(Qinvert::cName());

/**** Factory Creator's of quantum equal operator  ****/
DefaultCreator<string, QcellOp, Qeq> gEqualOperatorCreator_m(Qeq::cMark());
DefaultCreator<string, QcellOp, Qeq> gEqualOperatorCreator(Qeq::cName());

/**** Factory Creator's of quantum not equal operator  ****/
DefaultCreator<string, QcellOp, Qneq> gNeOperatorCreator_m(Qneq::cMark());
DefaultCreator<string, QcellOp, Qneq> gNeOperatorCreator(Qneq::cName());

/**** Factory Creator's of quantum less than operator  ****/
DefaultCreator<string, QcellOp, Qlt> gLtOperatorCreator_m(Qlt::cMark());
DefaultCreator<string, QcellOp, Qlt> gLtOperatorCreator(Qlt::cName());

/**** Factory Creator's of quantum less than or equal operator  ****/
DefaultCreator<string, QcellOp, Qle> gLeOperatorCreator_m(Qle::cMark());
DefaultCreator<string, QcellOp, Qle> gLeOperatorCreator(Qle::cName());

/**** Factory Creator's of quantum greater than operator  ****/
DefaultCreator<string, QcellOp, Qgt> gGtOperatorCreator_m(Qgt::cMark());
DefaultCreator<string, QcellOp, Qgt> gGtOperatorCreator(Qgt::cName());

/**** Factory Creator's of quantum greater than or equal operator  ****/
DefaultCreator<string, QcellOp, Qge> gGeOperatorCreator_m(Qge::cMark());
DefaultCreator<string, QcellOp, Qge> gGeOperatorCreator(Qge::cName());

/**** Factory Creator's of quantum and binary operation ****/
DefaultCreator<string, QcellOp, Qand> gAndOpCreator_m(Qand::cMark());
DefaultCreator<string, QcellOp, Qand> gAndOpCreator(Qand::cName());

/**** Factory Creator's of quantum nAnd binary operation ****/
DefaultCreator<string, QcellOp, Qnand> gNandOpCreator_m(Qnand::cMark());
DefaultCreator<string, QcellOp, Qnand> gNandOpCreator(Qnand::cName());

/**** Factory Creator's of quantum or binary operation ****/
DefaultCreator<string, QcellOp, Qor> gOrOpCreator_m(Qor::cMark());
DefaultCreator<string, QcellOp, Qor> gOrOpCreator(Qor::cName());

/**** Factory Creator's of quantum nOr binary operation ****/
DefaultCreator<string, QcellOp, Qnor> gNorOpCreator_m(Qnor::cMark());
DefaultCreator<string, QcellOp, Qnor> gNorOpCreator(Qnor::cName());

/**** Factory Creator's of quantum xOr binary operation ****/
DefaultCreator<string, QcellOp, Qxor> gXorOpCreator_m(Qxor::cMark());
DefaultCreator<string, QcellOp, Qxor> gXorOpCreator(Qxor::cName());

/**** Factory Creator's of quantum nXor binary operation ****/
DefaultCreator<string, QcellOp, Qnxor> gNxorOpCreator_m(Qnxor::cMark());
DefaultCreator<string, QcellOp, Qnxor> gNxorOpCreator(Qnxor::cName());

/**** Factory Creator's of quantum half-adder addition ****/
DefaultCreator<string, QcellOp, QhalfAdder> gHalfAdderOpCreator_m(QhalfAdder::cMark());
DefaultCreator<string, QcellOp, QhalfAdder> gHalfAdderOpCreator(QhalfAdder::cName());

/**** Factory Creator's of quantum adder addition ****/
DefaultCreator<string, QcellOp, Qadder> gAdderOpCreator_m(Qadder::cMark());
DefaultCreator<string, QcellOp, Qadder> gAdderOpCreator(Qadder::cName());

/*** Quantum-nary Operations Factory ***/
#ifdef _WINDOWS
Factory<string, QnaryOp> Factory<string, QnaryOp>::gFactory;
#else
template class dann5::Factory<string, QnaryOp>;
template class dann5::Factory<string, QnaryOp>::Creator<string, QnaryOp>;
#endif

/**** Factory Creator's of quantum invert nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryInvert> gNaryInvertOperatorCreator_m(QnaryInvert::cMark());
DefaultCreator<string, QnaryOp, QnaryInvert> gNaryInvertOperatorCreator(QnaryInvert::cName());

/**** Factory Creator's of quantum equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryEq> gNaryEqualOperatorCreator_m(QnaryEq::cMark());
DefaultCreator<string, QnaryOp, QnaryEq> gNaryEqualOperatorCreator(QnaryEq::cName());

/**** Factory Creator's of quantum not equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryNeq> gNaryNeOperatorCreator_m(QnaryNeq::cMark());
DefaultCreator<string, QnaryOp, QnaryNeq> gNaryNeOperatorCreator(QnaryNeq::cName());

/**** Factory Creator's of quantum less than nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryLt> gNaryLtOperatorCreator_m(QnaryLt::cMark());
DefaultCreator<string, QnaryOp, QnaryLt> gNaryLtOperatorCreator(QnaryLt::cName());

/**** Factory Creator's of quantum less than or equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryLe> gNaryLeOperatorCreator_m(QnaryLe::cMark());
DefaultCreator<string, QnaryOp, QnaryLe> gNaryLeOperatorCreator(QnaryLe::cName());

/**** Factory Creator's of quantum greater than nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryGt> gGtNaryOperatorCreator_m(QnaryGt::cMark());
DefaultCreator<string, QnaryOp, QnaryGt> gGtNaryOperatorCreator(QnaryGt::cName());

/**** Factory Creator's of quantum greater than or equal nary-operator  ****/
DefaultCreator<string, QnaryOp, QnaryGe> gGeNaryOperatorCreator_m(QnaryGe::cMark());
DefaultCreator<string, QnaryOp, QnaryGe> gGeNaryOperatorCreator(QnaryGe::cName());

/**** Factory Creator's of quantum and binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qand> > gAndNaryOpCreator_m(QnaryBiOp<Qand>::cMark());
DefaultCreator<string, QnaryOp, QnaryBiOp<Qand> > gAndNaryOpCreator(QnaryBiOp<Qand>::cName());

/**** Factory Creator's of quantum nAnd binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnand> > gNandNaryOpCreator_m(QnaryBiOp<Qnand>::cMark());
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnand> > gNandNaryOpCreator(QnaryBiOp<Qnand>::cName());

/**** Factory Creator's of quantum or binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qor> > gOrNaryOpCreator_m(QnaryBiOp<Qor>::cMark());
DefaultCreator<string, QnaryOp, QnaryBiOp<Qor> > gOrNaryOpCreator(QnaryBiOp<Qor>::cName());

/**** Factory Creator's of quantum nOr binary nary-operation ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnor>> gNorNaryOpCreator_m(QnaryBiOp<Qnor>::cMark());
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnor>> gNorNaryOpCreator(QnaryBiOp<Qnor>::cName());

/**** Factory Creator's of quantum xOr binary nary-addition ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qxor>> gXorNaryOpCreator_m(QnaryBiOp<Qxor>::cMark());
DefaultCreator<string, QnaryOp, QnaryBiOp<Qxor>> gXorNaryOpCreator(QnaryBiOp<Qxor>::cName());

/**** Factory Creator's of quantum nXor binary nary-addition ****/
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnxor>> gNxorNaryOpCreator_m(QnaryBiOp<Qnxor>::cMark());
DefaultCreator<string, QnaryOp, QnaryBiOp<Qnxor>> gNxorNaryOpCreator(QnaryBiOp<Qnxor>::cName());

/**** Factory Creator's of Addition ****/
DefaultCreator<string, QnaryOp, Qadd> gAdditionCreator_m(Qadd::cMark());
DefaultCreator<string, QnaryOp, Qadd> gAdditionCreator(Qadd::cName());

/**** Factory Creator's of Subtraction ****/
DefaultCreator<string, QnaryOp, Qsubtract> gSubtractionCreator_m(Qsubtract::cMark());
DefaultCreator<string, QnaryOp, Qsubtract> gSubtractionCreator(Qsubtract::cName());

/**** Factory Creator's of Multiplication ****/
DefaultCreator<string, QnaryOp, Qmultiply> gMultiplicationCreator_m(Qmultiply::cMark());
DefaultCreator<string, QnaryOp, Qmultiply> gMultiplicationCreator(Qmultiply::cName());

/**** Factory Creator's of Multiplication ****/
DefaultCreator<string, QnaryOp, Qdivide> gDivisionCreator_m(Qdivide::cMark());
DefaultCreator<string, QnaryOp, Qdivide> gDivisionCreator(Qdivide::cName());
