# Códigos de Corrección de Errores

Lo que queremos es tratar de diseñar sistemas de comunicaciones tal que cuando ocurran errores de transmisión, podamos

1. Determinar que han ocurrido errores
2. Detectar cuáles fueron esos errores y corregirlos

En general 1. es más fácil que 2. y un código tendrá

- Una _capacidad de detección de errores_ más allá de la cual puede no detectarlos
- Una _capacidad de corrección de errores_ más allá de la cual quizás pueda detectar que hubo pero no corregirlos

## Conceptos base

### Definición de códigos

- Un código es un conjunto no nulo ( $\neq\empty$ ) de palabras sobre un alfabeto $A$
- Un código de bloque (o _block-code_) es un código en donde todas las palabras tienen la misma longitud, es decir, $\exists n$ tal que el código es un subconjunto de $A^n$. En este caso, $n$ se llama la longitud del bloque
- Un código binario es un código en donde el alfabeto es $\left\lbrace 0,1\right\rbrace$

### Suposiciones del problema

- En general, salvo que se diga lo contrario, cuando digamos _código_, nos estaremos refiriendo a **códigos binarios de bloque**, es decir, a subconjuntos de $\left\lbrace 0,1\right\rbrace^n$ para algún $n$
- Supondremos siempre que $C$ tiene al menos dos palabras
- La situación que estamos modelando es que tenemos un _transmisor_ que enviará algún mensaje a través de un _canal_, para que lo reciba un _receptor_. El canal tendrá ruido, por lo que puede pasar que el receptor no reciba lo que fue mandado
- Suposiciones del canal
  - El canal no pierde ni añade bits, solo los transforma
  - La probabilidad de un cambio de $1$ a $0$ es la misma que la de un cambio de $0$ a $1$
  - La probabilidad de un cambio en el bit $i$ es independiente de la probabilidad de un cambio en el bit $j$
    - A esta probabilidad independiente de $i$ llamaremos $p$
    - $0\lt p\lt\frac{1}{2}$

### Estrategia de corrección de errores

- Si nos llega una palabra $w$, deducir cuál es la palabra $v$ "más probable" que nos hayan mandado y que pueda producir $w$
  - Si bien no sabemos cuál es la palabra enviada, queremos ver cual es la palabra, de entre todas las posibles, que tenga la mayor probabilidad de haber sido enviada
  - Esto es equivalente, por la suposición sobre $p$, a detectar cuál es la palabra que _necesita la menor cantidad de errores_ para que haya llegado la palabra que llegó
- Se corrige la palabra recibida a la palabra más probable que se haya mandado

Lo que se verá es cómo se diseñan los códigos para ello y los algoritmos correspondientes, para tener una gran capacidad de correción de errores y también mucha información que un código puede codificar.

### Herramientas útiles

#### Distancia entre palabras

- Dadas dos palabras $v,w\in\left\lbrace 0,1\right\rbrace^n$, la **distancia de Hamming** (o simplemente _distancia_) entre $v,w$ es $$d(v,w)=d_H(v,w)=\sharp\left\lbrace\text{bits de diferencia entre v y w}\right\rbrace$$
  Definimos además, dado un código $C$: $$\delta(C)=Min\left\lbrace d_H(v,w):v,w\in C,v\neq w\right\rbrace$$
  - La distancia de Hamming es efectivamente una distancia, es decir:
    - $d_H(v,w)=d_H(w,v)$
    - $d_H(v,w)\geq 0$
    - $d_H(v,w)=0\Leftrightarrow v=w$
    - $d_H(v,w)\leq d_H(v,u)+d_H(u,w)$ (desigualdad triangular)

#### Detección y corrección de errores

##### Conceptos

- Dada una palabra $v\in\left\lbrace 0,1\right\rbrace^n$ y un número natural $r\geq 0$, definimos el **disco de radio** $r$ **alrededor de** $v$ como $$D_r(v)=\left\lbrace w\in\left\lbrace 0,1\right\rbrace^n:d_H(v,w)\leq r\right\rbrace$$
- Un código $C$ **detecta** $r$ errores si $D_r(v)\cap C=\left\lbrace v\right\rbrace\forall v\in C$
- Un código $C$ **corrige** $r$ errores si $D_r(v)\cap D_r(w)=\empty\forall v,w\in C:v\neq w$

##### Teorema

- **Teorema de** $\delta$: sea $C$ un código y $\delta=\delta(C)$, entonces:
    1. $C$ detecta $\delta-1$ errores, pero no detecta $\delta$
    2. Si $t=\left\lfloor\frac{\delta-1}{2}\right\rfloor$ entonces $C$ corrige $t$ errores pero no corrige $t+1$ errores

### Restricciones

De la teoría de códigos, este es uno de los problemas: "diseñar códigos que corrijan errores mandando la menor cantidad posible de bits _extras_ por sobre los bits de _información_"

- Mientras más errores querramos corregir, más largas van a tener que ser las palabras del código

#### Cota de Hamming (Teorema)

Sea $C$ un código de longitud $n$, $\delta=\delta(C)$ y $t=\left\lfloor\frac{\delta-1}{2}\right\rfloor$, entonces $$len(C)\leq\frac{2^n}{\sum_{i=1}^t \binom{n}{i}}$$

- Observar que si con ciertos parámetros $n$, $len(C)$, $t$ la cota de Hamming no se satisface, entonces sabemos que no existe un código con esos parámetros. Pero si la cota **sí** se satisface, eso **no implica** que **exista** un código con esos parámetros.
  - La cota de Hamming sirve para probar imposibilidad, no existencia
  - Si queremos probar existencia, vamos a tener que **dar** un código que satisfaga las condiciones que se piden

