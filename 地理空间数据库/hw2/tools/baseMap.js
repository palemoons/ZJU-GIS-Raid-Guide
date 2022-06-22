/*
Author: hwd

Date:2017-03-07

Revised by dhr on 2020-3-3

Description:Provide the superclass for geoMap,choroplethmap and heatmap
 */
function BaseMap(divId,zoom,baseMapType){
	this.divId=divId;
	this.zoom=zoom;
	this.center=null;
	this.baseMapType=baseMapType;
	this.baseMaps=this.getBaseLayer();
	this.map=null;
	this.color={
		point:['#e41a1c','#e7298a'],
		line:['#d95f02','#984ea3'],
		polygon:['#ff7f00','#ffff33']
	};
}
BaseMap.prototype={
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
	getMap:function({divId,center,zoom,layers}){
		return L.map(divId,{
		        center,
		        zoom,
		        layers
		    });
	},
	getBaseLayer:function(){
		// let streets=L.tileLayer('https://api.mapbox.com/v4/mapbox.streets/{z}/{x}/{y}.png?access_token=pk.eyJ1IjoiaWNlOHh4eHgiLCJhIjoiY2pibHh2NDA2NHBpYzJxcWdjdG1iMzA0eiJ9.cnZSGe7YbOpIU6CJEYg14g', {
	 //        maxZoom: 18,
	 //        attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, ' +
	 //        '<a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, ' +
	 //        'Imagery © <a href="http://mapbox.com">Mapbox</a>',
	 //        id: 'mapbox.light'
	 //    });
	 //    let dark=L.tileLayer('https://api.mapbox.com/styles/v1/mapbox/dark-v9/tiles/256/{z}/{x}/{y}?access_token=pk.eyJ1IjoiaWNlOHh4eHgiLCJhIjoiY2pibHh2NDA2NHBpYzJxcWdjdG1iMzA0eiJ9.cnZSGe7YbOpIU6CJEYg14g', {
	 //        maxZoom: 18,
	 //        attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, ' +
	 //        '<a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, ' +
	 //        'Imagery © <a href="http://mapbox.com">Mapbox</a>',
	 //        id: 'mapbox.dark'
	 //    });
	 //    var baseLayers = {
	 //        "Streets": streets,
	 //        "Dark":dark
	 //    };
	 //    return baseLayers;
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
				else if(type==='MultiLineString')
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
	}
};