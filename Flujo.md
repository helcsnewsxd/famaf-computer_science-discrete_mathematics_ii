# Flujos y Networks

## Notaciones

  - Si $P$ es una propiedad que puede ser verdadera o falsa, $[P]$ denota el número $1$ si $P$ es verdadera y $0$ si $P$ es falsa
  - Si $g$ es una función definida en los lados y $A$ y $B$ son subconjuntos de vértices, entonces $g(A,B)$ denotará la suma $g(A,B)=\sum_{xy}[x\in A][y\in B][\overrightarrow{xy}\in E]g(\overrightarrow{xy})$
    - Luego, tenemos que si $\forall \overrightarrow{xy}\in E$, $g(\overrightarrow{xy})\leq f(\overrightarrow{xy})$, entonces $g(A,B)\leq f(A,B)\forall A,B\in V$
  - Dada una función $g$ sobre lados y un vértice $x$, definimos
    - $out_g(x)=g(\lbrace x\rbrace,\Gamma^+)=\sum_{y}[y\in\Gamma^+]g(\overrightarrow{xy})$
      - Todo lo que sale de $x$ por medio de $g$
    - $in_g(x)=g(\Gamma^-,\lbrace x\rbrace)=\sum_{y}[y\in\Gamma^-]g(\overrightarrow{yx})$
      - Todo lo que entra a $x$ por medio de $g$

## Definición de Network

Un Network es un grafo dirigido con pesos positivos en los lados, es decir, un triple $(V,E,c)$ donde $(V,E)$ es un grafo dirigido y $c:E\rightarrow\R_{\gt 0}$
  - Se llamará $c(\overrightarrow{xy})$ como la capacidad del lado $\overrightarrow{xy}$

## Definición de Flujo

Dado un network $(V,E,c)$ y un par de vértices $s,t\in V$, un _flujo_ de $s$ a $t$ es una función $f:E\rightarrow\R$ con las siguientes propiedades:
  - Factibilidad: $\forall \overrightarrow{xy}\in E$, $0\leq f(\overrightarrow{xy})\leq c(\overrightarrow{xy})$
    - No se transporta una cantidad negativa de un bien ni sobre la capacidad de transporte de un lado
  - Conservación: $\forall x\in V-\lbrace s,t\rbrace$, $in_f(x)=out_f(x)$
    - El network no tiene pérdidas, es decir, cualquier vértice que no sea $s$ ni $t$ no consume ni produce bienes
  - $s$ es productor o fuente (source): $out_f(s)\geq in_f(s)$
  - $t$ es consumidor o resumidero (sink): $in_f(t)\geq out_f(t)$

## Valor de un flujo

Dado un network $(V,E,c)$, el valor de un flujo $f$ de $s$ a $t$ es: $v(f)=out_f(s)-in_f(s)$
  - Equivalente a $in_f(t)-out_f(t)$
  - El valor de un flujo es igual a la cantidad neta de bienes producidos

## Flujos maximales

Dado un network $N$ y vértices $s,t$, un flujo maximal de $s$ a $t$ (o _Max flow_) es un flujo $f$ de $s$ a $t$ tal que $v(g)\leq v(f)$ para todo flujo $g$ de $s$ a $t$

Ahora, el problema que queremos resolver es: "Dado un network, encontrar un flujo maximal en tiempo polinomial"

### Criterios para maximalidad

#### Condiciones suficientes pero no necesarias

  - Sea $f$ flujo en un network $N$ tal que $v(f)=c(\lbrace s\rbrace,V)$, entonces $f$ es maximal
  - Sea $f$ flujo en un network $N$ tal que $v(f)=c(V,\lbrace t\rbrace)$, entonces $f$ es maximal

#### Condición del corte

##### Definiciones importantes

- Un Corte es un subconjunto de los vértices que tiene a $s$ pero no tiene a $t$
- La capacidad de un corte es $cap(S)=c(S,\bar S)$
- Un corte es _minimal_ si $cap(S)\leq cap(T)\forall$ corte $T$

##### Condición importante

**Teorema de Ford-Fulkerson (Max Flow-Min Cut)**:

- Si $f$ es un flujo y $S$ es un corte, entonces $v(f)=f(S,\bar{S})-f(\bar{S},S)$
- $v(f)\leq cap(S)\forall f,S$ flujos y cortes, respectivamente
- Si $f$ flujo, son equivalentes:
  - $\exists S$ corte $:v(f)=cap(S)$
  - $f$ maximal
  - No existen $f$-caminos aumentantes

  y si se cumplen, $S$ es minimal

