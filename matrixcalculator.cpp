#include "matrixcalculator.h"

MatrixCalculator::MatrixCalculator() :
    BaseCalculator(),
    m_journal(m_db.ClimateJournal()),
    m_matrix(m_db.Matrix())
{

}

BaseCalculator::t_errorCode MatrixCalculator::Execute()
{  
    return OK;
}

bool MatrixCalculator::AddObservation(const mm::t_observation &obs)
{    
    return true;
}
