#ifndef SOURCESDATA_H
#define SOURCESDATA_H

#include <QString>
#include <optional>
#include <vector>

#include "basedataloader.h"
#include "globaltypes.h"

class SourcesLoader : public BaseDataLoader
{
public:
    typedef struct sources
    {
        int                     id;
        QString                 nuclide;
        t_epsg4326coord     coord;
        double                  height;
        double                  temp;
        std::optional<double>   emission;
        std::optional<double>   firstQuarter;
        std::optional<double>   secondQuarter;
        std::optional<double>   thirdQuarter;
        std::optional<double>   fourthQuarter;
    } t_source;
    typedef std::vector<t_source> t_vSources;
    SourcesLoader();
    virtual ~SourcesLoader() = default;
    bool AddSources(const mt::t_sourcesTable& sources, const mt::t_emissionsTable& emissions);
    void Reset() override;
};

#endif // SOURCESDATA_H
