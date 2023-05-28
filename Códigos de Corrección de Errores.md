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

- Observar que si con ciertos parámetros $n$, $ len(C) $, $t$ la cota de Hamming no se satisface, entonces sabemos que no existe un código con esos parámetros. Pero si la cota **sí** se satisface, eso **no implica** que **exista** un código con esos parámetros.
  - La cota de Hamming sirve para probar imposibilidad, no existencia
  - Si queremos probar existencia, vamos a tener que **dar** un código que satisfaga las condiciones que se piden

##### Código perfecto

- Un código $C$ es _perfecto_ si el $\leq$ de la cota de Hamming es un $=$. Es decir, si $$len(C)=\frac{2^n}{\sum_{i=1}^t \binom{n}{i}}$$
- Se llaman así porque son lo mejor que puede haber, dadas las limitaciones del universo
  - Hay una cantidad infinita (haciendo variar el $n$) pero son extremadamente raros
