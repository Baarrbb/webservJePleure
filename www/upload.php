<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Uploader un fichier</title>
</head>
<body>
    <h1>Uploader un fichier</h1>
    
    <form action="/get_upload.php" method="POST" enctype="multipart/form-data">
        <label for="file">Choisissez un fichier :</label><br><br>
        <input type="file" id="file" name="file" accept="*/*"><br><br>

        <input type="submit" value="Envoyer le fichier">
    </form>

    <p>Veuillez choisir un fichier à télécharger puis cliquez sur "Envoyer le fichier".</p>
</body>
</html>
