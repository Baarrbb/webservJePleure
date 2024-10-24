<?php
// Lire tout ce qui a été envoyé dans la requête
$request_body = file_get_contents('php://input');

// Afficher les headers de la requête
echo "Request Headers:\n";
foreach (getallheaders() as $name => $value) {
    echo "$name: $value\n";
}

// Afficher le corps de la requête
echo "\nRequest Body:\n";
echo $request_body;
