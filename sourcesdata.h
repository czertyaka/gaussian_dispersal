#ifndef SOURCESDATA_H
#define SOURCESDATA_H

#include <QString>
#include <optional>
#include <vector>

#include "datamanager.h"

class SourcesData : public DataManager
{
public:
    typedef struct sources
    {
        int                     id;
        QString                 nuclide;
        double                  x;
        double                  y;
        double                  height;
        double                  temp;
        std::optional<double>   emission;
        std::optional<double>   firstQuarter;
        std::optional<double>   secondQuarter;
        std::optional<double>   thirdQuarter;
        std::optional<double>   fourthQuarter;
    } t_source;
    typedef std::vector<t_source> t_vSources;
    SourcesData();
    bool AddSources(const t_vSources& vSources);
};

#endif // SOURCESDATA_H
