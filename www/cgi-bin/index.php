<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Index Page</title>
	</head>
	<body>
		<h1>Welcome to the Index Page</h1>
		<p>
			<a href="info.php">Go to Info Page</a>
		</p>
		<?php
			phpinfo(INFO_GENERAL);
			phpinfo();
			echo "BONJOUR\n";
		?>
	</body>
</html>