En realidad, se llama **Max Flow Min Cut** a:
$$f\text{ maximal }\Rightarrow\exists S:v(f)=cap(S)$$

Como consecuencia de este teorema tenemos que:
- Si $f$ es maximal, podemos obtener un $S$ minimal con lo siguiente:
  - $S=\lbrace s\rbrace\cup\lbrace x\in V:\exists f$-camino aumentante desde $s$ a $x\rbrace$

  Ahora, tenemos dos opciones
  - Si en algún momento $t\in S$, paramos porque $f$ no es maximal
  - Si terminamos con $t\not\in S$, el algoritmo se detiene, el flujo es maximal y el último $S$ es el corte minimal que queremos

#### Teorema de la Integralidad

En un Network con capacidades enteras, todo flujo entero maximal es un flujo maximal

## Algoritmos para encontrar flujos maximales

La mayoría de los algoritmos que se verán encuentran un flujo maximal construyendo flujos _no maximales_, cada uno con valor más grande que el anterior, hasta llegar a un flujo maximal

### Idea principal

Supongamos que ya tenemos un flujo $f$. ¿Cómo cambiar $f$ pero de tal forma que lo que quede siga siendo un flujo?
  - Consideremos un camino dirigido entre $s$ y $t$
  - Dado ese camino, podemos cambiar $f$ a lo largo de este, incrementando el valor de $f$ en cada lado por una constante
    - $nuevaF(\overrightarrow{x_ix_{i+1}})=f(\overrightarrow{x_ix_{i+1}})+cte$
    - La $cte$ no debe hacer que se excedan las capacidades de los lados
  - En los $x_i$, sus $in$ y $out$ se incrementan en $cte$, por lo que la diferencia es la misma

Luego, la idea base es:

  - Comenzando desde algún flujo (por ejemplo el nulo), vamos encontrando caminos dirigidos desde $s$ a $t$
  - Aumentamos el flujo a lo largo de los caminos teniendo en cuenta de no mandar más flujo por el mismo que lo que pueden soportar los lados

Sin embargo, una vez detectado el camino y obtenida la cota superior de cuánto se puede mandar, ¿cuánto mandamos?

Esto se resolverá en los algoritmos que veremos.

### Algoritmo Greedy

El algoritmo es el siguiente:
  - Comienzo con $f=0$ (es decir, $f(\overrightarrow{xy})=0\forall\overrightarrow{xy}\in E$)
  - Busco un camino dirigido desde $s=x_o,x_1,..,x_r=t$ con $\overrightarrow{x_ix_{i+1}}\in E:f(\overrightarrow{x_ix_{i+1}})\lt c(\overrightarrow{x_ix_{i+1}})\forall i=0,..,r-1$
    - Lo llamaremos camino dirigido _no saturado_
  - Calcular $\epsilon=Min\lbrace c(\overrightarrow{x_ix_{i+1}})-f(\overrightarrow{x_ix_{i+1}})\rbrace$
  - Aumentar $f$ a lo largo del camino obtenido en $\epsilon$
    - Lo denotaremos como $sx_1...x_{r-1}t:\epsilon$
  - Seguir buscando caminos dirigidos no saturados y seguir los pasos mencionados

Sin embargo, aunque sea $O(m^2)$, no necesariamente obtiene un flujo maximal sino que depende de las elecciones de los caminos. Pese a eso, este algoritmo puede ser modificado para encontrar un flujo maximal en tiempo polinomial

### Algoritmos Not Greedy

Dado el algoritmo Greedy anteriormente explicado, el truco no está en tratar de "adivinar" la secuencia correcta:
  - Se puede contruir un algoritmo que corre Greedy y cuando llega a cierto punto "se da cuenta" que se equivocó en la elección de los caminos y "corrige" los errores
  - Para ello, entonces, necesitaremos dos herramientas
    - Algo que nos haga ver que nos equivocamos --> **Corte** (vista antes)
    - Una generalización del concepto de camino dirigido no saturado para poder corregir los errores --> **Camino aumentante**

#### Caminos aumentantes

Un camino aumentante (o $f$-camino aumentante) o camino de Ford-Fulkerson, es una sucesión de vértices $x_0,x_1,..,x_r$ tales que:
- $x_0=s,x_r=t$
- $\forall i=0,..,r-1$,
  - o bien $\overrightarrow{x_ix_{i+1}}\in E\wedge f(\overrightarrow{x_ix_{i+1}})\lt c(\overrightarrow{x_ix_{i+1}})$ --> Lado forward
  - o bien $\overrightarrow{x_{i+1}x_i}\in E\wedge f(\overrightarrow{x_{i+1}x_i})\gt 0$ --> Lado backward

