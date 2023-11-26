//
//  QlogicCompiler.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-23.
//

#include <QlogicCompiler.h>

#include <memory>
#include <typeinfo>

#include <QnaryOp.h>
#include <QcellOps.h>

using namespace dann5;

void QlogicCompiler::compile(const Qop& op)
{
    const QnaryOp* pNaryOp = dynamic_cast<const QnaryOp*>(&op);
    if(pNaryOp != nullptr)
    {
        const Qcells& opLogic = pNaryOp->cells();
        for(auto pCell: opLogic)
            mLogic.push_back(static_pointer_cast<Qcell>(pCell->clone()));
        return;
    }
    const QcellOp* pCellOp = dynamic_cast<const QcellOp*>(&op);
    if(pCellOp != nullptr)
        mLogic.push_back(static_pointer_cast<Qcell>(pCellOp->clone()));
    else
        throw logic_error("ERROR @QlogicCompiler: Qop is neither QnaryOp or QcellOp.");
}
