#include <QtTest>

// add necessary includes here
#include "meteorology.h"

using namespace mm;

class scattering_test : public QObject
{
    Q_OBJECT

public:
    scattering_test();
    ~scattering_test();

private slots:
    void test_3857to4326();
    void test_4326to3857();

};

scattering_test::scattering_test()
{

}

scattering_test::~scattering_test()
{

}

void scattering_test::test_3857to4326()
{
    t_epsg3857coord epsg3857;
}

void scattering_test::test_4326to3857()
{
    t_epsg4326coord epsg4326;

    {
        epsg4326.lat = 0;
        epsg4326.lon = 0;

        t_epsg3857coord epsg3857(epsg4326);

        const double delta = 1e-6;
        QVERIFY(epsg3857.easting >= -delta && epsg3857.easting <= delta);
        QVERIFY(epsg3857.northing >= -delta && epsg3857.northing <= delta);
    }

    {
        epsg4326.lat = 10;
        epsg4326.lon = 10;

        t_epsg3857coord epsg3857(epsg4326);


        const double delta = 10;
        const double exp_easting = 1113194.91;
        const double exp_northing = 11118889.97;
        QVERIFY(epsg3857.easting >= exp_easting - delta && epsg3857.easting <= exp_easting + delta);
        QVERIFY(epsg3857.northing >= exp_northing - delta && epsg3857.northing <= exp_northing + delta);
    }
}

QTEST_APPLESS_MAIN(scattering_test)

#include "tst_scattering_test.moc"
