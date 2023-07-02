# Lista de demostraciones para el Final

## 1. Complejidad de EK

### Enunciado

EK tiene complejidad $O(nm^2)$

### Demostración

- _Suposiciones_
  - **S1**: $\overrightarrow{xy}\in E\Rightarrow\overrightarrow{yx}\not\in E$
    - Si estuvieran, se agrega un nodo intermedio y no se cambia la complejidad ya que, a lo sumo, se duplican las aristas
- _Definiciones_
  - **D1**: $f_k$ es el flujo creado por EK en el paso $k$
  - **D2**: un lado $\overrightarrow{xy}$ es $k$-usado si forma parte del camino aumentante tomado por EK en el paso $k$
  - **D3**: un lado $\overrightarrow{xy}$ es $k$-inusable si no forma parte de ningún camino aumentante en el paso $k$
    - Está saturado si es forward
    - Está vacío si es backward
  - **D4**: un lado $\overrightarrow{xy}$ es $k$-crítico si es $k$-usado y $k+1$-inusable
  - **D5**: $d_k(x)=Min\lbrace p:p\text{ es la longitud de un }f_k\text{-camino aumentante entre }s,x\rbrace$
    - Análogamente se define $b_k(x)$ como la distancia entre $x,t$
- _Lemas_
  - **L1**: las distancias D5 no disminuyen en pasos sucesivos
    - _Lo consideramos demostrado_
  - **L2**: si $\overrightarrow{xy}$ es $k$-usado, entonces $d_k(y)=d_k(x)+1$
    - Ya que, por EK, es parte de un camino aumentante construido con BFS, de la forma $s..xy..t$
  - **L3**: si $\overrightarrow{xy}$ es $k$-crítico y $k+r$-usado, entonces $d_{k+r}(t)\geq d_k(t)+2$ $(r\gt 0)$
    - **(1)** $d_k(y)=d_k(x)+1\wedge d_{k+r}(x)=d_{k+r}(y)+1$ (por L2)
    - Tenemos

      ![Ecuación](https://latex.codecogs.com/svg.image?%20%5Cbegin%7Baligned%7D%20d_r(t)%20=%20&%5C%20d_r(x)&plus;b_r(x)%5Ctext%7B%20por%20D2%7D%5C%5C%20=%20&%5C%20d_r(y)&plus;1&plus;b_r(x)%5Ctext%7B%20por%20lo%20visto%20antes%7D%5C%5C%20%5Cgeq%20&%5C%20d_k(y)&plus;1&plus;b_k(x)%5Ctext%7B%20por%20L1%7D%5C%5C%20=%20&%5C%20d_k(x)&plus;1&plus;1&plus;b_k(x)%5Ctext%7B%20por%20lo%20visto%20antes%7D%5C%5C%20=%20&%5C%20d_k(t)&plus;2%5Ctext%7B%20por%20D2%7D%20%5Cend%7Baligned%7D)
- _Demostración_
  - La complejidad global de EK es $O(cntIteraciones)\times O(BFS)$
    - **(1)** $O(cntIteraciones)=O(nm)$
      - Cuando un lado es $k$-crítico, puede volverse a usar luego de que la distancia entre $s,t$ aumente en $2$ (L3). Luego, como la distancia puede variar entre $1$ y $n-1$, un lado se puede volver crítico a lo sumo $\frac{n-1}{2}$ veces
      - Como EK vuelve algún lado crítico en cada iteración, hay a lo sumo $m\frac{n-1}{2}$ iteraciones
      - Luego, $O(cntIteraciones)=O(nm)$
    - **(2)** $O(BFS)=O(m)$
    - Por (1) y (2), $O(EK)=O(nm^2)$
  
**Q.E.D.**
