#include <Factory.h>
#include <Logger.h>

#include <Qubo.h>
#include <Qop.h>
#include <Qadd.h>
#include <Qmultiply.h>

using namespace std;
using namespace dann5;
using namespace dann5::ocean;


/**** A global instance of dann5 Logger ****/
unique_ptr<Logger> Logger::mpSingleton;

/**** A global instance of Qubo Table Factory singleton ****/
Factory<string, QuboTable> Factory<string, QuboTable>::gFactory;

/**** Equal operator Qubo Table ****/

const string EqQT::cMark = "==";
const string EqQT::cName = "equal";

/**** Factory Creator's of Equal operator Qubo Table ****/
Creator<string, QuboTable, EqQT> gEqualQTcreator_m(EqQT::cMark);
Creator<string, QuboTable, EqQT> gEqualQTcreator(EqQT::cName);


/**** Not operator Qubo Table ****/

const string NeqQT::cMark = "!=";
const string NeqQT::cName = "notequal";

/**** Factory Creator's of Not operator Qubo Table ****/
Creator<string, QuboTable, NeqQT> gNotQTcreator_m(NeqQT::cMark);
Creator<string, QuboTable, NeqQT> gNotQTcreator(NeqQT::cName);


/**** Less-than operator Qubo Table ****/

const string LtQT::cMark = "<";
const string LtQT::cName = "lessthan";

/**** Factory Creator's of Less-than operator Qubo Table ****/
Creator<string, QuboTable, LtQT> gLessThanQTcreator_m(LtQT::cMark);
Creator<string, QuboTable, LtQT> gLessThanQTcreator(LtQT::cName);


/**** Less-equal operator Qubo Table ****/

const string LeQT::cMark = "<=";
const string LeQT::cName = "lessequal";

/**** Factory Creator's of Less-equal operator Qubo Table ****/
Creator<string, QuboTable, LeQT> gLessEqualQTcreator_m(LeQT::cMark);
Creator<string, QuboTable, LeQT> gLessEqualQTcreator(LeQT::cName);


/**** Greater-than operator Qubo Table ****/

const string GtQT::cMark = ">";
const string GtQT::cName = "greaterthan";

/**** Factory Creator's of Greater-than operator Qubo Table ****/
Creator<string, QuboTable, GtQT> gGreaterThanQTcreator_m(GtQT::cMark);
Creator<string, QuboTable, GtQT> gGreaterThanQTcreator(GtQT::cName);


/**** Greater-equal operator Qubo Table ****/

const string GeQT::cMark = ">=";
const string GeQT::cName = "greaterequal";

/**** Factory Creator's of Greater-equal operator Qubo Table ****/
Creator<string, QuboTable, GeQT> gGreaterEqualQTcreator_m(GeQT::cMark);
Creator<string, QuboTable, GeQT> gGreaterEqualQTcreator(GeQT::cName);


/**** And binary operation Qubo Table ****/

const string AndQT::cMark = "&";
const string AndQT::cName = "and";

/**** Factory Creator's of And binary operation Qubo Table ****/
Creator<string, QuboTable, AndQT> gAndQTcreator_m(AndQT::cMark);
Creator<string, QuboTable, AndQT> gAndQTcreator(AndQT::cName);


/**** Nand binary operation Qubo Table with two outputs****/

const string NandQT::cMark = "~&";
const string NandQT::cName = "nand";

/**** Factory Creator's of Nand binary operation Qubo Table ****/
Creator<string, QuboTable, NandQT> gNandQTcreator_m(NandQT::cMark);
Creator<string, QuboTable, NandQT> gNandQTcreator(NandQT::cName);


/**** Or binary operation Qubo Table ****/

const string OrQT::cMark = "|";
const string OrQT::cName = "or";

/**** Factory Creator's of Or binary operation Qubo Table ****/
Creator<string, QuboTable, OrQT> gOrQTcreator_m(OrQT::cMark);
Creator<string, QuboTable, OrQT> gOrQTcreator(OrQT::cName);


/**** Nand binary operation Qubo Table with two outputs****/

const string NorQT::cMark = "~|";
const string NorQT::cName = "nor";

/**** Factory Creator's of Nor binary operation Qubo Table ****/
Creator<string, QuboTable, NorQT> gNorQTcreator_m(NorQT::cMark);
Creator<string, QuboTable, NorQT> gNorQTcreator(NorQT::cName);


/**** Not-left-or-right binary operation Qubo Table with two outputs****/

const string NotLeftOrRightQT::cMark = "~l|r";
const string NotLeftOrRightQT::cName = "notleftorright";

/**** Factory Creator's of not-left-or-right binary operation Qubo Table ****/
Creator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator_m(NotLeftOrRightQT::cMark);
Creator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator(NotLeftOrRightQT::cName);


/**** DWave not-left-or-right binary operation Qubo Table with two outputs****/

const string DwNotLeftOrRightQT::cMark = "d~l|r";
const string DwNotLeftOrRightQT::cName = "dwnotleftorright";

/**** Factory Creator's of DWave not-left-or-right binary operation Qubo Table ****/
Creator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_m_dw(DwNotLeftOrRightQT::cMark);
Creator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_dw(DwNotLeftOrRightQT::cName);


/**** Xor binary operation Qubo Table with two outputs****/

const string XorQT::cMark = "^";
const string XorQT::cName = "xor";

