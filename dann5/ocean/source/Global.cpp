#include <Factory.h>
#include <Logger.h>

#include <Qubo.h>
#include <Qop.h>
#include <Qadd.h>
#include <QderivedOps.h>
#include <Qmultiply.h>

using namespace std;
using namespace dann5;
using namespace dann5::ocean;


/**** A global instance of dann5 Logger ****/
unique_ptr<Logger> Logger::mpSingleton;

/**** A global instance of Qubo Table Factory singleton ****/
#ifdef _WINDOWS // error LNK2001: unresolved external symbol "private: static class dann5::Factory<string,QuboTable> dann5::Factory<string,QuboTable>::gFactory"
Factory<string, QuboTable> Factory<string, QuboTable>::gFactory;
#else
template class Factory<string, QuboTable>;
template class Factory<string, QuboTable>::Creator<string, QuboTable>;
#endif

/**** Equal operator Qubo Table ****/

const string EqQT::cMark = "==";
const string EqQT::cName = "equal";

/**** Factory Creator's of Equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, EqQT> gEqualQTcreator_m(EqQT::cMark);
DefaultCreator<string, QuboTable, EqQT> gEqualQTcreator(EqQT::cName);


/**** Not operator Qubo Table ****/

const string NeqQT::cMark = "!=";
const string NeqQT::cName = "notequal";

/**** Factory Creator's of Not operator Qubo Table ****/
DefaultCreator<string, QuboTable, NeqQT> gNotQTcreator_m(NeqQT::cMark);
DefaultCreator<string, QuboTable, NeqQT> gNotQTcreator(NeqQT::cName);


/**** Less-than operator Qubo Table ****/

const string LtQT::cMark = "<";
const string LtQT::cName = "lessthan";

/**** Factory Creator's of Less-than operator Qubo Table ****/
DefaultCreator<string, QuboTable, LtQT> gLessThanQTcreator_m(LtQT::cMark);
DefaultCreator<string, QuboTable, LtQT> gLessThanQTcreator(LtQT::cName);


/**** Less-equal operator Qubo Table ****/

const string LeQT::cMark = "<=";
const string LeQT::cName = "lessequal";

/**** Factory Creator's of Less-equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, LeQT> gLessEqualQTcreator_m(LeQT::cMark);
DefaultCreator<string, QuboTable, LeQT> gLessEqualQTcreator(LeQT::cName);


/**** Greater-than operator Qubo Table ****/

const string GtQT::cMark = ">";
const string GtQT::cName = "greaterthan";

/**** Factory Creator's of Greater-than operator Qubo Table ****/
DefaultCreator<string, QuboTable, GtQT> gGreaterThanQTcreator_m(GtQT::cMark);
DefaultCreator<string, QuboTable, GtQT> gGreaterThanQTcreator(GtQT::cName);


/**** Greater-equal operator Qubo Table ****/

const string GeQT::cMark = ">=";
const string GeQT::cName = "greaterequal";

/**** Factory Creator's of Greater-equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, GeQT> gGreaterEqualQTcreator_m(GeQT::cMark);
DefaultCreator<string, QuboTable, GeQT> gGreaterEqualQTcreator(GeQT::cName);


/**** And binary operation Qubo Table ****/

const string AndQT::cMark = "&";
const string AndQT::cName = "and";

/**** Factory Creator's of And binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, AndQT> gAndQTcreator_m(AndQT::cMark);
DefaultCreator<string, QuboTable, AndQT> gAndQTcreator(AndQT::cName);


/**** Nand binary operation Qubo Table with two outputs****/

const string NandQT::cMark = "!&";
const string NandQT::cName = "nand";

/**** Factory Creator's of Nand binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NandQT> gNandQTcreator_m(NandQT::cMark);
DefaultCreator<string, QuboTable, NandQT> gNandQTcreator(NandQT::cName);


/**** Or binary operation Qubo Table ****/

const string OrQT::cMark = "|";
const string OrQT::cName = "or";

/**** Factory Creator's of Or binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, OrQT> gOrQTcreator_m(OrQT::cMark);
DefaultCreator<string, QuboTable, OrQT> gOrQTcreator(OrQT::cName);


/**** Nand binary operation Qubo Table with two outputs****/

const string NorQT::cMark = "!|";
const string NorQT::cName = "nor";

/**** Factory Creator's of Nor binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NorQT> gNorQTcreator_m(NorQT::cMark);
DefaultCreator<string, QuboTable, NorQT> gNorQTcreator(NorQT::cName);


/**** Not-left-or-right binary operation Qubo Table with two outputs****/

const string NotLeftOrRightQT::cMark = "!l|r";
const string NotLeftOrRightQT::cName = "notleftorright";

/**** Factory Creator's of not-left-or-right binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator_m(NotLeftOrRightQT::cMark);
DefaultCreator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator(NotLeftOrRightQT::cName);


/**** DWave not-left-or-right binary operation Qubo Table with two outputs****/

const string DwNotLeftOrRightQT::cMark = "d!l|r";
const string DwNotLeftOrRightQT::cName = "dwnotleftorright";

/**** Factory Creator's of DWave not-left-or-right binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_m_dw(DwNotLeftOrRightQT::cMark);
DefaultCreator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_dw(DwNotLeftOrRightQT::cName);


/**** Xor binary operation Qubo Table with two outputs****/

const string XorQT::cMark = "^";
const string XorQT::cName = "xor";

/**** Factory Creator's of Xor binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, XorQT> gXorQTcreator_m(XorQT::cMark);
DefaultCreator<string, QuboTable, XorQT> gXorQTcreator(XorQT::cName);


/**** Nxor binary operation Qubo Table with two outputs****/

