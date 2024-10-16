
const url = 'https://raw.githubusercontent.com/freeCodeCamp/ProjectReferenceData/master/global-temperature.json';

const colors = [
	"#4575b4", "#74add1", "#abd9e9", "#e0f3f8",
	"#ffffbf", 
	"#fee090", "#fdae61", "#f46d43", "#d73027", "#a50026"
];

const thresholds = [
	2.8, 3.9, 5.0, 6.1, 7.2, 8.3, 9.5, 10.6, 11.7, 12.8
];

const legendWidth = 400;
const legendHeight = 20;

const h = 400;
const w = 1000;
const padding = 80;


const title = d3.select("body")
				.append("h2")
				.attr("id", "title")
				.text("Monthly Global Land-Surface Temperature");

const desc = d3.select("body")
				.append("div")
				.attr("id", "description")
				.text("1753 - 2015: base temperature 8.66℃");

const svg = d3.select("body")
				.append("div")
				.append("svg")
				.attr("width", w)
				.attr("height", h);

fetch(url)
	.then(response => response.json())
	.then (data => {

		const dataset = data.monthlyVariance;
		const temp = data.baseTemperature;

		const xScale = d3.scaleLinear()
				.domain([
					d3.min(dataset, (d) => d.year),
					d3.max(dataset, (d) => d.year) + 1
				])
				.range([padding, w - padding]);
		const xAxis = d3.axisBottom(xScale)
				.tickFormat(d3.format('d'))
				.ticks((d3.max(dataset, (d) => d.year) - d3.min(dataset, (d) => d.year)) / 10);
		svg.append("g").attr("id", "x-axis").attr("transform", "translate(0," + (h - padding) + ")").call(xAxis);

		const yScale = d3.scaleTime()
				.domain([
					new Date(0, 0, 0, 0, 0, 0, 0),
					new Date(0, 12, 0, 0, 0, 0, 0)
				])
				.range([padding, h - padding]);
		const yAxis = d3.axisLeft(yScale).tickFormat(d3.timeFormat('%B'));
		svg.append("g").attr("id", "y-axis").attr("transform", "translate(" + padding + ",0)").call(yAxis);

		svg.selectAll("rect").data(dataset).enter()
			.append("rect")
			.attr("class", "cell")
			.attr("data-month", (d) => d.month - 1)
			.attr("data-year", (d) => d.year)
			.attr("data-temp", (d) => temp + d.variance)
			.attr("x", (d) => xScale(d.year))
			.attr("y", (d) => yScale(new Date(0, (d.month - 1), 0, 0, 0, 0, 0)))
			.attr("width", (d) => {
				const nbYear = d3.max(dataset, (d) => d.year) - d3.min(dataset, (d) => d.year);
				const l = (w - (2 * padding)) / nbYear;
				return l;
			})
			.attr("height", (h - (2 * padding)) / 12)
			.attr("fill", (d) => {
				const t = temp + d.variance;
				if (t < 2.8)
					return "#313695";
				else if (t >= 2.8 && t < 3.9)
					return colors[0];
				else if (t >= 3.9 && t < 5.0)
					return colors[1];
				else if (t >= 5.0 && t < 6.1)
					return colors[2];
				else if (t >= 6.1 && t < 7.2)
					return colors[3];
				else if (t >= 7.2 && t < 8.3)
					return colors[4];
				else if (t >= 8.3 && t < 9.5)
					return colors[5];
				else if (t >= 9.5 && t < 10.6)
					return colors[6];
				else if (t >= 10.6 && t < 11.7)
					return colors[7];
				else if (t >= 11.7 && t < 12.8)
					return colors[8];
				else if (t >= 12.8)
					return colors[9];
				else 
					return "white";
			});


		const colorScale = d3.scaleThreshold()
			.domain(thresholds)
			.range(colors);

		const legend = svg.append("g")
			.attr("id", "legend")
			.attr("transform", "translate(" + (w - legendWidth - padding) + "," + (h - padding + 40) + ")");

		const legendXScale = d3.scaleLinear()
			.domain([d3.min(thresholds), d3.max(thresholds)])
			.range([0, legendWidth]);

		legend.selectAll("rect")
			.data(thresholds)
			.enter()
			.append("rect")
			.attr("x", (d, i) => legendXScale(thresholds[i]))
			.attr("y", 0)
			.attr("width", (d, i) => {
					if (i === thresholds.length - 1) {
							return legendWidth - legendXScale(thresholds[i]);
					} else {
							return legendXScale(thresholds[i + 1]) - legendXScale(thresholds[i]);
					}
			})
			.attr("height", legendHeight)
			.attr("fill", (d, i) => colors[i]);

		const legendAxis = d3.axisBottom(legendXScale)
			.tickValues(thresholds)
			.tickFormat(d3.format(".1f"));

		legend.append("g")
			.attr("transform", "translate(0," + legendHeight + ")")
			.call(legendAxis);


		var tooltip = d3.select("body")
				.append("div")
				.attr("id", "tooltip");
		svg.selectAll(".cell")
			.on("mouseover", function(){
				tooltip.style("visibility", "visible")
				d3.select(this)
				.style("stroke", "black");
			})
			.on("mousemove", function(d){
				tooltip.attr("data-year", d.year);
				tooltip.style("top", event.pageY + "px").style("left", (event.pageX)+ 20 +"px");
				const month = [ [1, "Janvier"],
							[2, "Fevrier"],
							[3, "Mars"],
							[4, "Avril"],
							[5, "Mai"],
							[6, "Juin"],
							[7, "Juillet"],
							[8, "Aout"],
							[9, "Septembre"],
							[10, "Octobre"],
							[11, "Novembre"],
							[12, "Decembre"] ];
				const formatNb = d3.format(",.1f");
				const sgn = d.variance >= 0 ? "+" : "";
				tooltip.html(d.year + " - " + month[d.month - 1][1] + "<br>"
						+ formatNb(temp + d.variance) + "°C<br>" + sgn + formatNb(d.variance) + "°C");
			})
			.on("mouseleave", function(){
				tooltip.style("visibility", "hidden")
				d3.select(this)
				.style("stroke", "none");
			});


});
