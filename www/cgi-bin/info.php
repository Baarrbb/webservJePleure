<?php
	header('Content-Type: text/html');
	echo "<html><head><title>CGI Info</title></head><body>";
	echo "<h1>PHP CGI Test</h1>";

	// Affichage des variables d'environnement
	echo "<h2>Environment Variables</h2>";
	echo "<ul>";
	foreach ($_SERVER as $key => $value) {
		echo "<li><b>$key</b>: $value</li>";
	}
	echo "</ul>";

	echo "</body></html>";
?>
