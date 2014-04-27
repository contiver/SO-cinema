SO-cinema
=========
Primer trabajo para la materia 'Sistemas Operativos' del ITBA, primer cuatrimestre 2014.
Se optó por implementar un sistema de venta de entradas de cine, para lo cual se hizo un pequeño programa para que genere una (pseudo) base de datos, y luego se implemento clientes y servidores con los diferentes IPCs pedidos.

Compilación y ejecución
------
Cada directorio contiene un makefile para compilar con `make`, la implementación correspondiente. A su vez, haciendo un makefile en el directorio principal para ejecutar todos los makefiles y compilar todo de una.

Una vez compilados los ejecutables, correr una vez **db** dentro de *database/bin* para generar los archivos que representan peliculas, y luego correr la implementacion deseada. Siempre correr primero el servidor antes que los clientes.                                                                                                       

