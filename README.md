# Calculadora en notación posfija e infija

## Calculadora en notación posfija (dc.c) 

Se implementa un programa dc que permita realizar operaciones matemáticas. La calculadora leerá exclusivamente de entrada estándar (no toma argumentos por línea de comantos), interpretando cada línea como una operación en notación polaca inversa (también llamada notación posfija, en inglés reverse Polish notation); para cada línea, se imprimirá por salida estándar el resultado del cálculo.

Ejemplo de varias operaciones, y su resultado:
```
$ cat oper.txt
5 3 +
5 3 -
5 3 /
3 5 8 + +
3 5 8 + -
3 5 - 8 +
2 2 + +
0 1 ?
1 -1 0 ?
5 sqrt

$ ./dc < oper.txt
8
2
1
16
-10
6
ERROR
ERROR
-1
2
```


### Funcionamiento
+ Todas las operaciones trabajarán con números enteros, y devolverán números enteros.

+ El conjunto de operadores posibles es: suma (+), resta (-), multiplicación (*), división entera (/), raíz cuadrada (sqrt), exponenciación (^), logaritmo (log) en base arbitraria, y operador ternario (?).

+ Todos los operadores funcionan con dos operandos, excepto sqrt (toma un solo argumento) y el operador ternario (toma tres).

+ Tal y como se describe en la bibliografía enlazada, cualquier operación aritmética a op b se escribe en postfijo como a b op (por ejemplo,
se escribe en postfijo como 3 2 -).

+ Es útil modelar la expresión como una pila cuyo tope es el extremo derecho de la misma (por ejemplo en 3 2 -, el tope es -); entonces, se puede decir que lo primero que se desapila es el operador, y luego los operandos en orden inverso.

+ Para operaciones con un solo operando, el formato es obviamente a op (por ejemplo, 5 sqrt{:.nowrap}). Por su parte, para el operador ternario, el ordenamiento de los argumentos seguiría el mismo principio, transformándose a ? b : c en a b c ?.

Ejemplos (nótese que toda la aritmética es entera, y el resultado siempre se trunca):
```
20 11 - → 20-11 = 9
20 -3 / → 20/-3 = -6
20 10 ^ → 20^10 = 10240000000000
60 sqrt → √60 = 7
256 4 ^ 2 log → log₂(256⁴) = 32
1 -1 0 ? → 1 ? -1 : 0 = -1 (funciona como en C)
```

Formato de entrada
+ Cada línea de la entrada estándar representa una operación en su totalidad (produce un único resultado); y cada una de estas operaciones operación es independiente de las demás.

+ Los símbolos en la expresión pueden ser números, u operadores. Todos ellos estarán siempre separados por uno o más espacios; la presencia de múltiples espacios debe tenerse en cuenta a la hora de realizar la separación en tokens.

+ El resultado final de cada operación se imprime en una sola línea por salida estándar (stdout). En caso de error, se imprime —para esa operación— la cadena ERROR, también por salida estándar, y sin ningún tipo de resultado parcial. Tras cualquier error en una operación, el programa continuará procesando el resto de líneas con normalidad.

### Condiciones de error
El mensaje ERROR debe imprimirse como resultado en cualquiera de las siguientes situaciones:

1. Cantidad de operandos insuficiente (1 +)

2. Al finalizar la evaluación, queda más de un valor en la pila (por ejemplo 1 2 3 +, o + 2 3 -)

3. Errores propios de cada operación matemática, descritos a continuación:
+ División por 0
+ Raíz de un número negativo
+ Base del logaritmo menor a 2
+ Potencia con exponente negativo
+ Conversor desde notación infija

## Calculadora en notación posfija (infix.c)

Una vez implementada la calculadora en notación postfija, se agregó soporte para operaciones expresadas en notación infija, es decir, notación aritmética común.

Un segundo programa, infix, actúa como filtro de conversión, esto es: lee de entrada estándar operaciones en notación infija (una por línea), e imprime por salida estándar la representación en postfijo de la misma operación. Ejemplo:

```
$ cat arith.txt
3 + 5
5 - 3
8 / 2 + 1
9 - 2 * 4
(9-2) * 4
5 + 4 ^ 3 ^ 2

$ ./infix < arith.txt
3 5 +
5 3 -
8 2 / 1 +
9 2 4 * -
9 2 - 4 *
5 4 3 2 ^ ^ +

$ ./infix < arith.txt | ./dc
8
2
5
1
28
262149
```
La conversión se realiza mediante el algoritmo shunting yard.


### Manejo de cadenas (strutil.c)
Se describen a continuación las cuatro funciones a implementadas.

#### substr()
La función substr() permite obtener un prefijo de longitud de una cadena dada.

