const url = "https://cdn.freecodecamp.org/testable-projects-fcc/data/tree_map/video-game-sales-data.json";

const w = 1000;
const h = 600;

const colors = [
	["2600", "#d2d2d2"],
	["Wii", "#5993c4"],
	["NES", " #b3e4a0"],
	["GB", "#fbc991"],
	["DS", "#c0d2ed"],
	["X360", "#f89a3a"],
	["PS3", "#65b257"],
	["PS2", "#d65753"],
	["SNES", "#cfc0dd"],
	["GBA", "#e394ce"],
	["PS4", "#a687cb"],
	["3DS", "#f9aeaa"],
	["N64", "#cdb0a9"],
	["PS", "#a07a70"],
	["XB", "#f5c5db"],
	["PC", "#999999"],
	["PSP", "#c9c94d"],
	["XOne", "#e2e1a3"]
];

function getCategoryColor(category) {
	for (let i = 0; i < colors.length; i++) {
		if (colors[i][0] === category) {
			return colors[i][1];
		}
	}
	return '#000000';
}

let tooltip = d3.select("#tooltip");

const title = d3.select("body")
				.append("h2")
				.attr("id", "title")
				.text("Video Game Sales");

const desc = d3.select("body")
				.append("div")
				.attr("id", "description")
				.text("Top 100 Most Sold Video Games Grouped by Platform");

const svg = d3.select("body")
				.append("div")
				.append("svg")
				.attr("width", w)
				.attr("height", h);

