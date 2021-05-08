#ifndef GEOGRAPHY_H
#define GEOGRAPHY_H

struct epsg4326coord;

typedef struct epsg3857coord
{
    double easting;
    double northing;
    epsg3857coord(double easting = 0, double northing = 0);
    epsg3857coord(const epsg4326coord& o);
    bool operator==(const epsg3857coord& o) const;
} t_epsg3857coord;

typedef t_epsg3857coord t_pseudoMercatorCoord;

typedef struct epsg4326coord
{
    enum t_Unit
    {
        DEGREES,
        RADIANS
    };
    double lon;
    double lat;
    epsg4326coord(double lon = 0, double lat = 0, t_Unit unit = DEGREES);
    epsg4326coord(const epsg3857coord& o);
    bool operator==(const epsg4326coord& o) const;
    void SetUnits(t_Unit unit);
private:
    t_Unit m_unit;;
} t_epsg4326coord;

double distance(t_epsg4326coord p1, t_epsg4326coord p2);

#endif // GEOGRAPHY_H
