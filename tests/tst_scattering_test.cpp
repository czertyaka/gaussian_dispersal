#include <QtTest>
#include <sstream>
#include <QDir>

#ifndef TESTING
#error
#endif

// add necessary includes here
#include "globaltypes.h"
#include "csvwriter.h"
#include "database.h"

DataBase& DataBase::GetInstance()
{
    static DataBase self;
    return self;
}

DataBase::DataBase() :
    m_concentrationsTable(m_dilutionFactorsTable)
{}

DataBase::~DataBase() {}

DataBase::t_emissionsTable &DataBase::Emissions()
{
    return m_emissionsTable;
}

DataBase::t_sourcesTable &DataBase::Sources()
{
    return m_sources;
}

using namespace mt;

static void Show(const t_epsg3857coord& coord)
{
    std::ostringstream ss;
    ss << "E = " << coord.easting << ", N =" << coord.northing;
    QWARN(ss.str().c_str());
}

static void Show(const t_epsg4326coord& coord)
{
    std::ostringstream ss;
    ss << "lat = " << coord.lat << ", lon =" << coord.lon;
    QWARN(ss.str().c_str());
}

class scattering_test : public QObject
{
    Q_OBJECT

public:
    scattering_test();
    ~scattering_test();

private slots:
    void test_3857to4326();
    void test_4326to3857();
    void test_csvWriter();
    void test_degreesToWindDir();
    void test_interpoltedSlice();
};

scattering_test::scattering_test()
{

}

scattering_test::~scattering_test()
{

}

static void test_3857to4326Case(const t_epsg3857coord& epsg3857, double exp_lat, double exp_lon)
{
    const double delta = 0.1;

    t_epsg4326coord epsg4326(epsg3857);
    Show(epsg4326);

    QVERIFY(epsg4326.lat >= exp_lat - delta && epsg4326.lat <= exp_lat + delta);
    QVERIFY(epsg4326.lon >= exp_lon - delta && epsg4326.lon <= exp_lon + delta);
}

void scattering_test::test_3857to4326()
{
    t_epsg3857coord epsg3857;

    epsg3857.easting = 0;
    epsg3857.northing = 0;
    test_3857to4326Case(epsg3857, 0, 0);

    epsg3857.easting = 0;
    epsg3857.northing = -19971868.88;
    test_3857to4326Case(epsg3857, -85, 0);

    epsg3857.easting = 0;
    epsg3857.northing = 19971868.88;
    test_3857to4326Case(epsg3857, 85, 0);

    epsg3857.easting = 20037508.34;
    epsg3857.northing = 0;
    test_3857to4326Case(epsg3857, 0, 180);

    epsg3857.easting = -20037508.34;
    epsg3857.northing = 0;
    test_3857to4326Case(epsg3857, 0, -180);

    epsg3857.easting = 2671667.78;
    epsg3857.northing = -5311971.85;
    test_3857to4326Case(epsg3857, -43, 24);
}

static void test_4326to3857Case(const t_epsg4326coord& epsg4326, double exp_easting, double exp_northing)
{
    const double delta = 1;

    t_epsg3857coord epsg3857(epsg4326);
    Show(epsg3857);

    QVERIFY(epsg3857.easting >= exp_easting - delta && epsg3857.easting <= exp_easting + delta);
    QVERIFY(epsg3857.northing >= exp_northing - delta && epsg3857.northing <= exp_northing + delta);
}

void scattering_test::test_4326to3857()
{
    t_epsg4326coord epsg4326;

    epsg4326.lat = 0;
    epsg4326.lon = 0;
    test_4326to3857Case(epsg4326, 0, 0);

    epsg4326.lat = -85;
    epsg4326.lon = 0;
    test_4326to3857Case(epsg4326, 0, -19971868.88);

    epsg4326.lat = 85;
    epsg4326.lon = 0;
    test_4326to3857Case(epsg4326, 0, 19971868.88);

    epsg4326.lat = 0;
    epsg4326.lon = 180;
    test_4326to3857Case(epsg4326, 20037508.34, 0);

    epsg4326.lat = 0;
    epsg4326.lon = -180;
    test_4326to3857Case(epsg4326, -20037508.34, 0);

    epsg4326.lat = -43;
    epsg4326.lon = 24;
    test_4326to3857Case(epsg4326, 2671667.78, -5311971.85);
}

void scattering_test::test_csvWriter()
{
    QString dir = QDir::currentPath();

    {
        CsvWriter writer(dir + "/test1.csv", 2);
        QVERIFY(writer.Init());

        writer.AddComment("comment");
        writer.AddItem(1);
        writer.AddItem(0.2);
        writer.AddItem('v');
        writer.AddItem("haha");
    }
}

void scattering_test::test_degreesToWindDir()
{
    QVERIFY(degree_to_dir(0) == E);
    QVERIFY(degree_to_dir(-4) == E);
    QVERIFY(degree_to_dir(363) == E);
    QVERIFY(degree_to_dir(-350) == E);
    QVERIFY(degree_to_dir(10) == E);
    QVERIFY(degree_to_dir(89) == N);
    QVERIFY(degree_to_dir(90) == N);
    QVERIFY(degree_to_dir(91) == N);
}

void scattering_test::test_interpoltedSlice()
{
    std::array<double, WIND_DIR_COUNT> array = {1, 3, 2, 5, 4, 7, 6, 9, 8,
                                                11, 10, 13, 12, 15, 14, 16};
    InterpolatedSlice interp;
    interp.Init(array);

    QString dir = QDir::currentPath();
    CsvWriter writer(dir + "/test2.csv", 2);
    QVERIFY(writer.Init());

    for (double degree = -2*360; degree < 2*360; ++degree)
    {
        writer.AddItem(degree);
        writer.AddItem(interp(degree));
    }
}

QTEST_APPLESS_MAIN(scattering_test)

#include "tst_scattering_test.moc"
