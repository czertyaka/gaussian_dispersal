#include "basecalculator.h"
#include "database.h"

BaseCalculator::BaseCalculator() :
    m_db(DataBase::GetInstance()),
    m_error(OK)
{

}
