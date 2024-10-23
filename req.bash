#!/bin/bash


HOST="lala.42.fr"
PORT="8080"

(
  echo "GET /lala/test.php/extrapath HTTP/1.1"
  echo "Host: $HOST"
  echo
) | telnet $HOST $PORT