Por ejemplo, la llamada substr("Hola mundo", 6) devolvería la cadena "Hola m". El resultado debe ser una nueva cadena de memoria dinámica con dichos caracteres (y, por supuesto, el carácter de fin de cadena). Ejemplos de uso:
```
substr("Hola mundo", 6)         → "Hola m"
substr("Algoritmos", 30)        → "Algoritmos"
substr("", 2)                   → ""

const char* ejemplo = "Ejemplo";
substr(ejemplo, 2)              → "Ej"
substr(ejemplo + 4, 2)          → "pl"
```
Complejidad algorítmica: O(k), siendo k la longitud de la cadena dada. 


#### split()
La función split() divide una cadena en subcadenas, con una división por cada ocurrencia de un caracter de separación determinado. Por ejemplo, separando por comas:
```
split("abc,def,ghi", ',')  →  ["abc", "def", "ghi"]
```
En C, devolveremos el resultado como un arreglo dinámico de cadenas dinámicas terminado en NULL. Esto es:

// Ejemplo de arreglo dinámico de cadenas
```
char **strv = malloc(sizeof(char*) * 4);
strv[0] = strdup("abc");
strv[1] = strdup("def");
strv[2] = strdup("ghi");
strv[3] = NULL;
```

Complejidad algorítmica: O(n), siendo n la longitud de la cadena inicial. 

#### join()
La función join() es la inversa de split(). Concatena un arreglo de cadenas terminado en NULL mediante un caracter de separación:

// Ejemplo de uso de join
```
char **strv = split("abc,def,ghi", ',');
char *resultado = join(strv, ';');  // "abc;def;ghi"

char **palabras = split("Hola mundo", ' ');
char *otro_resultado = join(palabras, ',');  // "Hola,mundo"
```

Complejidad algorítmica: O(n), siendo n la longitud de la cadena resultante. 

Las pruebas del corrector automático proveen una indicación del comportamiento de join() (si bien todas las funciones deben correr en tiempo lineal, join() provee una dificultad de implementación mayor a split() o substr()).

Este sería un test con comportamiento lineal:
```
[ RUN      ] test_join.test_complejidad_10000
[       OK ] test_join.test_complejidad_10000 (7 ms)
[ RUN      ] test_join.test_complejidad_20000
[       OK ] test_join.test_complejidad_20000 (14 ms)
[ RUN      ] test_join.test_complejidad_30000
[       OK ] test_join.test_complejidad_30000 (17 ms)
[ RUN      ] test_join.test_complejidad_40000
[       OK ] test_join.test_complejidad_40000 (27 ms)
[ RUN      ] test_join.test_complejidad_50000
[       OK ] test_join.test_complejidad_50000 (33 ms)
[ RUN      ] test_join.test_complejidad_60000
[       OK ] test_join.test_complejidad_60000 (40 ms)
```

#### free_strv()
free_strv() libera la memoria asociada con un arreglo dinámico de cadenas dinámicas.


### Biblioteca de tipos (calc_helper.c)

Dos archivos (calc_helper.h y calc_helper.c) conforman una biblioteca de tipos y funciones útiles para la implementación de ambos programas, dc e infix.

La biblioteca proporciona:
+ Funciones para procesar texto (dc_split e infix_split)
+ Funciones para reconocer elementos válidos en la entrada (calc_parse, y sus tipos asociados)
+ Una pila de enteros, pilaint_t, implementada a partir del TDA Pila de punteros genéricos (útil para la versión no recursiva de dc)
+ Los archivos de la biblioteca están extensamente comentados, y en uno de los tipos se explica algunas ampliaciones que se pueden realizar para hacer aún más fácil la implementación de los programas. En particular, se sugiere agregar varios campos en el struct calc_oper para que, modificando la función calc_parse(), se informe de las distintas propiedades de cada operador.

## Formato de entrada
Cada línea de la entrada consistirá de una secuencia de tokens; cada uno de ellos podrá ser:
+ Uno de los cinco operadores aritméticos + - * / ^
+ Un paréntesis de apertura, (; o de cierre, )
+ Un número entero, no negativo y en base decimal

Se debe aceptar espacios en blanco en cualquier punto de la expresión, excepto entre los dígitos de un valor numérico.

Se garantiza, por último, que todas las expresiones de entrada estarán bien formadas: paréntesis balanceados, espaciado conforme a norma, etc.

#### Asociatividad y precedencia
Todos los operadores asocian por la izquierda, excepto la exponenciación, ^, que asocia por la derecha.

Por otra parte, ^ es el operador de mayor precedencia, seguido de * y / (ambos al mismo nivel); + y - son, ambos, los operadores de menor precedencia.

