Contenido:

- README.txt: este archivo.
- Makefile: archivo con las dependencias para compilar los programas.
- colapri.h: declaracion de la cola de prioridades generica.
- colapri-arreglo.c y colapri-arreglo.h: implementacion del diccionario
  mediante arreglos.  Se entrega solo a modo de referencia.
- test-colapri-arreglo.c: test de prueba de la cola con arreglos.
- test-colapri-lista.c: test de prueba de la cola con listas.  Lo
  necesita para probar que su tarea funciona.
- colapri-lista.h: encabezado para nuevaColaPriLista.  No modifique
  este archivo.

Instrucciones:

- Para compilar y ejecutar el test de prueba con la implementacion basada
en arreglos:

  % make test-colapri-arreglo
  ...
  % ./test-colapri-arreglo
  ...
  Perfecto.  Todo funciona bien.
  %

- Programe su implementacion de la cola de prioridades basada en listas
enlazadas en el archivo colapri-lista.c.  Tome como referencia la
implementacion que se encuentra en colapri-arreglo.c.

- Note que su tarea se compilara usando el estandar c99.
La compilacion no debe dar ningun error o warning al compilar en anakena.

- Compile, ejecute y valide con el test de prueba para su implementacion
mediante:

  % make test-colapri-lista
  ...
  % ./test-colapri-lista
  ...
  Perfecto.  Todo funciona bien.
  %

Notese que el test de prueba termina en: Perfecto.  Todo funciona bien.
Si no aparece este mensaje al final, su tarea *no funciona*, no la
entregue.

- Durante la correccion de su tarea se verificara que funciona
correctamente usando el test-colapri-lista.c original (sin ninguna
modificacion).  Si su tarea no funciona, tendra la nota minima.
Ademas se inspeccionara el codigo para comprobar que uso listas enlazadas.
