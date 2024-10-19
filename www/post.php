<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test POST</title>
</head>
<body>
    <h1>Tester les requêtes POST</h1>

    <?php
    $message = "";
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $message = htmlspecialchars($_POST['data']); // Échapper les caractères spéciaux pour éviter XSS
    }
    ?>

    <form action="" method="POST" enctype="application/x-www-form-urlencoded">
        <label for="data">Données à envoyer :</label><br>
        <textarea id="data" name="data" rows="4" cols="50"></textarea><br><br>
        
        <input type="submit" value="Envoyer POST">
    </form>

    <?php if ($message): ?>
        <h2>Données soumises :</h2>
        <p><?php echo $message; ?></p>
    <?php endif; ?>

    <p>Cette page envoie une requête POST et affiche les données soumises ci-dessus.</p>
</body>
</html>