/**** Factory Creator's of Xor binary operation Qubo Table ****/
Creator<string, QuboTable, XorQT> gXorQTcreator_m(XorQT::cMark);
Creator<string, QuboTable, XorQT> gXorQTcreator(XorQT::cName);


/**** Nxor binary operation Qubo Table with two outputs****/

const string NxorQT::cMark = "~^";
const string NxorQT::cName = "nxor";

/**** Factory Creator's of Nxor binary operation Qubo Table ****/
Creator<string, QuboTable, NxorQT> gNxorQTcreator_m(NxorQT::cMark);
Creator<string, QuboTable, NxorQT> gNxorQTcreator(NxorQT::cName);


/**** Half-adder binary operation Qubo Table with two outputs****/

const string Adder05QT::cMark = "h+";
const string Adder05QT::cName = "halfadder";

/**** Factory Creator's of Half-adder binary operation Qubo Table ****/
Creator<string, QuboTable, Adder05QT> gHalfAdderQTcreator_m(Adder05QT::cMark);
Creator<string, QuboTable, Adder05QT> gHalfAdderQTcreator(Adder05QT::cName);


/**** Adder trinary operation Qubo Table with two outputs****/

const string AdderQT::cMark = "+";
const string AdderQT::cName = "adder";

/**** Factory Creator's of Adder trinary operation Qubo Table ****/
Creator<string, QuboTable, AdderQT> gAdderQTcreator_m(AdderQT::cMark);
Creator<string, QuboTable, AdderQT> gAdderQTcreator(AdderQT::cName);

/**** Adder Carry operation as one of the outputs****/

const string AdderQT::Carry::cMark = "#";
const string AdderQT::Carry::cName = "carry";

/*** Quantum Cell Operations Factory ***/

Factory<string, QcellOp> Factory<string, QcellOp>::gFactory;

Creator<string, QcellOp, Qeq> gEqualOperatorCreator_m(EqQT::cMark);
Creator<string, QcellOp, Qeq> gEqualOperatorCreator(EqQT::cName);

Creator<string, QcellOp, Qneq> gNotOperatorCreator_m(NeqQT::cMark);
Creator<string, QcellOp, Qneq> gNotOperatorCreator(NeqQT::cName);

Creator<string, QcellOp, Qlt> gLtOperatorCreator_m(LtQT::cMark);
Creator<string, QcellOp, Qlt> gLtOperatorCreator(LtQT::cName);

Creator<string, QcellOp, Qle> gLeOperatorCreator_m(LeQT::cMark);
Creator<string, QcellOp, Qle> gLeOperatorCreator(LeQT::cName);

Creator<string, QcellOp, Qgt> gGtOperatorCreator_m(GtQT::cMark);
Creator<string, QcellOp, Qgt> gGtOperatorCreator(GtQT::cName);

Creator<string, QcellOp, Qge> gGeOperatorCreator_m(GeQT::cMark);
Creator<string, QcellOp, Qge> gGeOperatorCreator(GtQT::cName);

Creator<string, QcellOp, Qand> gAndOpCreator_m(AndQT::cMark);
Creator<string, QcellOp, Qand> gAndOpCreator(AndQT::cName);

Creator<string, QcellOp, QnAnd> gNandOpCreator_m(NandQT::cMark);
Creator<string, QcellOp, QnAnd> gNandOpCreator(NandQT::cName);

Creator<string, QcellOp, Qor> gOrOpCreator_m(OrQT::cMark);
Creator<string, QcellOp, Qor> gOrOpCreator(OrQT::cName);

Creator<string, QcellOp, QnOr> gNorOpCreator_m(NorQT::cMark);
Creator<string, QcellOp, QnOr> gNorOpCreator(NorQT::cName);
/*
Creator<string, QcellOp, QnLeftOrRight> gNleftOrRightOpCreator("notleftorright");

Creator<string, QcellOp, DwNotLeftOrRightOp> gNotLeftOrRightOpCreator_dw("dwnotleftorright");
*/
Creator<string, QcellOp, Qxor> gXorOpCreator_m(XorQT::cMark);
Creator<string, QcellOp, Qxor> gXorOpCreator(XorQT::cName);
/*
Creator<string, QcellOp, NxorOp> gNxorOpCreator_m("~^");
Creator<string, QcellOp, NxorOp> gNxorOpCreator("nxor");
*/
Creator<string, QcellOp, Qadder05> gHalfAdderOpCreator_m(Adder05QT::cMark);
Creator<string, QcellOp, Qadder05> gHalfAdderOpCreator(Adder05QT::cName);

Creator<string, QcellOp, Qadder> gAdderOpCreator_m(AdderQT::cMark);
Creator<string, QcellOp, Qadder> gAdderOpCreator(AdderQT::cName);

/*** Quantum-nary Operations Factory ***/

Factory<string, QnaryOp> Factory<string, QnaryOp>::gFactory;

/**** Factory Creator's of Addition ****/
Creator<string, QnaryOp, Qadd> gAdditionCreator_m(AdderQT::cMark);
Creator<string, QnaryOp, Qadd> gAdditionCreator("add");

/**** Multiplication binary operation ****/

const string Qmultiply::cMark = "*";
const string Qmultiply::cName = "multiply";

/**** Factory Creator's of Multiplication ****/
Creator<string, QnaryOp, Qmultiply> gMultiplicationCreator_m(Qmultiply::cMark);
Creator<string, QnaryOp, Qmultiply> gMultiplicationCreator((string(Qmultiply::cName)));