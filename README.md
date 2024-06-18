# TPFPI
Trabajo Practico Final de Programacion Imperativa 
Autores: Nahuel Ignacio Prado y Matias Cuneo Gima
Alumnos del Instituto Tecnologico de Buenos Aires (ITBA)

## Archivos

### bikeSharing.c:
Lee, analiza y procesa los datos proporcionados realizando las querys 1, 2, 3 y 4.

## Librerías:

### treeADT:
Esta libreria proporciona un arbol binario de busqueda, el cual almacena en sus nodos una id comparable y un elemento de tipo generico.

### stationADT:
Esta librería cuenta con funciones para crear, almacenar, procesar y liberar, conjuntos de estaciones y alquileres de biciletas.

### read:
Esta libreria sirve para lectura de archivos CSVs y utiliza stationsADT cargado de datos.
Tip: Se pueden modificar constantes simbolicas en el .h para lectura de otro tipo de archivos separados por otro delimitador, tambien se asume que el DAYLIGHT SAVING TIME es desconocido, en caso de querer modificarlo cambie la constante en el .h como se menciona en el comentario que esta a la derecha del define 

### htmlTable:
Esta libreria permite la creacion de archivos html con formato de tabla.

### writeCSV:
Esta libreria permite reducir la complejidad al front para crear cada querry.
Tip: Se puede modificar el separador de datos desde el .h, es una constante simbolica

### querys:
Esta libreria usa stationADT para poder obtener los datos ya procesados y apartir de htmlTable y writeCSV da forma a los archivos solicitados en cada querry.

## Compilación

#### CUIDADO AL COMPILAR AL EJECUTAR MAKEFILE SE BORRARÁN TODOS LOS ARCHIVOS .o EN EL DIRECTORIO.

Para compilar lo unico que hay que hacer es ejecutar el comando make y se generaran dos archivos ejecutables
bikeSharingMON y bikeSharingNYC Cada uno corresponde con una ciudad, como se indica en su nombre.

```sh    
    #Dentro del directorio del proyecto
    make
```

## Ejecución

Finalmente para ejecutar cada programa debe se pasar como minimo dos parámetros, el primero es un archivo .csv con los datos que contengan los alquileres, el segundo contiene los datos sobre las estaciones. Las funciones de lectura esperan que se pasen los parametros con los formatos acordes y que se utilizen los archivos con formato correspondiente. Ademas la cuarta query puede o no recibir un tercer y cuarto parametro el cual debe ser un año. Si se ingresa solo el tercer parametro este indicara una fecha de inicio a tener en cuenta para la cuarta query. Mientras que si se ingresa el cuarto paramentro este indicara una fecha de tope, de esta forma se genera un rango de años validos para la query cuatro.

####Comando para Montreal:
```sh
    ./bikeSharingMON alquileresMON.csv estacionesMON.csv [OPTION]AñoInicio [OPTION]AñoFin
```
####Comando para Nueva York:
```sh
    ./bikeSharingNYC alquileresNYC.csv estacionesNYC.csv [OPTION]AñoInicio [OPTION]AñoFin
``` 

#### Advertencia: 

#### si se ingresan mas de cuatro parametros saldra un mensaje de advertencia indicando que podria estar ingresando parametros erroneos o extras

Si se desea desativar este mensaje modifique la constante simbolica del stationADT.h "WARNING_FLAG", cambie el 1 por un 0 