fetch(url)
	.then(response => response.json())
	.then (data => {

	const dataset = data;

	let root = d3.hierarchy(dataset, (node) => { return node.children; })
				.sum((node) => { return node.value; })
				.sort((node1, node2) => {
					return node2.value - node1.value;
	});

	d3.treemap()
		.size([w, h])(root);

	let block = svg.selectAll("g")
					.data(root.leaves())
					.enter()
					.append("g")
					.attr('transform', (game) => {
						return "translate(" + game.x0 + ", " + game.y0 + ")";
					});

	block.append("rect")
		.attr("class", "tile")
		.attr('width', function (d) { return d.x1 - d.x0; })
		.attr('height', function (d) { return d.y1 - d.y0; })
		.style("stroke", "black")
		.style("fill", (game) => {
			let category = game.data.category;
			if(category === '2600'){ return getCategoryColor("2600")}
			else if(category === 'Wii'){ return getCategoryColor("Wii")}
			else if(category === 'NES'){ return getCategoryColor("NES") }
			else if (category === "GB") { return getCategoryColor("GB")}
			else if(category === 'DS'){ return getCategoryColor('DS') }
			else if(category === 'X360'){ return getCategoryColor("X360") }
			else if(category === 'PS3'){ return getCategoryColor("PS3") }
			else if(category === 'PS2'){ return getCategoryColor("PS2") }
			else if(category === 'SNES'){ return getCategoryColor("SNES") }
			else if(category === 'GBA'){ return getCategoryColor("GBA") }
			else if(category === 'PS4'){ return getCategoryColor("PS4") }
			else if(category === '3DS'){ return getCategoryColor("3DS") }
			else if(category === 'N64'){ return getCategoryColor("N64") }
			else if(category === 'PS'){ return getCategoryColor("PS") }
			else if(category === 'XB'){ return getCategoryColor("XB") }
			else if(category === 'PC'){ return getCategoryColor("PC") }
			else if(category === 'PSP'){ return getCategoryColor("PSP") }
			else if(category === 'XOne'){ return getCategoryColor("XOne") }
	})
	.attr("data-name", (game) => { return game.data.name })
	.attr("data-category", (game) => { return game.data.category })
	.attr("data-value", (game) => { return game.data.value})
	.on("mouseover", function() {
		tooltip.transition()
			.style("visibility", "visible");
		d3.select(this)
			.style("stroke", "white");
	})
	.on("mousemove", (d) => {
		tooltip.attr("data-value", d.data.value)
			.style("top", event.pageY + "px")
			.style("left", (event.pageX)+ 20 +"px");
		tooltip.html(`Name : ` + d.data.name + `<br>Category : `
					+ d.data.category + `<br>Value : ` + d.data.value);
	})
	.on("mouseleave", function() {
		tooltip.transition()
				.style("visibility", "hidden");
		d3.select(this)
			.style("stroke", "black");
	});

	block.append("text")
		.attr("x", (d) => 5)
		.attr("y", (d) => 20)
		.text((game) => {
			return game.data.name;
		});


	const width = 20;
	const height = 20;
	const x1 = 25;
	
	d3.select("body")
		.append("div")
		.append("svg")
		.attr("id", "legend")
		.attr("width", 200)
		.attr("height", 200);

	const legend = d3.select("#legend");

	legend.append("g")
		.attr("id", "first");
	legend.select("#first")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 20)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("Wii"));
	legend.select("#first")
		.append("text")
		.attr("y", 35)
		.attr("x", x1)
		.text("Wii");

	legend.append("g")
		.attr("id", "second");
	legend.select("#second")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 50)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("DS"));
	legend.select("#second")
		.append("text")
		.attr("y", 65)
		.attr("x", x1)
		.text("DS");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 80)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("X360"));
	legend.append("text")
		.attr("y", 95)
		.attr("x", x1)
		.text("X360");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 110)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("GB"));
	legend.append("text")
		.attr("y", 125)
		.attr("x", x1)
		.text("GB");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 140)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("PS2"));
	legend.append("text")
		.attr("y", 155)
		.attr("x", x1)
		.text("PS2");


	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 170)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("3DS"));
	legend.append("text")
		.attr("y", 185)
		.attr("x", x1)
		.text("3DS");


	legend.append("g")
		.attr("id", "first");
	legend.select("#first")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 20)
		.attr("x", 100)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("SNES"));
	legend.select("#first")
		.append("text")
		.attr("y", 35)
		.attr("x", x1 + 100)
		.text("SNES");

	legend.append("g")
		.attr("id", "second");
	legend.select("#second")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 50)
		.attr("x", 100)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("PS"));
	legend.select("#second")
		.append("text")
		.attr("y", 65)
		.attr("x", x1 + 100)
		.text("PS");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 80)
		.attr("x", 100)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("N64"));
	legend.append("text")
		.attr("y", 95)
		.attr("x", x1 + 100)
		.text("N64");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 110)
		.attr("x", 100)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("PS4"));
	legend.append("text")
		.attr("y", 125)
		.attr("x", x1 + 100)
		.text("PS4");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 140)
		.attr("x", 100)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("PS3"));
	legend.append("text")
		.attr("y", 155)
		.attr("x", x1 + 100)
		.text("PS3");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 170)
		.attr("x", 100)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("NES"));
	legend.append("text")
		.attr("y", 185)
		.attr("x", x1 + 100)
		.text("NES");


	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 20)
		.attr("x", 200)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("GBA"));
	legend.append("text")
		.attr("y", 35)
		.attr("x", x1 + 200)
		.text("GBA");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 50)
		.attr("x", 200)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("XB"));
	legend.append("text")
		.attr("y", 65)
		.attr("x", x1 + 200)
		.text("XB");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 80)
		.attr("x", 200)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("PC"));
	legend.append("text")
		.attr("y", 95)
		.attr("x", x1 + 200)
		.text("PC");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 110)
		.attr("x", 200)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("2600"));
	legend.append("text")
		.attr("y", 125)
		.attr("x", x1 + 200)
		.text("2600");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 140)
		.attr("x", 200)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("PSP"));
	legend.append("text")
		.attr("y", 155)
		.attr("x", x1 + 200)
		.text("PSP");

	legend.append("g")
		.append("rect")
		.attr("width", width)
		.attr("height", height)
		.attr("y", 170)
		.attr("x", 200)
		.attr("class", "legend-item")
		.attr("fill", getCategoryColor("XOne"));
	legend.append("text")
		.attr("y", 185)
		.attr("x", x1 + 200)
		.text("XOne");

});
