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
        mt::t_epsg4326coord     coord;
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
    bool AddSources(const t_vSources& vSources);
    void Reset() override;
};

#endif // SOURCESDATA_H
