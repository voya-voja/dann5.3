//
//  UTestULint.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-09-29.
//

#include "UTestULint.hpp"

#include <ULint.h>
#include <bitset>

using namespace dann5test;

using namespace dann5;

void UTestULint::runAll(ostream& out)
{

    ULint bn("101",2), tr("120201", 3), oc("377", 8), dc("355"), hx("ff", 16);
    cout << endl << bn.toString(2) << ", " << bn.toString(3) << ", " << bn.toString(8) << ", " << bn << ", " << bn.toString(16);
    cout << endl << tr.toString(2) << ", " << tr.toString(3) << ", " << tr.toString(8) << ", " << tr << ", " << tr.toString(16);
    cout << endl << oc.toString(2) << ", " << oc.toString(3) << ", " << oc.toString(8) << ", " << oc << ", " << oc.toString(16);
    cout << endl << dc.toString(2) << ", " << dc.toString(3) << ", " << dc.toString(8) << ", " << bn << ", " << dc.toString(16);
    cout << endl << hx.toString(2) << ", " << hx.toString(3) << ", " << hx.toString(8) << ", " << hx << ", " << hx.toString(16);

    ULint prm(40437), _a3(3), _a4(4);

    ULint a = prm + _a3,
        s = prm - _a3,
        m = prm * _a3,
        d = prm / _a3,
        r = prm % _a3;

    out << "add: " << a << endl
        << "sub: " << s << endl
        << "mul: " << m << endl
        << "div: " << d << endl
        << "rem: " << r << endl;
    out << "r == 0: " << (r == ULint(0, true)) << endl
        << "a > s: " << (a > s) << endl
        << "a >= m: " << (a > m) << endl
        << "m < d: " << (m < d) << endl
        << "d <= s: " << (d <= s) << endl;

    a = prm + _a4;
    s = prm - _a4;
    m = prm * _a4;
    d = prm / _a4;
    r = prm % _a4;

    out << "add: " << a << endl
        << "sub: " << s << endl
        << "mul: " << m << endl
        << "div: " << d << endl
        << "rem: " << r << endl
        << "r == 0: " << (r == ULint(0, true)) << endl
        << "a > s: " << (a > s) << endl
        << "a >= m: " << (a > m) << endl
        << "m < d: " << (m < d) << endl
        << "d <= s: " << (d <= s) << endl;

    std::bitset<9> b(52415993);
    
    ULint u(52415993);
    ULint ten = ULint(10);
    ULint five = ULint(5);
    bool gt = ten > ten;
    bool g = ten > five;
    bool t = five > ten;
    ULint three(3);
    u = ten * three;
    u *= five;
    u %= three;
    out << b << endl << u.toString(2) << endl;
    out << endl << oct << b.to_ullong() << endl << u << endl;
    out << dec << b.to_ullong() << endl << u << endl;
    out << hex << b.to_ullong() << endl  << u << endl << dec;

    unsigned long long value = 1800999832145436372;
    std::bitset<64> bs(value);
    out << value << endl << bs << endl;
    ULint ulint(1800999832145436372);
    ulint /= three;
    for (size_t at = 7; (at + 1) > 0; at--)
    {
        std::bitset<8> x1(ulint[at]);
        out << x1;
    }
    out << endl << ulint.toString(2) << endl;
    out << oct << bs.to_ullong() << endl << ulint << endl;
    out << dec << bs.to_ullong() << endl << ulint << endl << ulint.toString(10) << endl;
    out << endl << hex << bs.to_ullong() << endl << ulint << endl << dec;
    out << ulint.toString(3) << endl;

    ULint assgn(257), assgnmnt;
    assgnmnt = assgn;
    out << assgnmnt << " = " << assgn << endl;
    out << "assignment: " << assgnmnt.toString(2) << endl;

    ULint assgn1(513), assgnmnt1(1025);
    out << assgnmnt1 << " = " << assgn1 << endl;
    out << "assignment: " << assgnmnt1.toString(2) << " = " << assgn1.toString(2) << endl;

    ULint assgn2(512), assgnmnt2(1024), a3(256);
    out << assgnmnt2 << " = " << assgn2 << " & " << a3 << endl;
    out << "assignment: " << assgnmnt2.toString(2) << endl;

    assgnmnt >>= 3;
    out << assgnmnt << " >>= " << 3 << endl;
    out << "assignment: " << assgnmnt.toString(2) << endl;

    assgnmnt <<= 5;
    out << assgnmnt << " <<= " << 5 << endl;
    out << "assignment: " << assgnmnt.toString(2) << endl;

    assgnmnt += assgn;
    out << assgnmnt << " += " << assgn << endl;
    out << "assignment: " << assgnmnt.toString(2) << endl;

    assgnmnt -= assgn;
    assgnmnt -= assgn;
    out << assgnmnt << " -= 2 * " << assgn << endl;

    assgn *= assgn;
    out << assgn << " *= " << assgn << endl;

    ULint dvdnd(210862), dvsr(358);
    ULint qtnt = dvdnd / dvsr;
    out << qtnt << " = " << dvdnd << " / " << dvsr << endl;

    assgnmnt = ulint % assgn;
    out << assgnmnt << " = "<< ulint << " % " << assgn << endl;

    ULint br("ff", 16);
    cout << endl << br << ", " << br.toString(16) << "\n";
    br += ten;
    cout << endl << br << ", " << br.toString(16) << "\n";

    ULint x("15305780290961646443858803558983811673237734933575719864519459714368164464027");
    cout << "15305780290961646443858803558983811673237734933575719864519459714368164464027\n" << x.toString();
    
    ULint x1("4933575719864519459714368164464027"), zero;
    cout  << hex << "\n4933575719864519459714368164464027\n" << dec << x1.toString(16);
    while (x1 != zero)
    {
        x1 /= ten;
        cout << "\n" << x1.toString(16) << "\t" << x1.toString();
    }
    ULint x10th("493357571986451945971436816446402");
    cout << "\n" << x10th.toString(16) << "\t" << x10th.toString();
    while (x10th != zero)
    {
        x10th /= ten;
        cout << "\n" << x10th.toString(16) << "\t" << x10th.toString();
    }
    ULint x100th("49335757198645194597143681644640");
    cout << "\n" << x100th.toString(16) << "\t" << x100th.toString() << "\n" << x100th.toString(2) << "\n";   //incorrect
    while (x100th != zero)
    {
        x100th /= ten;
        cout << "\n" << x100th.toString(16) << "\t" << x100th.toString();
    }
    ULint x1000th("4933575719864519459714368164464");
    ULint xt(x1000th * ten);
    cout << "\n" << xt.toString(16) << "\t" << xt.toString() << "\n" << xt.toString(2) << "\n"; //
    cout << "\n" << x1000th.toString(16) << "\t" << x1000th.toString() << "\n" << x1000th.toString(2) << "\n"; // correct
    while (x1000th != zero)
    {
        x1000th /= ten;
        cout << "\n" << x1000th.toString(16) << "\t" << x1000th.toString();
    }
}
