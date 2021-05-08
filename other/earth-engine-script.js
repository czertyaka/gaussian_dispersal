// calculate coordinates of square center
var latC = 60+44/60+2.55/3600;
var lonC = 55+46/60+9.4/3600;

var CC = ee.Geometry.Point([latC, lonC]);

var Ht = 0.26945 * 2; // height of square

var N = lonC + Ht / 2; // bounds of longitude
var S = lonC - Ht / 2;

var CN = ee.Geometry.Point([latC, N]);
var CS = ee.Geometry.Point([latC, S]);

var Wdth = 0.477985 * 2; // width of square

var W = latC - Wdth / 2; // bounds of longitude
var E = latC + Wdth / 2;

var WC = ee.Geometry.Point([W, lonC]);
var EC = ee.Geometry.Point([E, lonC]);

// set area of interest
var geometry = ee.Geometry.BBox(W, S, E, N);
Map.addLayer(geometry, {}, "BBox", true, 0.5);

// get projection with 50 m step
var default_projection = ee.Projection({
                            crs: 'EPSG:4326',
                          })
                          .atScale(50);

// get lons and lats
var pixels = ee.Image.pixelLonLat()
                .clip(geometry)
                .reproject({
                  crs: default_projection
                });
print(pixels.sample());

// get land cover
var land_cover = ee.Image("COPERNICUS/Landcover/100m/Proba-V-C3/Global/2019")
                .select('discrete_classification')
                .clip(geometry)
                .reproject({
                  crs: default_projection
                });
print(land_cover.sample());                

// and elevation
var elevation = ee.Image('CGIAR/SRTM90_V4')
                .clip(geometry)
                .reproject({
                  crs: default_projection
                });
print(elevation.sample());                
                
Map.addLayer(elevation, {min: 200, max: 300}, "Elevation", true, 0.5);
Map.addLayer(land_cover, {}, "Land Cover", true, 0.5);

var image = ee.Image.cat(pixels, elevation, land_cover);

Export.table.toDrive({
  collection: image.sample(),
  fileNamePrefix: "image_data"
});