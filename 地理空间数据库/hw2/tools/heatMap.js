/*
Author: hwd

Date:2017-03-07

Description:Provide  HeatMap
 */
function HeatMap(divId,zoom,baseMapType){
	BaseMap.call(this,divId,zoom,baseMapType);
	this.heatMapLayer=null;
	this.overlayMaps=null;
	
}
var extension={
	display:function(){
		this.dataConversion();
		this.center=this.getCentroid(geometry.features !== undefined?geometry.features:[geometry]);
		this.getHeatMapLayer();
		console.log(this.baseMaps);
		this.map=this.getMap({divId:this.divId,center:this.center.reverse(),zoom:this.zoom,layers:[this.baseMaps[(this.num2Type())[this.baseMapType]],this.heatMapLayer]});
		this.overlayMaps = {
	        "HeatMap":this.heatMapLayer
		};
	    L.control.layers(this.baseMaps, this.overlayMaps).addTo(this.map);
	},
	getHeatMapLayer:function(){
		let heatMapData;
		if(geometry.features.every(d=>d.properties.num===null))
			heatMapData=_.map(geometry.features,d=>[d.geometry.coordinates[1],d.geometry.coordinates[0]]);
		else{
			let [minVal,maxVal]=d3.extent(geometry.features,d=>d.properties.num);
			heatMapData=_.map(geometry.features,d=>[d.geometry.coordinates[1],d.geometry.coordinates[0],1/(maxVal-minVal)*(d.properties.num-minVal)]);
		}
		var options={
			"radius": 25,
			"minOpacity":0.3
		};
		this.heatMapLayer=L.heatLayer(heatMapData,options);
	}
};
HeatMap.prototype=_.extend(new BaseMap(),extension);