En caso que el camino tenga $x_0=x,x_r=z$, diremos que es un _camino aumentante desde_ $x$ _hasta_ $z$

#### Ford-Fulkerson

##### Algoritmo

El algoritmo de FF para hallar el flujo maximal es:
- Comenzar con $f=0$ (es decir, $f(\overrightarrow{xy})=0\forall\overrightarrow{xy}\in E$)
- Buscar un $f$-camino aumentante $s=x_0,x_1,..,x_r=t$
- Considerar $\epsilon=Min\lbrace\epsilon_i\rbrace$ donde
  - $\epsilon_i=c(\overrightarrow{x_ix_{i+1}})-f(\overrightarrow{x_ix_{i+1}})\text{ en lados forward}$
  - $\epsilon_i=f(\overrightarrow{x_{i+1}x_i})\text{ en lados backwards}$
- Cambiar $f$ a lo largo del camino aumentante en $\epsilon$:
  - $f(\overrightarrow{x_ix_{i+1}})+=\epsilon$ en lados forward
  - $f(\overrightarrow{x_{i+1}x_i})-=\epsilon$ en lados backward
- Repetir el proceso hasta que no halla más caminos aumentantes

##### Notación

- Dado un camino, la acción de mandar $10$ unidades de flujo se denotará: $sE\overleftarrow{DC}\overleftarrow{BA}Ft:10$
  - Donde los lados denotados con $\overleftarrow{}$ son backward
- El flujo cambia de la siguiente forma:
  - $f+=10$ en $\overrightarrow{sE},\overrightarrow{ED},\overrightarrow{CB},\overrightarrow{AF},\overrightarrow{Ft}$
  - $f-=10$ en $\overrightarrow{CD},\overrightarrow{AB}$

##### Teoremas sobre su funcionamiento

- Puede no terminar nunca
- Mantiene _flujicidad_: Si $f$ es un flujo de valor $v$ y aumentamos $f$ con un $f$-camino aumentante con $\epsilon$ calculado como se explica en el algoritmo de FF, entonces lo que queda sigue siendo flujo y el valor del nuevo flujo es $v+\epsilon$
  - Si termina, devuelve un flujo
- **Corolario de Teorema Max Flow Min Cut**: Si FF termina, entonces termina con un flujo maximal
- **Teorema con el se demuestra la Integralidad**: En un Network donde todas las capacidades sean enteros, FF siempre termina y el flujo maximal resultante es un flujo entero

#### Edmonds-Karp (heurística)

Propusieron dos posibles mejoras de FF:
- Aumentar eligiendo caminos de longitud mínima --> Polinomial solo dependiendo de $n,m$ --> Este es el bueno, llamado _Algoritmo EK_ con complejidad $O(nm^2)\Rightarrow$ para networks ralos es $O(n^3)$ pero para densos, $O(n^5)$
- Aumentar eligiendo caminos de aumento máximo --> Polinomial dependiendo de $n,m,$ capacidades

y demostraron que en ambos casos SIEMPRE termina.

Una forma de recordarlo es:
$$EK=FF+BFS$$

##### Notaciones de EK para resolver los ejercicios

- Primero, damos el network listando los lados y las capacidades
- Como $EK=FF+BFS$, tenemos que ir construyendo la cola con $s$ como primer vértice
  - Dado el 1er vértice de la cola, agregamos sus vecinos
  - Tachamos este vértice (porque sale de la cola)
  - Cuando un nodo tiene de vecino a $t$, solo agregamos $t$ y terminamos

  Para poder reconstruir el camino, tenemos que saber para cada nodo quién lo puso en la cola
- Calcular cuánto flujo mandar por el camino ($\epsilon$)
  - Recorriendo el camino después de tenerlo
  - Calcularlo mientras hacemos el BFS
    - Tenemos un registro $\epsilon(x)$ que nos indica cuánto flujo podemos mandar por el camino aumentante temporario que va de $s$ a $x$
    - Si $x$ es puesto en la cola por un vértice $z$ de modo
      - Forward --> $\epsilon(x)=Min\lbrace\epsilon(z),c(\overrightarrow{zx})-f(\overrightarrow{zx})\rbrace$ y $p(x)=z$
      - Backward --> $\epsilon(x)=Min\lbrace\epsilon(z),f(\overrightarrow{xz})\rbrace$ y $p(x)=z^-$
  - Para recordar todos los datos, colocaremos del siguiente modo los datos $\overset{x}{\overset{p(x)}{\epsilon(x)}}$

