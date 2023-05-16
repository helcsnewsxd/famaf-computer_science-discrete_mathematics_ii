# Matchings

## Definición del problema

Dado un grafo $G$ bipartito, encontrar un matching en $G$ con la mayor cantidad de lados posibles

- Dado un grafo $G$ un _matching_ en $G$ es un subgrafo $M$ de $G$ tal que el grado de cada vértice de $M$ es uno (en $M$)
  - Es decir, es un conjunto de lados disjuntos

Un matching que tenga la mayor cantidad posible de lados se dice _matching maximal_

## Reducción del problema a flujo

1. Transformamos el grafo bipartito no dirigido en un network (el cual tendrá todas sus capacidades enteras)
    - Consideramos $G$ bipartito y $X,Y$ sus partes (de modo que todo lado conecta un $x\in X$ con un $y\in Y$)
    - Se agregan dos vértices nuevos $s,t$
    - Se consideran los siguientes lados (de capacidad $1$):
      - $\overrightarrow{xy}$ para todo $x\in X,y\in Y,xy\in E(G)$
      - $\overrightarrow{sx}$ para todo $x\in X$
      - $\overrightarrow{yt}$ para todo $y\in Y$

2. Encontramos un flujo entero maximal en ese network, que sabemos por el _teorema de la Integralidad_ que existe
    - Por ejemplo, usando FF, EK o Dinitz

3. A partir de ese flujo maximal obtenemos un matching maximal
    - Dado un flujo $f$ entero, si tomamos
      - $W=\{ x\in X:in_f(x)=1\}$
      - $U=\{ y\in Y:out_f(y)=1\}$
      - $F=\{ xy\in E(G):x\in W,y\in U:f(\overrightarrow{xy})=1\}$

      entonces $(W\cup U,F)$ es un subgrafo de $G$ que es el matching maximal, donde su cantidad de lados ( $\#F$ ) es igual al valor del flujo ( $v(f)$ )
    - Vale también la inversa (de matching a flujo), por lo que flujos maximales enteros se corresponden con matchings maximales

## Perfección y completitud de los matchings

Para resolver estos problemas simplemente se halla un matching maximal y se chequea la condición

### Matching Perfecto

Matching que "cubre" todos los vértices, es decir, donde el conjunto de sus vértices es igual que el de $G$

- Condición necesaria
  - $|X|=|Y|$
- **Teorema de Konig (del matrimonio)**
  - Todo grafo bipartito regular tiene un matching perfecto

### Matching completo

Matching que "cubre" todos los vértices de una de las partes del grafo, es decir, si cubre a todos los de $X$ se dice que es _completo sobre_ $X$

- Condición necesaria
  - $|X|\leq |Y|$
    - Si $|X|=|Y|$, entonces es también perfecto
- **Teorema de Hall**
  - Si $G$ es un grafo bipartito con partes $X,Y$ y $|S|\leq|\Gamma(S)|\forall S\subseteq X$, entonces existe un matching completo sobre $X$

## Matchings pesados

### Minimizando el mayor costo (Algoritmo de Gross)

- Se considera un grafo $G$ bipartito con pesos en los lados, con ambas partes con la misma cardinalidad (digamos $n$) y al menos un matching perfecto
- Se quiere encontrar de entre todos los matchings perfectos, uno que minimice el mayor peso
- El algoritmo es el siguiente:
  - Se hace búsqueda binaria sobre los costos de los lados en el grafo
  - Se calcula el matching maximal para el grafo donde se consideran solo las aristas con costo menor o igual a $edgeValue[m]$
  - Si tiene matching perfecto, hay que tomar un umbral más chico
    - Puede considerarse la mitad entre el $m$ y la cota inferior, pero una optimización (no mejora complejidad del peor caso) es considerar en vez del umbral actual, el máximo costo considerado en el matching perfecto
  - Caso contrario, si no lo tiene, hay que tomar uno más alto (el nuevo umbral es la mitad entre el $m$ y la cota superior)
- La complejidad del algoritmo de Gross es $O(n^{\frac{5}{2}}lg(n))$

### Minimizando la suma de los costos
