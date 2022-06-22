/*
Author: hwd

Date:2017-03-07

Revised by dhr on 2019-5-10

Description:Provide the superclass for geoMap,choroplethmap and heatmap
 */

 L.TileLayer.ChinaProvider = L.TileLayer.extend({

    initialize: function(type, options) { // (type, Object)
        var providers = L.TileLayer.ChinaProvider.providers;

        var parts = type.split('.');

        var providerName = parts[0];
        var mapName = parts[1];
        var mapType = parts[2];

        var url = providers[providerName][mapName][mapType];
        options.subdomains = providers[providerName].Subdomains;
        options.key = options.key || providers[providerName].key;
        L.TileLayer.prototype.initialize.call(this, url, options);
    }
});

L.TileLayer.ChinaProvider.providers = {
    TianDiTu: {
        Normal: {
            Map: "https://t{s}.tianditu.gov.cn/vec_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=vec&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=9c5f9860353de21dac86f148232f3fb8",
            Annotion: "https://t{s}.tianditu.gov.cn/cva_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=cva&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=9c5f9860353de21dac86f148232f3fb8"
        },
        Satellite: {
            Map: "https://t{s}.tianditu.gov.cn/img_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=img&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=9c5f9860353de21dac86f148232f3fb8",
            Annotion: "https://t{s}.tianditu.gov.cn/cia_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=cia&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=9c5f9860353de21dac86f148232f3fb8"
        },
        Terrain: {
            Map: "https://t{s}.tianditu.gov.cn/ter_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=ter&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=9c5f9860353de21dac86f148232f3fb8",
            Annotion: "https://t{s}.tianditu.gov.cn/cta_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=cta&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=9c5f9860353de21dac86f148232f3fb8"
        },
        Subdomains: ['0', '1', '2', '3', '4', '5', '6', '7']
    }
};

L.tileLayer.chinaProvider = function(type, options) {
    return new L.TileLayer.ChinaProvider(type, options);
};

