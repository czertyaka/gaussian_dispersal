#ifndef SOURCESDATA_H
#define SOURCESDATA_H

#include <QString>
#include <optional>
#include <vector>

#include "datamanager.h"

class SourcesData : public DataManager
{
public:
    enum t_coordinatesType
    {
        EPSG3857,
        RELATIVE
    };
    typedef struct sources
    {
        int                     id;
        QString                 nuclide;
        t_coordinatesType       coordinatesType;
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
    void Reset() override;
};

#endif // SOURCESDATA_H