const string NxorQT::cMark = "*=";
const string NxorQT::cName = "nxor";

/**** Factory Creator's of Nxor binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NxorQT> gNxorQTcreator_m(NxorQT::cMark);
DefaultCreator<string, QuboTable, NxorQT> gNxorQTcreator(NxorQT::cName);


/**** Half-adder binary operation Qubo Table with two outputs****/

const string Adder05QT::cMark = "h+";
const string Adder05QT::cName = "halfadder";

/**** Factory Creator's of Half-adder binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, Adder05QT> gHalfAdderQTcreator_m(Adder05QT::cMark);
DefaultCreator<string, QuboTable, Adder05QT> gHalfAdderQTcreator(Adder05QT::cName);


/**** Adder trinary operation Qubo Table with two outputs****/

const string AdderQT::cMark = "+";
const string AdderQT::cName = "adder";

/**** Factory Creator's of Adder trinary operation Qubo Table ****/
DefaultCreator<string, QuboTable, AdderQT> gAdderQTcreator_m(AdderQT::cMark);
DefaultCreator<string, QuboTable, AdderQT> gAdderQTcreator(AdderQT::cName);

/**** Adder Carry operation as one of the outputs****/

const string AdderQT::Carry::cMark = "#";
const string AdderQT::Carry::cName = "carry";

/*** Quantum Cell Operations Factory ***/
#ifdef _WINDOWS
Factory<string, QcellOp> Factory<string, QcellOp>::gFactory;
#else
template class Factory<string, QcellOp>;
template class Factory<string, QcellOp>::Creator<string, QcellOp>;
#endif

DefaultCreator<string, QcellOp, Qeq> gEqualOperatorCreator_m(EqQT::cMark);
DefaultCreator<string, QcellOp, Qeq> gEqualOperatorCreator(EqQT::cName);

DefaultCreator<string, QcellOp, Qneq> gNotOperatorCreator_m(NeqQT::cMark);
DefaultCreator<string, QcellOp, Qneq> gNotOperatorCreator(NeqQT::cName);

DefaultCreator<string, QcellOp, Qlt> gLtOperatorCreator_m(LtQT::cMark);
DefaultCreator<string, QcellOp, Qlt> gLtOperatorCreator(LtQT::cName);

DefaultCreator<string, QcellOp, Qle> gLeOperatorCreator_m(LeQT::cMark);
DefaultCreator<string, QcellOp, Qle> gLeOperatorCreator(LeQT::cName);

DefaultCreator<string, QcellOp, Qgt> gGtOperatorCreator_m(GtQT::cMark);
DefaultCreator<string, QcellOp, Qgt> gGtOperatorCreator(GtQT::cName);

DefaultCreator<string, QcellOp, Qge> gGeOperatorCreator_m(GeQT::cMark);
DefaultCreator<string, QcellOp, Qge> gGeOperatorCreator(GeQT::cName);

DefaultCreator<string, QcellOp, Qand> gAndOpCreator_m(AndQT::cMark);
DefaultCreator<string, QcellOp, Qand> gAndOpCreator(AndQT::cName);

DefaultCreator<string, QcellOp, QnAnd> gNandOpCreator_m(NandQT::cMark);
DefaultCreator<string, QcellOp, QnAnd> gNandOpCreator(NandQT::cName);

DefaultCreator<string, QcellOp, Qor> gOrOpCreator_m(OrQT::cMark);
DefaultCreator<string, QcellOp, Qor> gOrOpCreator(OrQT::cName);

DefaultCreator<string, QcellOp, QnOr> gNorOpCreator_m(NorQT::cMark);
DefaultCreator<string, QcellOp, QnOr> gNorOpCreator(NorQT::cName);
/*
DefaultCreator<string, QcellOp, QnLeftOrRight> gNleftOrRightOpCreator("notleftorright");

DefaultCreator<string, QcellOp, DwNotLeftOrRightOp> gNotLeftOrRightOpCreator_dw("dwnotleftorright");
*/
DefaultCreator<string, QcellOp, Qxor> gXorOpCreator_m(XorQT::cMark);
DefaultCreator<string, QcellOp, Qxor> gXorOpCreator(XorQT::cName);

DefaultCreator<string, QcellOp, Qnxor> gNxorOpCreator_m(NxorQT::cMark);
DefaultCreator<string, QcellOp, Qnxor> gNxorOpCreator(NxorQT::cName);

DefaultCreator<string, QcellOp, Qadder05> gHalfAdderOpCreator_m(Adder05QT::cMark);
DefaultCreator<string, QcellOp, Qadder05> gHalfAdderOpCreator(Adder05QT::cName);

DefaultCreator<string, QcellOp, Qadder> gAdderOpCreator_m(AdderQT::cMark);
DefaultCreator<string, QcellOp, Qadder> gAdderOpCreator(AdderQT::cName);

/*** Quantum-nary Operations Factory ***/
#ifdef _WINDOWS
Factory<string, QnaryOp> Factory<string, QnaryOp>::gFactory;
#else
template class Factory<string, QnaryOp>;
template class Factory<string, QnaryOp>::Creator<string, QnaryOp>;
#endif

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
DefaultCreator<string, QcellOp, Qsubtract> gSubtractionCreator_m(Qsubtract::cMark);
DefaultCreator<string, QcellOp, Qsubtract> gSubtractionCreator(Qsubtract::cName);

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
DefaultCreator<string, QcellOp, Qdivide> gDivisionCreator_m(Qdivide::cMark);
DefaultCreator<string, QcellOp, Qdivide> gDivisionCreator(Qdivide::cName);