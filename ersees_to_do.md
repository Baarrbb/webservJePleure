


Yanis to do:


### General

Si syntax d'une directive pas bonne on lance pas le serveur et log.

J'ai pas check pour toutes les directives mais jai l'impression que c'est le cas pour toutes. Si j'ai la directive verifier que elle n'est pas vide !

### Location 

Tu ne verifies pas si il y a bien qqc apres location donc si j'ai :

```
location
{
	...
}
```

a la place de 
```
location / 
{
	...
}done
```

je seg fault, retour du terminal :
```
terminate called after throwing an instance of 'Server::WrongExpression'
  what():  Index too far
zsh: IOT instruction (core dumped)  ./webserv
```

### Root

**SI** j'ai une directive root verifier que la directive n'est pas vide.
done

### Client body size:

Si tu pouvais renommer `client_body_limit_size` en `client_body_limit_size` ou `client_body_max_size`<br>
Parce que sur nginx `client_body_limit_size` c pour set la size du buffer qui lit le body et non pas
pour le limiter (oui je chipote mais ca devrait pas te prendre longtemps a changer).done

```
verifier la syntax :
	- que des chiffres
	- que des chiffres + ('k' ou 'K')
	- que des chiffres + ('m' ou 'M')
	- que des chiffres + ('g' ou 'G')
```

```
Si pb de syntax :
	Soit tu me renvoies une string empty (donc je fais comme si j'avais pas de limite)
	Soit on empeche demarrage serveur + log qui va avec. (Comme nginx (je penche plus pour ca))
```
```
Si possible :
	- me transformer la valeur en octets
	(ex: 1k = 1024)
	- me renvoyer un long
```

#### Question :
 - nginx a un body size par default. Est ce qu'on en setup un ?
 - php-cgi a aussi une limit sur la requete POST et une limite sur la taille des files uploads.<br>
Qu'est ce qu'on fait par rapport a ca et qu'est ce qu'on pourrait faire ?<br>
par ex chez moi php-cgi a `upload_max_filesize` a `2M` donc relativement petit.<br>
Si qqn veut upload un truc plus gros alors qu'il a mis client body size a 10M bh ca marchera pas.<br>
J'ai pas encore tester pcq du coup j'ai pas encore acces a client body size.



### error page : ca nn
```
verifier la syntax :
	default_error_page code file;
	(ex: error_page 404 /error.html)
	(ex: error_page 500 502 503 504 /error.hml)
```
```
Si pb de syntax :
	Soit tu me renvoies une string empty (donc je fais comme si j'avais pas d'error page)
	Soit on empeche demarrage serveur + log qui va avec. (Comme nginx (je penche plus pour ca))
```
```
Si possible :
	tu me renvoies un vecteur de int avec le ou les code(s).
	et string avec le file
```


### autoindex done

Verifier syntax. cad `on` ou `off` sinon log.

### index done
Verifier si bien qqc apres la directive sinon log.
