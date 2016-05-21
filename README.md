# Trabajo para redes de computadores II

Implementación de una aplicación multicast.
El servidor envía mensajes a una dirección multicast IPv6 y el cliente los recibe.

## Instrucciones

Para compilar :
```
$ make
```

Se puede ejecutar sin argumentos de entrada o con ellos, en caso de utilizarlos serán de la siguiente forma:

DireccionMulticast%Puerto%Interfaz

Debe utilizarse el separador '%' entre argumentos. Ejemplo de ejecución del servidor:
```
$ ./server FF15::35%6789%eth0
```
En caso de no especificarse argumentos se utilizarán los definidos por defecto en includes.h
