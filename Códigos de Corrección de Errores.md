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

### Detección y Corrección de errores

- Así como todo espacio vectorial puede pensarse como la imagen de una cierta transformación lineal, también puede pensarse como el **núcleo** de alguna otra transformación lineal
  - Con esto, detectar errores es trivial: si $C=Nu(L)$, entonces las palabras de $C$ son exactamente aquellas $v$ para las cuales $L(v)=0$.

#### Matriz de Chequeo

- $H$ es una matriz de chequeo (de paridad) de un código $C$ de longitud $n$ si $C=Nu(H)=\left\lbrace x\in\left\lbrace 0,1\right\rbrace^n:H.x^t=0\right\rbrace$
  - Si bien la definición no lo requiere, consideraremos que las filas de $H$ son LI
  - Notemos lo siguiente
    - Dada una matriz $H$, definimos $C=Nu(H)$ y $C$ será lineal
    - Y viceversa, como todo subespacio vectorial es el núcleo de alguna transformación lineal, y asociada a toda transformación lineal tenemos una matriz, todo código lineal tiene asociada al menos una matriz de chequeo
- **Dimensiones** (considerando filas como LI)
  - Cantidad de columnas $\rightarrow n$
  - Cantidad de filas $\rightarrow r=n-k$
    - Dado esto, la dimensión de $C$ en términos del número de filas y columnas de la matriz de chequeo es $k=n-r=\text{nro columnas - nro filas}$
      - Si $H$ tiene la identidad, es contar la cantidad de columnas que no forman parte de esta
  - Por ello, $H$ es $(n-k)\times n$ si las filas son LI
- Propiedades y Teoremas
  - Si $H$ es matriz de chequeo de $C$, entonces $$\delta(C)=Min\left\lbrace j:\exists\text{ un conjunto de j columnas LD de H}\right\rbrace$$

##### **Elección de matriz óptima**

###### Problema

Supongamos que se pide dar una matriz de chequeo lo más chica posible que sirva para codificar $2^k$ palabras con un código lineal que corrija al menos un error

- Buscamos que las filas sean LI
- Como se necesitan codificar $2^k$ palabras, entonces la matriz será $r\times (r+k)$
- Queremos corregir al menos un error, así que queremos que la matriz de chequeo tenga todas las columnas distintas y no contenga la columna $0$
- La condición $r+k\leq 2^r-1$ es una condición necesaria y suficiente para poder tener $r+k$ columnas distintas y no nulas para la matriz de chequeo

###### Código de Hamming (Otro punto de vista del problema)

Este código surge del siguiente problema: "dado un $r$ (cantidad de filas de la matriz de chequeo), ¿hasta cuál $k$ (dimensión del código lineal) podemos llegar?"

- El máximo $k$ es $2^r-1-r$
- Consisten en tomar una matriz con TODAS las $2^r-1$ columnas no nulas posibles
  - Es independiente del orden porque, si bien los códigos que surgen son distintos, son equivalentes entre sí al ser permutaciones las palabras entre estos
- Denotaremos por $H_r$ a cualquier código de Hamming con $r$ filas
- **Propiedades**
  - Tiene $r$ filas
  - Tiene dimensión $k=2^r-1-r$
  - Tiene $2^{2^r-1-r} palabras$
  - Tiene $\delta=3$
    - Por construcción no tiene la columna $0$ ni columnas repetidas, así que $\delta\geq 3$
    - Pero como tiene todas las columnas, en particular también tiene tres que son LD en algún lugar, así que $\delta\leq 3$
  - Por lo tanto, _corrije exactamente un error_
  - Los códigos de Hamming son **perfectos**
- **Ordenamientos óptimos de las columnas de un código de Hamming**
  - Son cuatro dependiendo de
    - Si contamos las columnas desde la izquierda o desde la derecha
    - Si representamos los números con el bit más representativo arriba o abajo
  - El orden óptimo es el que tiene como columna $i$-ésima la representación binaria del número $i$
  - Es óptimo ya que permite un mejor cálculo en el algoritmo de corrección de errores
    - Si $H.w^t\neq 0$, tomamos $v=w+e_j$ como palabra mandada, donde $j$ es tal que su representación binaria es la columna $H.w^t$
    - Calcular $H.w^t$ es simplemente hacer suma (xor) de los $j$ correspondientes a los bits $1$ de $w$. Es decir, los números de las posiciones en donde $w$ tiene $1$
- **Contras**
  - Si se producen 2 errores, no hay forma de darse cuenta que hubo 2 errores y no 1, pues la suma de dos columnas va a dar otra, siempre. Por ello, dos errores SIEMPRE serán corregidos como si hubiera habido uno solo y, por lo tanto, corregidos mal
    - Para esto, se usan los **códigos de Hamming extendidos**, los cuales surgen de agregar un bit de paridad al código $C$

#### Corrección de al menos UN error

- Si $H$ es una matriz que _no tiene la columna cero ni tiene columnas repetidas_, entonces $C=NU(H)$ corrige _al menos UN error_. Si, además, alguna columna es suma de otras dos u otras tres columnas, entonces $C$ corrije exactamente un error.
- **ALGORITMO de corrección de UN error**
  - Calcular $H.w^t$
  - Si el resultado es la columna $0$, aceptar $w$ como la palabra mandada
  - Si el resultado es una columna de $H$, digamos la $j$-ésima, cambiar el bit $j$ de $w$ para recuperar la palabra mandada
  - Si el resultado **no** es ninguno de los anteriores, hubo más de un error y no se puede corregir de esta forma

### Relación entre Matriz Generadora y Matriz de Chequeo

Si $A$ es una matriz $r\times k$ entonces $[I_r|A]$ es una matriz de chequeo de un código $C$ si y solo si $[A^t|I_k]$ es una matriz generadora de $C$

- Similarmente, [A|I_r] es una matriz de chequeo de un código $C$ si y solo si $[I_k|A^t]$ es una matriz generadora de $C$

### Teorema: Cota de Singleton

Si $C$ es un código lineal de longitud $n$ y dimensión $k$, entonces: $$\delta(C)\leq n-k+1$$

- Códigos en donde se cumple la _igualdad_ se llaman MDS codes (Maximum Distance Separable Codes) y son extremadamente importantes en transmisión y criptografía.

### Códigos para corrección de MÁS DE UN error

#### Códigos de repetición

Sea $C$ un código de longitud $n$, definimos el código de repetición $r$ veces a partir de $C$ que denotaremos como $Rep_r(C)$ de la siguiente forma: $$Rep_r(C)=\left\lbrace v\in\left\lbrace 0,1\right\rbrace^{nr}\right\rbrace :v_1v_2...v_n\in C\wedge$$ $$v_1v_2...v_n=v_{n+1}v_{n+2}...v_{2n}=v_{2n+1}v_{2n+2}...v_{3n}=...=v_{(r-1)n+1}v_{(r-1)n+2}...v_{rn}$$

- Es decir, las palabras de $$Rep_r(C)$$ son de la forma $v...v$ con $v\in C$ donde hay $r$ "$v$"
  - La _cantidad_ de palabras de $C$ y $Rep_r(C)$ es la misma
- $\delta(Rep_r(C))=r\delta(C)$
  - Esto nos permite construir códigos con $\delta$ arbitrariamente grandes pero a un costo grande en cantidad de bits extras

## Códigos de Corrección de Errores Cíclicos
