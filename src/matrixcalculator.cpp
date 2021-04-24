#include "matrixcalculator.h"
#include "smithparamcalculator.h"
#include "datainterface.h"

#include <QDateTime>

MatrixCalculator::MatrixCalculator() :
    BaseCalculator(),
    m_journal(m_db.ClimateJournal()),
    m_matrix(m_db.Matrix())
{

}

BaseCalculator::t_errorCode MatrixCalculator::Execute()
{  
    for (DataBase::t_climateJournal::iterator iter = m_db.ClimateJournal().begin();
         iter != m_db.ClimateJournal().end(); ++iter)
    {
        if (!AddObservation(*iter))
        {
            MY_LOG("error while adding observation " << iter->dateTime.toString() << " to matrix");
            return ERROR;
        }
    }

    return OK;
}

bool MatrixCalculator::AddObservation(const mm::t_observation &obs)
{    
    SmithParamCalculator smithParamCalculator(obs);
    if (smithParamCalculator.Execute() == ERROR)
    {
        MY_LOG("error while calculating smith parameter");
        return false;
    }

    return true;
}
