
const url = 'https://raw.githubusercontent.com/freeCodeCamp/ProjectReferenceData/master/cyclist-data.json';

const h = 400;
const w = 1000;
const padding = 50;

const title = d3.select("body")
				.append("h2")
				.attr("id", "title")
				.text("Doping in Professional Bicycle Racing");

const svg = d3.select("body")
				.append("div")
				.append("svg")
				.attr("width", w)
				.attr("height", h);

fetch(url)
	.then(response => response.json())
	.then(data => {
		const dataset = data;

		const xScale = d3.scaleLinear()
						.domain([
							d3.min(dataset, (d) => d.Year) - 1,
							d3.max(dataset, (d) => d.Year) + 1
						])
						.range([padding, w - padding]);
		const xAxis = d3.axisBottom(xScale).tickFormat(d3.format('d'));
		svg.append("g").attr("id", "x-axis").attr("transform", "translate(0," + (h - padding) + ")").call(xAxis);
	
		const yScale = d3.scaleTime()
						.domain([
							d3.min(dataset, (d) => new Date(d.Seconds * 1000)),
							d3.max(dataset, (d) => new Date(d.Seconds * 1000))
						])
						.range([padding, h - padding]);
		const yAxis = d3.axisLeft(yScale).tickFormat(d3.timeFormat("%M:%S"));
		svg.append("g").attr("id", "y-axis").attr("transform", "translate(" + padding + ",0)").call(yAxis);

		svg.selectAll("circle").data(dataset).enter().append("circle")
		.attr("class", "dot")
		.attr("data-xvalue", (d) => d.Year)
		.attr("data-yvalue", (d) => new Date(d.Seconds * 1000))
		.attr("cx", (d) => xScale(d.Year))
		.attr("cy", (d) => yScale(d.Seconds * 1000))
		.attr("r", 5)
		.attr("fill", (d) => d.Doping === "" ? "orange" : "blue")
		.attr("stroke", "black")
		.attr("opacity", 0.65);

		var legend = d3.select("body").append("div")
						.attr("id", "legend")
						.html("Orange : No doping allegations <br> Blue : Riders with doping allegations");

		var tooltip = d3.select("body")
						.append("div")
						.attr("id", "tooltip");
		svg.selectAll("circle")
			.on("mouseover", function(){
			return tooltip.style("visibility", "visible");
		})
			.on("mousemove", function(d){
			tooltip.attr("data-year", d.Year);
			tooltip.style("top", event.pageY + "px").style("left", (event.pageX)+ 20 +"px");
			tooltip.html(d.Name + " : " + d.Nationality + "<br> Year : " + d.Year + ", Time : " + d.Time
				+ "<br><br>" + d.Doping );
		})
			.on("mouseout", function(){
			return tooltip.style("visibility", "hidden");
		});

});
