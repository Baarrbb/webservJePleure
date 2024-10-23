<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Vérifier si un fichier a été envoyé
    if (isset($_FILES['file']) && $_FILES['file']['error'] === UPLOAD_ERR_OK) {
        // Détails du fichier
        $fileTmpPath = $_FILES['file']['tmp_name'];
        $fileName = $_FILES['file']['name'];
        $fileSize = $_FILES['file']['size'];
        $fileType = $_FILES['file']['type'];
        
        // Dossier de destination pour l'upload
        $uploadFolder = './uploads/';
        
        // Créer le dossier s'il n'existe pas
        if (!is_dir($uploadFolder)) {
            mkdir($uploadFolder, 0777, true);
        }

        // Chemin complet du fichier après l'upload
        $dest_path = $uploadFolder . $fileName;

        // Déplacer le fichier du dossier temporaire vers le dossier de destination
        if (move_uploaded_file($fileTmpPath, $dest_path)) {
            echo "Fichier uploadé avec succès : $fileName<br>";
            echo "Taille du fichier : $fileSize octets<br>";
            echo "Type de fichier : $fileType<br>";
			header("Location: /uploads/");
    		exit();
        } else {
            echo "Erreur lors de l'upload du fichier.";
        }
    } else {
        echo "Aucun fichier n'a été envoyé ou une erreur est survenue.";
    }
}
?>
