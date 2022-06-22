/*
Author: hwd

Date:2017-03-07

Description:Provide  ChoroplethMap
 */
function ChoroplethMap(divId,zoom,baseMapType){
	BaseMap.call(this,divId,zoom,baseMapType);
	this.geoJsonLayer=null;
	this.toolTipLayerGroup=null;
	this.overlayMaps=null;
	[this.minVal,this.maxVal]=d3.extent(geometry.features,d=>d.properties.num);
	this.startColor='#ffffff';
	this.endColor='#d7191c';
	this.colorMap=d3.scaleLinear().domain([this.minVal,this.maxVal]).range([this.startColor, this.endColor]);
	this.opacityMap=d3.scaleLinear().domain([this.minVal,this.maxVal]).range([0.25, 1]);

	this.colorMapLayer=this.getColorMapLayer();
}
var extension={
	display:function(){
		this.dataConversion();
		this.center=this.getCentroid(geometry.features !== undefined?geometry.features:[geometry]);
		this.geoJsonLayer=this.getGeoJsonLayer();
		this.tooltipLayerGroup=this.getToolTipLayer();
		this.overlayMaps={
			"ChoroplethMap":this.geoJsonLayer,
			"Description":this.tooltipLayerGroup
		};
		this.map=this.getMap({divId:this.divId,center:this.center.reverse(),zoom:this.zoom,layers:[this.baseMaps[(this.num2Type())[this.baseMapType]],this.geoJsonLayer]});
	    L.control.layers(this.baseMaps, this.overlayMaps).addTo(this.map);
	    this.colorMapLayer.addTo(this.map);
	},
	getGeoLayerStyle:function(){
		let that=this;
		return{
			weight:3,
			getOpacity:function(feature){
				return that.opacityMap(feature.properties.num);
			},
			getColor:function(feature){

				//console.log(feature.properties.num);
				return that.colorMap(feature.properties.num);
			}
		};
	},
	getColorMapHtml:function(){
		var margin = {top: 5, right: 5, bottom: 5, left: 5},
			    width = 60,
			    height = 100;
		var widthLegend = 60;
		var key = d3.select(".colorMapTemplate")
		    .append("svg")
		    .attr("width", widthLegend)
		    .attr("height", height + margin.top + margin.bottom);
		var legend = key
		    .append("defs")
		    .append("linearGradient")
		    .attr("id", "gradient")
		    .attr("x1", "100%")
		    .attr("y1", "0%")
		    .attr("x2", "100%")
		    .attr("y2", "100%")
		    .attr("spreadMethod", "pad");

		    legend
		    .append("stop")
		    .attr("offset", "0%")
		    .attr("stop-color", this.endColor)
		    .attr("stop-opacity", 1);

		    legend
		    .append("stop")
		    .attr("offset", "100%")
		    .attr("stop-color", this.startColor)
		    .attr("stop-opacity", 1);

		    key.append("rect")
		    .attr("width", widthLegend/2-10)
		    .attr("height", height)
		    .style("fill", "url(#gradient)")
		    .attr("transform", "translate(0," + margin.top + ")");
		var y = d3.scaleLinear()
		    .range([height, 0])
		    .domain([this.minVal, this.maxVal]);

	    var yAxis = d3.axisRight()
	    	.scale(y);
	    key.append("g")
		    .attr("class", "y axis") 
		    .attr("transform", "translate(21," + margin.top + ")")
		    .call(yAxis);
		return d3.select('.colorMapTemplate').html();
	},
	getColorMapLayer:function(){
		var panel = L.control({position: 'bottomright'});
		let that=this;
		panel.onAdd = function () {
			L.DomUtil.create('div', 'colorMapTemplate',L.DomUtil.get(that.divId));
		    this._div = L.DomUtil.create('div', 'colorMap',L.DomUtil.get(that.divId)); // create a div with a class "info"
		    this.update();
		    return this._div;
		};
		// method that we will use to update the control based on feature properties passed
		panel.update = function (props) {
			
		    this._div.innerHTML =that.getColorMapHtml();
		    d3.select('.colorMapTemplate').html('');
		};
		return panel;

	}
};
ChoroplethMap.prototype=_.extend(new BaseMap(),extension);