function GeoMap(divId,zoom,baseMapType,showToolTipLayer){
	this.divId=divId;
	this.zoom=zoom;
	this.center=null;
	this.baseMapType=baseMapType;
	this.showToolTipLayer=showToolTipLayer;
	this.baseMaps=this.getBaseLayer();
	this.map=null;
	this.color={
		point:['#e41a1c','#e7298a'],
		line:['#d95f02','#984ea3'],
		polygon:['#ff7f00','#ffff33']
	};
}
GeoMap.prototype={
	getCentroid:function(geo){
		/*let pointArray;
	    if(geo.type==='Point'){
	        return [geo.coordinates[0],geo.coordinates[1]];
	    }
	    else if(geo.type==='MultiPolygon')
	        pointArray=geo.coordinates[0][0];
	    else if(geo.type==='MultiLineString'||geo.type==='Polygon')
	        pointArray=geo.coordinates[0];
	    let lon=0,lat=0;
	    pointArray.forEach(point=>{
	        lon+=point[0];
	        lat+=point[1];
	    });*/
	    let lat=0,lng=0,latlng;
	    geo.forEach(function(g){
	    	latlng=d3.geoCentroid(g);
	    	lat+=latlng[0];
	    	lng+=latlng[1];
	    });
	    return [lat/geo.length,lng/geo.length];
	},
	getMap:function({divId,center,zoom}){
		return L.map(divId).setView(center,zoom);
	},
	getBaseLayer:function(){
		var normalm = L.tileLayer.chinaProvider('TianDiTu.Normal.Map', {
	        maxZoom: 18,
	        minZoom: 2,
	        attribution: 'Map data &copy; <a href="http://www.tianditu.gov.cn">天地图</a>, Imagery © <a href="http://www.tianditu.gov.cn">天地图</a>'
	    }),
	    normala = L.tileLayer.chinaProvider('TianDiTu.Normal.Annotion', {
	        maxZoom: 18,
	        minZoom: 2
	    }),
	    imgm = L.tileLayer.chinaProvider('TianDiTu.Satellite.Map', {
	        maxZoom: 18,
	        minZoom: 2,
	        attribution: 'Map data &copy; <a href="http://www.tianditu.gov.cn">天地图</a>, Imagery © <a href="http://www.tianditu.gov.cn">天地图</a>'
	    }),
	    imga = L.tileLayer.chinaProvider('TianDiTu.Satellite.Annotion', {
	        maxZoom: 18,
	        minZoom: 2
	    });
		var normal = L.layerGroup([normalm, normala]),
		    image = L.layerGroup([imgm, imga]);

			    var baseLayers = {
			        "Normal": normal,
			        "Image":image
			    };
	    return baseLayers;
	},
	getGeoLayerStyle:function(){
		let that=this;
		return{
			weight:2,
			getOpacity:()=>0.7,
			getColor:function(feature){
				let type=feature.geometry.type;
				if(type==='Point')
					return that.color.point[feature.index];
				else if(type==='MultiPolygon'||type==='Polygon')
					return that.color.polygon[feature.index];
				else if(type==='MultiLineString'||type==='LineString')
					return that.color.line[feature.index];
			}
		};
	},
	getToolTipPos:function(geo){
		if(geo.type==='Point')
			return [geo.coordinates[0],geo.coordinates[1]];
		else if(geo.type==='Polygon'||geo.type==='MultiPolygon')
			return this.getCentroid([geo]);
		else if(geo.type==='MultiLineString'){
			let half1=parseInt(geo.coordinates.length/2);
			let half2=parseInt(geo.coordinates[half1].length/2);
			return [geo.coordinates[half1][half2][0],geo.coordinates[half1][half2][1]];
		}
		else if(geo.type==='LineString'){
			let half1=parseInt(geo.coordinates.length/2);
			return [geo.coordinates[half1][0],geo.coordinates[half1][1]];
		}
	},
	dataConversion:function(){
		var wkx = require('wkx');
		var buffer = require('buffer');
		//console.log(isArray(geometry.features));
	   	//let lat,lon;
		if(geometry.features !== undefined) {
			//console.log(geometry.features);
			for (var i in geometry.features) {
				//console.log(geometry.features[i]);
				var wkbLonlat = geometry.features[i].geometry;
				var hexAry = wkbLonlat.match(/.{2}/g);
				var intAry = [];
				for (var j in hexAry) {
					intAry.push(parseInt(hexAry[j], 16));
				}
				var buf = new buffer.Buffer(intAry);
				var geom = wkx.Geometry.parse(buf);
				geometry.features[i].geometry = geom.toGeoJSON();
			}
		}
		else {
			var wkbLonlat = geometry.geometry;
			var hexAry = wkbLonlat.match(/.{2}/g);
			var intAry = [];
			for (var i in hexAry) {
				intAry.push(parseInt(hexAry[i], 16));
			}
			var buf = new buffer.Buffer(intAry);
			var geom = wkx.Geometry.parse(buf);
			geometry.geometry = geom.toGeoJSON();
		}
	},
	getGeoJsonLayer:function(){
		let geoJsonLayer;
		let that=this;
		geoJsonLayer=L.geoJson(geometry,{
			onEachFeature:this.onEachFeature.bind(this),
			pointToLayer:function(geoJsonPoint,latlng){
				return L.circleMarker([latlng.lat,latlng.lng], {
				    radius: 10
				});
			},
			style:function(feature){
				return {
		            weight: that.getGeoLayerStyle().weight,
		            color: that.getGeoLayerStyle().getColor(feature),
		            opacity: that.getGeoLayerStyle().getOpacity(feature),
		            fillColor: that.getGeoLayerStyle().getColor(feature),
		            fillOpacity: that.getGeoLayerStyle().getOpacity(feature)
	        	};
	        }
		});
		return geoJsonLayer;
	},
	onEachFeature:function(feature, layer) {
		var popupContent = "";
		if (feature.properties) {
			popupContent += feature.properties.popupContent;
		}
		let that=this;
		layer.bindPopup(popupContent);
		layer.on({
			mouseover:this.highlightFeature.bind(this),
			mouseout:this.resetHighlight.bind(this)
		});
	},
	highlightFeature:function(e){
		let layer=e.target;
		layer.setStyle({
			weight:3,
			color:'#ffffff',
			fillOpacity:this.getGeoLayerStyle().getOpacity(layer.feature),
			opacity:0.7

		});
		layer.bringToFront();
	},
	resetHighlight:function(e){
		let layer=e.target;
		layer.setStyle({
			weight: this.getGeoLayerStyle().weight,
	        color: this.getGeoLayerStyle().getColor(layer.feature),
	        opacity:  this.getGeoLayerStyle().getOpacity(layer.feature),
	        fillColor: this.getGeoLayerStyle().getColor(layer.feature),
	        fillOpacity: this.getGeoLayerStyle().getOpacity(layer.feature)
		});
		layer.bringToBack();

	},
	getToolTipLayer:function(){
	   	let tooltipLayerGroup=L.layerGroup();
		if(geometry.features !== undefined) {
			for(let i in geometry.features){
				let feature=geometry.features[i];
		        if (feature.properties){
		        	let className='div-icon';
		            tooltipLayerGroup.addLayer(
		                L.marker(this.getToolTipPos(feature.geometry).reverse(),
		                      {
		                        icon:L.divIcon({className: className,html:"<i>"+feature.properties.popupContent+"</i>"})
		                      }
		                )
		            );
		        }
	    	}
		}
		else{
			let feature=geometry;
		    if (feature.properties){
	        	let className='div-icon';
	            tooltipLayerGroup.addLayer(
	                L.marker(this.getToolTipPos(feature.geometry).reverse(),
	                      {
	                        icon:L.divIcon({className: className,html:'<b><i><h3>'+String(feature.properties.popupContent)+'</h3></i></b>'})
	                      }
	                )
	            );
		    }
		}
		return tooltipLayerGroup;
	},
	num2Type(){
		return{
			0:'Normal',
			1:'Image'
		};
	},
	display:function(){
		this.dataConversion();
		this.center=this.getCentroid(geometry.features !== undefined?geometry.features:[geometry]);
		this.geoJsonLayer=this.getGeoJsonLayer();
		this.tooltipLayerGroup=this.getToolTipLayer();
		this.map=this.getMap({divId:this.divId,center:this.center.reverse(),zoom:this.zoom});
		//console.log(this.baseMapType);
		//console.log(this.num2Type()[this.baseMapType]);
		//console.log(this.baseMaps[this.num2Type()[this.baseMapType]]);
		this.baseMaps[this.num2Type()[this.baseMapType]].addTo(this.map);
		this.geoJsonLayer.addTo(this.map);
		if(this.showToolTipLayer===1)
			this.tooltipLayerGroup.addTo(this.map);
	    //L.control.layers(this.baseMaps, this.overlayMaps).addTo(this.map);
	}
};