#include <Factory.h>
#include <Logger.h>

#include <Qubo.h>
#include <QcellOps.h>


using namespace std;
using namespace dann5;
using namespace dann5::ocean;


/**** A global instance of Qubo Table Factory singleton ****/
#ifdef _WINDOWS // error LNK2001: unresolved external symbol "private: static class dann5::Factory<string,QuboTable> dann5::Factory<string,QuboTable>::gFactory"
// Factory<string, QuboTable> Factory<string, QuboTable>::gFactory;
QTfactory QTfactory::gFactory;
#else
template class dann5::Factory<string, QuboTable>;
template class QTfactory::Creator<string, QuboTable>;
#endif

/**** Factory Creator's of Equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, InvertQT> gInvertQTcreator_m(Qinvert::cMark);
DefaultCreator<string, QuboTable, InvertQT> gInvertQTcreator(Qinvert::cName);

/**** Factory Creator's of Equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, EqQT> gEqualQTcreator_m(Qeq::cMark);
DefaultCreator<string, QuboTable, EqQT> gEqualQTcreator(Qeq::cName);

/**** Factory Creator's of Not operator Qubo Table ****/
DefaultCreator<string, QuboTable, NeqQT> gNotQTcreator_m(Qneq::cMark);
DefaultCreator<string, QuboTable, NeqQT> gNotQTcreator(Qneq::cName);

/**** Factory Creator's of Less-than operator Qubo Table ****/
DefaultCreator<string, QuboTable, LtQT> gLessThanQTcreator_m(Qlt::cMark);
DefaultCreator<string, QuboTable, LtQT> gLessThanQTcreator(Qlt::cName);

/**** Factory Creator's of Less-equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, LeQT> gLessEqualQTcreator_m(Qle::cMark);
DefaultCreator<string, QuboTable, LeQT> gLessEqualQTcreator(Qle::cName);

/**** Factory Creator's of Greater-than operator Qubo Table ****/
DefaultCreator<string, QuboTable, GtQT> gGreaterThanQTcreator_m(Qgt::cMark);
DefaultCreator<string, QuboTable, GtQT> gGreaterThanQTcreator(Qgt::cName);

/**** Factory Creator's of Greater-equal operator Qubo Table ****/
DefaultCreator<string, QuboTable, GeQT> gGreaterEqualQTcreator_m(Qge::cMark);
DefaultCreator<string, QuboTable, GeQT> gGreaterEqualQTcreator(Qge::cName);

/**** Factory Creator's of And binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, AndQT> gAndQTcreator_m(Qand::cMark);
DefaultCreator<string, QuboTable, AndQT> gAndQTcreator(Qand::cName);

/**** Factory Creator's of Nand binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NandQT> gNandQTcreator_m(Qnand::cMark);
DefaultCreator<string, QuboTable, NandQT> gNandQTcreator(Qnand::cName);

/**** Factory Creator's of Or binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, OrQT> gOrQTcreator_m(Qor::cMark);
DefaultCreator<string, QuboTable, OrQT> gOrQTcreator(Qor::cName);

/**** Factory Creator's of Nor binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NorQT> gNorQTcreator_m(Qnor::cMark);
DefaultCreator<string, QuboTable, NorQT> gNorQTcreator(Qnor::cName);

/**** Factory Creator's of not-left-or-right binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator_m("!l|r");
DefaultCreator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator("notleftorright");

/**** Factory Creator's of DWave not-left-or-right binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_m_dw("d!l|r");
DefaultCreator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_dw("dwnotleftorright");

/**** Factory Creator's of Xor binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, XorQT> gXorQTcreator_m(Qxor::cMark);
DefaultCreator<string, QuboTable, XorQT> gXorQTcreator(Qxor::cName);

/**** Factory Creator's of Nxor binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, NxorQT> gNxorQTcreator_m(Qnxor::cMark);
DefaultCreator<string, QuboTable, NxorQT> gNxorQTcreator(Qnxor::cName);

/**** Factory Creator's of Half-adder binary operation Qubo Table ****/
DefaultCreator<string, QuboTable, HalfAdderQT> gHalfAdderQTcreator_m(QhalfAdder::cMark);
DefaultCreator<string, QuboTable, HalfAdderQT> gHalfAdderQTcreator(QhalfAdder::cName);

/**** Factory Creator's of Adder trinary operation Qubo Table ****/
DefaultCreator<string, QuboTable, AdderQT> gAdderQTcreator_m(Qadder::cMark);
DefaultCreator<string, QuboTable, AdderQT> gAdderQTcreator(Qadder::cName);
