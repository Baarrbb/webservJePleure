<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Delete a File</title>
    <script>
        function deleteFile() {
            var fileSelect = document.getElementById('fileSelect');
            var selectedFile = fileSelect.value;

            if (selectedFile) {
                var xhr = new XMLHttpRequest();
                xhr.open("DELETE", "/uploads/" + encodeURIComponent(selectedFile), true);
                xhr.onload = function () {
                    if (xhr.status === 200) {
                        alert("File deleted successfully: " + selectedFile);
                        location.reload();  // Reload the page after deletion
                    } else {
                        alert("Failed to delete file. Status: " + xhr.status + " " + xhr.statusText);
                    }
                };
                xhr.onerror = function () {
                    alert("Error occurred while trying to delete the file.");
                };
                xhr.send();
            } else {
                alert("Please select a file to delete.");
            }
        }
    </script>
</head>
<body>
	<h2><a href="/upload.php">Vers upload</a></h2>

    <h1>Delete a File</h1>

    <form>
        <label for="fileSelect">Select file to delete:</label>
        <select id="fileSelect" name="file">
            <?php
            // PHP pour lister les fichiers du répertoire uploads
            $directory = './uploads';

            // Vérifier que le répertoire existe
            if (is_dir($directory)) {
                $files = scandir($directory);  // Récupérer les fichiers

                // Afficher les fichiers dans la liste déroulante
                foreach ($files as $file) {
                    if ($file !== '.' && $file !== '..') {
                        echo '<option value="' . htmlspecialchars($file) . '">' . htmlspecialchars($file) . '</option>';
                    }
                }

                if (count($files) <= 2) {
                    echo '<option disabled>No files available</option>';
                }
            } else {
                echo '<option disabled>Uploads directory not found</option>';
            }
            ?>
        </select>
        <button type="button" onclick="deleteFile()">Delete</button>
    </form>
</body>
</html>