
const url = 'https://raw.githubusercontent.com/freeCodeCamp/ProjectReferenceData/master/GDP-data.json';

const h = 400;
const w = 1000;
const padding = 50;

const title = d3.select("body")
				.append("h2")
				.attr("id", "title")
				.text("United States GDP");

const svg = d3.select("body")
				.append("div")
				.append("svg")
				.attr("width", w)
				.attr("height", h);

fetch(url)
	.then(response => response.json())
	.then(data => {
		const dataset = data.data;

		const parseTime = d3.utcParse("%Y-%m-%d");

		const xScale = d3.scaleTime()
						 .domain([d3.min(dataset, (d) => parseTime(d[0])), d3.max(dataset, (d) => parseTime(d[0]))])
						 .range([padding, w - padding]);
		const xAxis = d3.axisBottom(xScale);
		svg.append("g").attr("id", "x-axis").attr("transform", "translate(0," + (h - padding) + ")").call(xAxis);
	   
		const yScale = d3.scaleLinear()
						 .domain([0, d3.max(dataset, (d) => d[1])])
						 .range([h - padding, padding]);
		const yAxis = d3.axisLeft(yScale);
		svg.append("g").attr("id", "y-axis").attr("transform", "translate(" + padding + ",0)").call(yAxis);
		
		svg.selectAll("rect").data(dataset).enter().append("rect")
		  .attr("class", "bar")
		  .attr("data-date", (d) => d[0])
		  .attr("data-gdp", (d) => d[1])
		  .attr("x", (d) => xScale(parseTime(d[0])))
		  .attr("y", (d) => yScale(d[1]))
		  .attr("width", (w - 2 * padding) / dataset.length)
		  .attr("height", (d) => h - padding - yScale(d[1]));
		
		var tooltip = d3.select("body")
						.append("div")
						.attr("id", "tooltip");
		
		svg.selectAll("rect")
		  .on("mouseover", function(){
			return tooltip.style("visibility", "visible");
		})
		  .on("mousemove", function(d){
			tooltip.attr("data-date", d[0]);
			tooltip.attr("data-gdp", d[1]);
			tooltip.style("top", "275px").style("left", (event.pageX)+ 25 +"px");
			var date = parseTime(d[0]);
			var year = date.getUTCFullYear();
			var month = date.getUTCMonth() + 1;
			var q = "Q";
			if (month === 1)
			   q += "1"
			else if (month === 4)
			   q += "2"
			else if (month === 7)
			  q += "3"
			else if (month === 10)
			   q += "4"
			const formatNb = d3.format(",.1f");
			tooltip.html(year + " " + q + '<br>' + "$" + formatNb(d[1]) + " Billion");
		})
		  .on("mouseout", function(){
		  return tooltip.style("visibility", "hidden");
		});

});
