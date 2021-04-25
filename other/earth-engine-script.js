var default_projection = ee.Projection({
  crs: 'EPSG:4326',
})
.atScale(50);

var pixels = ee.Image.pixelLonLat()
.clip(geometry)
.reproject({
crs: default_projection
});
print(pixels.sample());

var land_cover = ee.Image("COPERNICUS/Landcover/100m/Proba-V-C3/Global/2019")
.select('discrete_classification')
.clip(geometry)
.reproject({
crs: default_projection
});
print(land_cover.sample());                

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