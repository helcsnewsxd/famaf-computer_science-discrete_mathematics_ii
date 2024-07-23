# Coloreo de Grafos (vértices)

Un coloreo es una función cualquiera $c:V\rightarrow S$ donde $S$ es un conjunto finito

Si la cadinalidad de $S$ es $k$, diremos que el coloreo tiene $k$ colores

## Coloreo propio

Un coloreo es _propio_ si $xy\in E\Rightarrow c(x)\neq c(y)$, es decir, los extremos de los lados tienen distinto color

Un grafo que tiene un coloreo propio con $k$ colores se dice $k$ _-coloreable_

## Número cromático

El número cromático de un grafo es $\chi(G)=Min\lbrace k:\exists$ coloreo propio con $k$ colores de $G\rbrace$

Si uno dice que $\chi(G)=k$, hay que hacer dos cosas para probarlo:
  - Dar un coloreo de $G$ con $k$ colores y probar que es propio
  - Probar que no existe ningún coloreo propio con $k-1$ colores de $G$

### Observaciones útiles

  - $\chi(G)\leq n$
  - $\chi(G)=1\Leftrightarrow E=\empty$
  - $\chi(G)\geq 2\forall G$ con al menos un lado
  - $\chi(C_{2r})=2$
    - $c(i)= i\pmod{2}$
  - $\chi(C_{2r+1})=3$
    - $c(i)=i\pmod{2}\forall i\lt 2r+1$
    - $c(2r+1)=2$
  - $\chi(G)\geq 3\Leftrightarrow C_{2r+1}$ es subgrafo de $G$
  - $\chi(K_n)=n$
  - Si $H$ es un subgrafo de $G$, entonces $\chi(H)\leq\chi(G)$

### Algoritmo de fuerza bruta

Un algoritmo para encontrar $\chi(G)$ es:
  - Calcular todos los coloreos posibles
  - Filtrar los propios
  - Quedarme con el que tenga la menor cantidad de colores

Sin embargo, tiene complejidad $O(n^nm)$ ya que:
  - Hay $n^n$ posibles colores
  - Chequear que un coloreo es propio es $O(m)$

### Algoritmo Greedy

#### Funcionamiento

  - _Input_: Se recibe como input el grafo $G$ y un _orden_ de los vértices
  - _Coloreo propio_ Se colorean los vértices de $G$ uno por uno, en el orden dado, manteniendo siempre el invariante que el coloreo parcial es propio
  - _Greedy_: Al momento de colorear cada vértice, se colorea con el menor color posible que se le pueda dar manteniendo la invariante del coloreo parcial propio

Sin embargo, ir tomando el "más chico" en cada vértice no optimiza la solución. Luego, no siempre obtiene $\chi(G)$

#### Complejidad

$$O(d(x_1)+...+d(x_n))=O(2m)=O(m)$$

#### Cotas

  - $\chi(G)\leq\Delta +1$
    - Como $d(x)\leq\Delta$, si tenemos $\Delta+1$ colores disponibles, siempre habrá un color extra para colorear a $x$
  - _Teorema de Brooks_: Si $G$ es conexo, entonces $\chi(G)\leq\Delta$, a menos que $G$ sea un ciclo impar o un grafo completo
    - _Propiedad_: Si $G$ es conexo, entonces existe un ordenamiento de los vértices tal que Greedy colorea todos los vértices, salvo uno, con $\Delta$ colores o menos

#### Correctitud

_Very Important Theorem (VIT)_:
  - Sea $G=(V,E)$ un grafo cuyos vértices están coloreados con un coloreo propio $c$ con $r$ colores $\lbrace 0,..,r-1\rbrace$
  - Sea $\pi$ una permutación de los colores
  - Sea $V_i=\lbrace x\in V:c(x)=i\rbrace,i=0,..,r-1$
  - Si ordenamos los vértices del siguiente modo: $V_{\pi(0)},..,V_{\pi(r-1)}$ (orden interno de $V_{\pi(i)}$ es irrelevante), entonces Greedy en ese orden coloreará $G$ con $r$ colores o menos

Luego, como corolario tenemos que:
  - Existe un ordenamiento de los vértices de $G$ tal que Greedy colorea $G$ con $\chi(G)$ colores

Por ello, se podría ordenar con todos los órdenes posibles y correr Greedy para obtener $\chi(G)$, pero como hay $n!$ órdenes posibles, no sería en tiempo polinomial.

Por ello, entonces, el VIT se suele usar para tratar de obtener una aproximación a $\chi(G)$

## Problema $k$-color

### Grafo bipartito

Un grafo se dice bipartito si $\chi(G)\leq 2$, o de forma equivalente, si $G=(V,E)$ y $\exists X,Y\subseteq V$ tales que:
  - $V=X\cup Y$
  - $W\cap Y=\empty$
  - $wv\in E\Rightarrow (w\in X,v\in Y) \lor (w\in Y,v\in X)$

### Descripción del problema

Dado un grafo $G$, ¿es $\chi(G)\leq k$?

#### Caso 2-color

Tiene solución en tiempo polinomial (complejidad $2*O(m)=O(m)$ ) y el algoritmo es:
  - Elegir un vértice $x$ cualquiera
  - Correr $BFS(x)$ creando un árbol
  - Para cada vértice $z$, sea $N(z)$ el nivel de $z$ en el árbol $BFS(x)$, colorear $c(z)=N(z)\pmod{2}$
  - Chequear si el coloreo es propio
    - Si lo es, $\chi(G)\leq 2$
    - Caso contrario, $\chi(G)\gt 2$

Luego, tenemos como corolario que $C_{2r+1}$ es subgrafo de $G\Leftrightarrow$ no es $2$-color