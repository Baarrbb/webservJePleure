
const urlEduc = "https://cdn.freecodecamp.org/testable-projects-fcc/data/choropleth_map/for_user_education.json";
const urlCounty = "https://cdn.freecodecamp.org/testable-projects-fcc/data/choropleth_map/counties.json";

const w = 950;
const h = 800;

let tooltip = d3.select("#tooltip");

let countybis;

const colors = [ "#e6f5e0",
				"#cae8bf",
				"#a7d89b",
				"#7fc376",
				"#55aa5f",
				"#3c8b48",
				"#266e31" ];

const svg = d3.select("body")
				.append("svg")
				.attr("width", w)
				.attr("height", h);

fetch(urlCounty)
	.then(response => response.json())
	.then(data => {
		let countyData = data;

		fetch(urlEduc)
			.then(response => response.json())
			.then(data => {
				const educData = data;

				countyData = topojson.feature(countyData, countyData.objects.counties).features;

				svg.selectAll("path")
					.data(countyData)
					.enter()
					.append("path")
					.attr("d", d3.geoPath())
					.attr("class", "county")
					.attr("fill", (d) => {
						let county = educData.find( (county) => {
							return county.fips === d.id;
						})
						let perc = county.bachelorsOrHigher;
						if (perc <= 3) return "white";
						else if (perc > 3 && perc < 12) return colors[0];
						else if (perc >= 12 && perc < 21) return colors[1];
						else if (perc >= 21 && perc < 30) return colors[2];
						else if (perc >= 30 && perc < 39) return colors[3];
						else if (perc >= 39 && perc < 48) return colors[4];
						else if (perc >= 48 && perc < 57) return colors[5];
						else if (perc >= 57) return colors[6];
					})
					.attr("data-fips", (d) => { return d.id; })
					.attr("data-education", (d) => {
						let county = educData.find( (county) => {
							return county.fips === d.id;
						})
						return county.bachelorsOrHigher;
					})
					.on("mouseover", function() {
						tooltip.transition()
							.style("visibility", "visible");
						d3.select(this)
							.style("stroke", "black");
					})
					.on("mousemove", (d) => {
						let county = educData.find((county) => {
							return county.fips === d.id;
						})
						tooltip
							.attr("data-education", county.bachelorsOrHigher)
							.style("top", event.pageY + "px")
							.style("left", (event.pageX)+ 20 +"px")
							.text(county.area_name + ", " + county.state + " : " + county.bachelorsOrHigher + "%");
					})
					.on("mouseleave", function() {
						tooltip.transition()
								.style("visibility", "hidden");
						d3.select(this)
							.style("stroke", "none");
					});
				
				const width = 20;
				const height = 20;
				
				svg.append("svg").attr("id", "legend");

				svg.select("#legend")
					.append("g")
					.attr("id", "first");
				svg.select("#first")
					.append("rect")
					.attr("width", width)
					.attr("height",height)
					.attr("y", 645)
					.attr("fill", colors[0]);
				svg.select("#first")
					.append("text")
					.attr("y", 660)
					.attr("x", 25)
					.text("> 3%");

				svg.select("#legend")
					.append("g")
					.attr("id", "second");
				svg.select("#second")
					.append("rect")
					.attr("width", width)
					.attr("height", height)
					.attr("y", 665)
					.attr("fill", colors[1]);
				svg.select("#second")
					.append("text")
					.attr("y", 680)
					.attr("x", 25)
					.text("> 12%");

				svg.select("#legend")
					.append("g")
					.attr("id", "third");
				svg.select("#third")
					.append("rect")
					.attr("width", width)
					.attr("height", height)
					.attr("y", 685)
					.attr("fill", colors[2]);
				svg.select("#third")
					.append("text")
					.attr("y", 700)
					.attr("x", 25)
					.text("> 21%");

				svg.select("#legend")
					.append("g")
					.attr("id", "fourth");
				svg.select("#fourth")
					.append("rect")
					.attr("width", width)
					.attr("height", height)
					.attr("y", 705)
					.attr("fill", colors[3]);
				svg.select("#fourth")
					.append("text")
					.attr("y", 720)
					.attr("x", 25)
					.text("> 30%");

				svg.append("rect")
					.attr("width", width)
					.attr("height", height)
					.attr("y", 725)
					.attr("fill", colors[4]);
				svg.append("text")
					.attr("y", 740)
					.attr("x", 25)
					.text("> 39%");

				svg.append("rect")
					.attr("width", width)
					.attr("height", height)
					.attr("y", 745)
					.attr("fill", colors[5]);
				svg.append("text")
					.attr("y", 760)
					.attr("x", 25)
					.text("> 48%");

				svg.append("rect")
					.attr("width", width)
					.attr("height", height)
					.attr("y", 765)
					.attr("fill", colors[6]);
				svg.append("text")
					.attr("y", 780)
					.attr("x", 25)
					.text("> 57%");
			})
});