#### Dinic (Mejora a EK)

Lo que no es tan bueno en EK es el _BFS_ que se recalcula en cada paso (aunque pasemos por caminos ya calculados antes). Luego, _DINIC_ va a mejorar esta parte para ir guardando esos cálculos y no tener que repetirlos.

##### Idea

La idea es _guardar_ todos los posibles caminos aumentantes de la longitud mínima (de $s$ a $t$) en un _network auxiliar_.

Dada esta estructura, una vez construido el network auxiliar con BFS, los caminos aumentantes se encuentran corriendo _Greedy_ con _DFS_ allí.

Se usa el network auxiliar hasta que no se puede usar más y luego se construye otro a partir del nuevo flujo.

##### Conceptos importantes

- **Flujos bloqueantes**: llamaremos a un flujo _bloqueante_ o _saturante_ si todo camino dirigido desde $s$ a $t$ tiene al menos un lado saturado.
  - Es decir, si cuando queremos usar Greedy en el network, no llegamos a $t$

- **Layered Networks** o **Networks por niveles**: un network por niveles es un network tal que el conjunto de vértices está dividido en subconjuntos $V_i$ (los niveles) tales que sólo existen lados entre un nivel y el siguiente, es decir, $\overrightarrow{xy}\in E\Leftrightarrow\exists i:x\in V_i,y\in V_{i+1}$

##### Algoritmo

El esquema es:
 1. Construir un network auxiliar (usando BFS)
 2. Correr Greedy con DFS en el network auxiliar hasta no poder seguir
    - Equivalente a decir: "Encontrar un flujo bloqueante del network auxiliar"
 3. Usar el flujo obtenido en el network auxiliar para modificar el flujo en el network original
 4. Repetir (1) con el nuevo flujo, hasta que, al querer construir un network auxiliar, no llegamos a $t$

##### Network auxiliar

La forma en la que se define el network auxiliar (network por niveles) usando el original y un flujo $f$ es la siguiente:

- Vértices: $V=\cup^r_{i=0}V_i$ tal que:
  - $V_i=\lbrace x:\exists$ camino aumentante encontrado por $EK$ con longitud $i\rbrace\forall i=0,..,r-1$
  - $V_r=\lbrace t\rbrace$
- Lados y capacidades: $\overrightarrow{xy}$ es un lado del network auxiliar si:
  - $x\in V_i,y\in V_{i+1}$
  - Se cumple alguna:
    - $f(\overrightarrow{xy})\lt c(\overrightarrow{xy})$ en el original
      - La capacidad del auxiliar en base al original será $cNew(\overrightarrow{xy})=c(\overrightarrow{xy})-f(\overrightarrow{xy})$
    - $f(\overrightarrow{yx})\gt 0$ en el original
      - La capacidad del auxiliar en base al original será $cNew(\overrightarrow{xy})=f(\overrightarrow{yx})$

El algoritmo de esto será:
- Tomar $V_0=\lbrace s\rbrace$
- Hacer $EK$ con el $BFS$
  - Si $x$ quiere agregar a $z$ a la cola (esté o no ya visitado), entonces se agrega el lado $\overrightarrow{xz}$ siempre que $z\in V_{i+1}$
  - Si llegamos a $t$
    - Dejamos de agregar más vértices
    - Agregamos solo lados entre vértices $V_{r-1}$ y $t$

##### Observaciones importantes

- Como el network auxiliar es un network por niveles, _todos_ los caminos de un mismo NA deben tener la _misma longitud_
- Dado un $NA_i$ con longitud de caminos $r_i$, los $NA_j$ tienen longitud $r_j\gt r_i\forall j\gt i$

##### Optimizaciones

Todas las optimizaciones son el el paso (2).

- **Dinitz**: Los lados saturados se eliminan del network auxiliar al igual que los lados o vértices que ya no se pueden usar
  - El NA se construye de forma tal que DFS _nunca tenga que hacer backtrack_
  - Luego, se termina el paso (2) cuando $s$ quede desconectado de $t$

