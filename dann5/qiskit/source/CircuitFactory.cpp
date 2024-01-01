#include <Factory.h>
#include <Logger.h>

#include <Circuits.h>
#include <QcellOps.h>


using namespace std;
using namespace dann5;
using namespace dann5::qiskit;


/**** A global instance of Qubo Table Factory singleton ****/
#ifdef _WINDOWS // error LNK2001: unresolved external symbol "private: static class dann5::Factory<string,Circuit> dann5::Factory<string,Circuit>::gFactory"
// Factory<string, Circuit> Factory<string, Circuit>::gFactory;
CircuitFactory CircuitFactory::gFactory;
#else
template class dann5::Factory<string, Circuit>;
template class CircuitFactory::Creator<string, Circuit>;
#endif

/**** Factory Creator's of measure and reset circuit ****/
DefaultCreator<string, Circuit, MeasureCircuit> gMeasureCircuitcreator("measure");
DefaultCreator<string, Circuit, ResetCircuit> gResetCircuitcreator("reset");

/**** Factory Creator's of DWave hadamard Circuit ****/
DefaultCreator<string, Circuit, HadamardCircuit> gHadamardCircuitcreator_m("h");
DefaultCreator<string, Circuit, HadamardCircuit> gHadamardCircuitcreator("hadamard");

/**** Factory Creator's of Equal operator Qubo Table ****/
DefaultCreator<string, Circuit, InvertCircuit> gInvertCircuitcreator_m(Qinvert::cMark());
DefaultCreator<string, Circuit, InvertCircuit> gInvertCircuitcreator(Qinvert::cName());

/**** Factory Creator's of Equal operator Qubo Table ****/
DefaultCreator<string, Circuit, EqCircuit> gEqualCircuitcreator_m(Qeq::cMark());
DefaultCreator<string, Circuit, EqCircuit> gEqualCircuitcreator(Qeq::cName());

/**** Factory Creator's of Not operator Qubo Table ****/
DefaultCreator<string, Circuit, NeqCircuit> gNotCircuitcreator_m(Qneq::cMark());
DefaultCreator<string, Circuit, NeqCircuit> gNotCircuitcreator(Qneq::cName());

/**** Factory Creator's of Less-than operator Qubo Table ****/
DefaultCreator<string, Circuit, LtCircuit> gLessThanCircuitcreator_m(Qlt::cMark());
DefaultCreator<string, Circuit, LtCircuit> gLessThanCircuitcreator(Qlt::cName());

/**** Factory Creator's of Less-equal operator Qubo Table ****/
DefaultCreator<string, Circuit, LeCircuit> gLessEqualCircuitcreator_m(Qle::cMark());
DefaultCreator<string, Circuit, LeCircuit> gLessEqualCircuitcreator(Qle::cName());

/**** Factory Creator's of Greater-than operator Qubo Table ****/
DefaultCreator<string, Circuit, GtCircuit> gGreaterThanCircuitcreator_m(Qgt::cMark());
DefaultCreator<string, Circuit, GtCircuit> gGreaterThanCircuitcreator(Qgt::cName());

/**** Factory Creator's of Greater-equal operator Qubo Table ****/
DefaultCreator<string, Circuit, GeCircuit> gGreaterEqualCircuitcreator_m(Qge::cMark());
DefaultCreator<string, Circuit, GeCircuit> gGreaterEqualCircuitcreator(Qge::cName());

/**** Factory Creator's of And binary operation Qubo Table ****/
DefaultCreator<string, Circuit, AndCircuit> gAndCircuitcreator_m(Qand::cMark());
DefaultCreator<string, Circuit, AndCircuit> gAndCircuitcreator(Qand::cName());

/**** Factory Creator's of Nand binary operation Qubo Table ****/
DefaultCreator<string, Circuit, NandCircuit> gNandCircuitcreator_m(Qnand::cMark());
DefaultCreator<string, Circuit, NandCircuit> gNandCircuitcreator(Qnand::cName());

/**** Factory Creator's of Or binary operation Qubo Table ****/
DefaultCreator<string, Circuit, OrCircuit> gOrCircuitcreator_m(Qor::cMark());
DefaultCreator<string, Circuit, OrCircuit> gOrCircuitcreator(Qor::cName());

/**** Factory Creator's of Nor binary operation Qubo Table ****/
DefaultCreator<string, Circuit, NorCircuit> gNorCircuitcreator_m(Qnor::cMark());
DefaultCreator<string, Circuit, NorCircuit> gNorCircuitcreator(Qnor::cName());

/**** Factory Creator's of Xor binary operation Qubo Table ****/
DefaultCreator<string, Circuit, XorCircuit> gXorCircuitcreator_m(Qxor::cMark());
DefaultCreator<string, Circuit, XorCircuit> gXorCircuitcreator(Qxor::cName());

/**** Factory Creator's of Nxor binary operation Qubo Table ****/
DefaultCreator<string, Circuit, NxorCircuit> gNxorCircuitcreator_m(Qnxor::cMark());
DefaultCreator<string, Circuit, NxorCircuit> gNxorCircuitcreator(Qnxor::cName());

/**** Factory Creator's of Half-adder binary operation Qubo Table ****/
DefaultCreator<string, Circuit, HalfAdderCircuit> gHalfAdderCircuitcreator_m(QhalfAdder::cMark());
DefaultCreator<string, Circuit, HalfAdderCircuit> gHalfAdderCircuitcreator(QhalfAdder::cName());

/**** Factory Creator's of Adder trinary operation Qubo Table ****/
DefaultCreator<string, Circuit, AdderCircuit> gAdderCircuitcreator_m(Qadder::cMark());
DefaultCreator<string, Circuit, AdderCircuit> gAdderCircuitcreator(Qadder::cName());