##### Código perfecto

- Un código $C$ es _perfecto_ si el $\leq$ de la cota de Hamming es un $=$. Es decir, si $$len(C)=\frac{2^n}{\sum_{i=1}^t \binom{n}{i}}$$
- Se llaman así porque son lo mejor que puede haber, dadas las limitaciones del universo
  - Hay una cantidad infinita (haciendo variar el $n$) pero son extremadamente raros

## Códigos de Corrección de Errores Lineales

- Un código **lineal** de longitud $n$ es un _subespacio vectorial_ de $\left\lbrace 0,1\right\rbrace^n$
  - Es decir, un código $C$ es lineal si y solo si es un subconjunto no vacío de $\left\lbrace 0,1\right\rbrace^n$ invariante por la suma (i.e., $u,v\in C\Rightarrow u+v\in C$ )
  - Se prefieren los códigos lineales a los no lineales, siendo los más usados. Una de las causas es que es más fácil calcular $\delta$ en códigos lineales

### Peso de Hamming

- Dada una palabra $v$ de un código, el **peso de Hamming** de $v$ es $|v|=d_H(v,0)$, es decir, es el número de unos que tiene $v$.
  - _Observación:_ $d_H(x,y)=|x+y|$
  - _Lema:_ Si $C$ es lineal, entonces $\delta(C)=\text{Min}\left\lbrace |v|:v\in C,v\neq 0\right\rbrace$
    - Por ello, en esta clase de códigos, se puede calcular en tiempo _lineal_ en vez de hacerlo de forma _cuadrática_

### Dimensión de un código lineal

- Un código lineal con dimensión $k$, longitud $n$ y $\delta(C)=\delta$ se suele denotar como un código $(n,k,\delta)$
  - La dimensión de un espacio vectorial es la cardinalidad de cualquiera de sus bases. La propiedad que se tiene que cumplir para ser base es:
    - Generar $V$: $u\in V\Rightarrow\exists c_1,..,c_k:u=c_1u_1+..+c_ku_k$
    - Es LI: $c_1u_1+..+c_ku_k=0\Rightarrow c_1=..=c_k=0$
- _Teorema importante:_ si $k$ es la dimensión de un código y $B$ tiene $k$ elementos, entonces $B$ es base de $C$ $\Leftrightarrow$ $B$ genera a $C$ $\Leftrightarrow$ $B$ es LI
  - Es decir, para probar que algo es base de un código de dimensión $k$, podemos ver que
    - Genera y es LI
    - Tiene $k$ elementos y genera
    - Tiene $k$ elementos y es LI
- **Cantidad de elementos de un código lineal**
  - Si $k$ es la dimensión de $C$, entonces la cantidad de elementos es de $2^k$
  - Por ello, la dimensión de un código es el **logaritmo en base 2** del número de palabras
- **La dimensión nos dice cuántos de los bits del código son de información**
  - Los restantes $n-k$ son los bits que se tienen que agregar para poder corregir la cantidad de errores que querramos corregir

### Codificación y decodificación

En el caso general de un código lineal, las palabras a codificar serán las palabras de $\left\lbrace 0,1\right\rbrace^k$ (P), donde $k$ es su dimensión pero las palabras del código estarán en $\left\lbrace 0,1\right\rbrace^n$ (C) para algún $n$.

- El transmisor necesita algo que transforme cada palabra de $P$ en una de $C$ para poder mandarla (_codificación_), mientras que el receptor necesita, luego de haber corregido los errores, poder transformar la palabra de $C$ en la de $P$ correspondiente (_decodificación_).
- Para ello, se usan **transformaciones lineales**

#### Transformaciones a realizar

- Una transformación lineal $T:V_1\rightarrow V_2$ entre espacios vectoriales es una función tal que $u,v\in V_1,c\in K\Rightarrow T(c.u+v)=c.T(u)+T(v)$
  - $Im(T)=\left\lbrace v\in V_2:\exists u\in V_1:T(u)=v\right\rbrace$
  - Si $T:V_1\rightarrow V_2$ es lineal, entonces $Im(T)$ es un subespacio vectorial de $V_2$
- Por ello, entonces, buscamos implementar $T$ de la forma especial
  - $T:\left\lbrace 0,1\right\rbrace^n:x\rightarrow (x,L(x))$ o
  - $T:\left\lbrace 0,1\right\rbrace^n:x\rightarrow (L(x),x)$

  de modo que la matriz que se use para la transformación solo guarde la parte correspondiente a $L$

#### Matriz generadora

- G es matriz generadora de $C$ si $C$ es la imagen de la transformación lineal ( $T:\left\lbrace 0,1\right\rbrace^k\rightarrow\left\lbrace 0,1\right\rbrace^n$ dada por $T(u)=u.G$ donde $k=dimC$ )
  - Es decir, cualquier matriz cuyas filas formen base de $C$ es generadora
    - Si $G$ es generadora de $C$ que tiene longitud $n$ y dimensión $k$, entonces $G$ debe ser $k\times n$
    - Dada una matriz $G$ que sea $k\times n$ y cuyas filas sean LI, podemos simplemente definir a $C$ como el espacio generador por las filas de $G$
- Para que la decodificación sea rápida, se busca que $G$ tenga la _identidad_ a izquierda o derecha
  - Si la tenemos la identidad de $k\times k$ a izquierda, entonces $u.G = (u,u.A)$ si $G=[I_k|A]$. Y análogo para la derecha.

## Códigos de Corrección de Errores Cíclicos