- **Evens**: Realiza la optimización de Dinitz pero de forma _lazy_ borrando mientras está haciendo el DFS y llega a uno de ellos
  - La información de que es inútil seguir buscando por $x$ en el NA es importante guardarla para futuras corridas de DFS
    - Borro $x$ o bien $\overrightarrow{zy}$ si hacemos backtrack desde $x$ a $z$
  - Cuando corremos DFS, si llegamos a $x$ que no tiene vecinos, debemos hacer un _backtrack_

#### MKM (con base en Dinic)

La optimización se da en el Network Auxiliar:
- Se construye el flujo bloqueante o maximal de a varios caminos aumentantes en paralelo (a diferencia de FF, EK y Dinic que lo hacen de a uno)
  - Operación PUSH
    - $PUSH$ empuja cierta cantidad de flujo desde $x$ hasta $y\forall y\in\Gamma^+(x)$
      - Vertice a vecinos
  - Operación PULL
    - $PULL$ envía cierta cantidad de flujo desde $y$ hasta $x\forall y\in\Gamma^-(x)$
      - Vecinos a vertice
  - Esquema: se aplica PUSH-PULL en $x$:
    - En las dos operaciones se envía la misma cantidad de flujo para que $in(x) = out(x)$
    - Hacer PUSH-PULL en $x$ desbalancea a sus vecinos
      - $\Gamma^+(x)$ tienen $in\gt out$
        - Para "equilibrar", se hace PUSH en $\Gamma^+(x)$ (y luego en sus vecinos, etc...) hasta llegar a $t$
      - $\Gamma^-(x)$ tienen $in\lt out$
        - Para "equilibrar", se hace PULL en $\Gamma^-(x)$ (y luego en sus vecinos, etc...) hasta llegar a $s$
      - Luego, lo que queda es un flujo y es equivalente a haber construido muchos caminos aumentantes, todos pasando por $x$
- Esquema de aplicación
    1. Elegimos un $x$
        - Elige el vértice que menos "capacidad" tiene de enviar flujo tanto hacia adelante como hacia atrás
    2. Hacemos el proceso de PUSH y PULL
        - PUSH en $x$, $\Gamma^+(x)$, ..., hasta llegar a $t$
        - PULL en $x$, $\Gamma^-(x)$, ..., hasta llegar a $s$
        - La cantidad de flujo es lo máximo que podamos mandar, es decir, el mínimo de
          - Lo máximo que $x$ puede mandar a $\Gamma^+(x)$
          - Lo máximo que $\Gamma^-(x)$ pueden mandar a $x$
    3. Seguir con el paso (1) hasta obtener un flujo bloqueante
- Tiene la misma complejidad que Wave, solo que en la práctica tarda más la elección de $x$ por su construcción

#### Wave (mejora de MKM)

- Invariante
  - A diferencia de Dinic y MKM, el invariante es que _g sea bloqueante_ y nos detenemos cuando sea flujo
    - Antes se trataba de mantener _flujosidad_
- Funciones importantes
  - Forward Balance (FB) en $x$
    - PUSH en cada uno de los vecinos, ..., hasta llegar a $t$
    - Puede darse el caso en que haya un vértice que **no podemos balancear** porque no puede mandar más flujo hacia delante
          - En este caso tenemos un **vértice bloqueado**
  - Backward Balance (BB) en $x$
    - SOLO se aplica a vértices bloqueados. Devuelve flujo a uno o más de los vecinos que le mandaron
      - Siempre balancea al bloqueado porque, en el peor de los casos, devuelve todo el flujo
      - Devolver flujo desde un vértice bloqueado a uno no bloqueado puede desbalancearlo al vecino
        - Esperará al siguiente PUSH para balancearse
- Algoritmo wave (con "olas"):
  1. $PUSH(s)$. Se saturan todos los caminos de $s$ a $\Gamma^+(s)$ (y se bloquea a $s$)
  2. $FB(s)$ --> ola hacia adelante
      - Recorre los vértices desde $s$ a $t$, mandando flujo hacia los vértices del siguiente nivel, balanceando a los que pueda
        - En caso de no poder balancear un nodo, lo bloquea
          - Nunca más se desbloquean
  3. $BB(t)$ --> ola hacia atrás
      - Recorre los vértices desde $t$ hacia $s$, devolviendo flujo desde los vértices bloqueados solamente, balanceandolos
  4. Chequear si están o no TODOS balanceados (si $D(s) + D(t) = 0$)
      - Si no lo están, volver al paso (2). Caso contrario, terminar
- Complejidad
  - $O(n^3)$
