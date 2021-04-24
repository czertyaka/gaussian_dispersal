#ifndef ClimaticVariablesLoader_H
#define ClimaticVariablesLoader_H

#include <QString>

#include "basedataloader.h"
#include "climatecsvparser.h"

class ClimaticVariablesLoader : public BaseDataLoader
{
public:
    ClimaticVariablesLoader();
    virtual ~ClimaticVariablesLoader();
    bool AddJournal(const QString& filename, ClimateCsvParser::t_format format);
    void Reset() override;
private:
    ClimateCsvParser* m_parser;
};

#endif // ClimaticVariablesLoader_H
