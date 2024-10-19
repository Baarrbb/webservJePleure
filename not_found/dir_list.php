
<?php
	// Récupérer le chemin du répertoire que tu veux lister (via l'URL)
	$dir = $_SERVER['TARGET_DIR'];

	if (substr($dir, 0, 1) === '.') {
		$dir = '.' . $dir;  // Ajouter un deuxième "." devant
	}

	// Vérifier si c'est un répertoire valide
	if (is_dir($dir)) {
		// Ouvrir et lire le répertoire
		$files = scandir($dir);

		if ($files === false) {
			echo "Impossible d'ouvrir le répertoire.";
			exit;
		}

		echo "<html><head><title>Index of " . htmlspecialchars($_SERVER['TARGET_DIR']) . "</title></head><body>";
		echo "<h1>Index of " . htmlspecialchars($_SERVER['TARGET_DIR']) . "</h1>";
		echo "<ul>";

		foreach ($files as $file) {
			if ($file == "." || $file == "..") continue;
			$filePath = $_SERVER['TARGET_DIR'] . $file;
			if (is_dir($dir . '/' . $file)) {
				echo '<li><strong><a href="' . htmlspecialchars($file) . '/">' . htmlspecialchars($file) . '/</a></strong></li>';
			} else {
				echo '<li><a href="' . htmlspecialchars($file) . '">' . htmlspecialchars($file) . '</a></li>';
			}
		}

		echo "</ul></body></html>";
	} else {
		echo "Le répertoire n'existe pas.";
		exit;
	}
?>
