# PPD - Tema 1 - Rezultate Comprehensive Testing
---

## Java - Rezultate

### Test 1: N=10, M=10, k=3
**Sequential:** 0.10 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 4 | 2.60 ms | 0.04x | 2.00 ms | 0.05x |

---

### Test 2: N=1000, M=1000, k=5
**Sequential:** 105.70 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 34.20 ms | 3.09x | 35.80 ms | 2.95x |
| 4 | 21.90 ms | 4.83x | 24.50 ms | 4.31x |
| 8 | 22.60 ms | 4.68x | 24.50 ms | 4.31x |
| 16 | 23.00 ms | 4.60x | 25.40 ms | 4.16x |

**Observații:** Cel mai bun speedup: 4.83x cu 4 threads (Horizontal)

---

### Test 3: N=10, M=10000, k=5
**Sequential:** 4.30 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 3.70 ms | 1.16x | 4.10 ms | 1.05x |
| 4 | 3.60 ms | 1.19x | 3.60 ms | 1.19x |
| 8 | 4.40 ms | 0.98x | 4.10 ms | 1.05x |
| 16 | 5.00 ms | 0.86x | 4.80 ms | 0.90x |

**Observații:** Beneficii minime de la paralelizare (overhead > beneficiu la 8+ threads)

---

### Test 4: N=10000, M=10, k=5
**Sequential:** 4.20 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 3.40 ms | 1.24x | 4.00 ms | 1.05x |
| 4 | 3.50 ms | 1.20x | 4.10 ms | 1.02x |
| 8 | 4.10 ms | 1.02x | 4.90 ms | 0.86x |
| 16 | 4.80 ms | 0.88x | 5.20 ms | 0.81x |

**Observații:** Speedup scade cu creșterea numărului de thread-uri

---

### Test 5: N=10000, M=10000, k=5
**Sequential:** 4470.50 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 2097.90 ms | 2.13x | 2196.00 ms | 2.04x |
| 4 | 2091.70 ms | 2.14x | 2204.80 ms | 2.03x |
| 8 | 2075.70 ms | 2.15x | 2198.50 ms | 2.03x |
| 16 | 2347.30 ms | 1.90x | 2249.30 ms | 1.99x |

**Observații:** Speedup constant ~2.1x, degradare la 16 threads

---

## C++ - Rezultate

### Test 1: N=10, M=10, k=3
**Sequential:** 0.00 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 4 | 0.00 ms | -nan | 0.10 ms | 0.00x |

**Observații:** Input prea mic pentru măsurători relevante

---

### Test 2: N=1000, M=1000, k=5
**Sequential:** 40.00 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 20.30 ms | 1.97x | 24.20 ms | 1.65x |
| 4 | 20.20 ms | 1.98x | 20.70 ms | 1.93x |
| 8 | 20.00 ms | 2.00x | 21.10 ms | 1.90x |
| 16 | 22.30 ms | 1.79x | 20.50 ms | 1.95x |

**Observații:** Speedup aproape liniar până la 8 threads

---

### Test 3: N=10, M=10000, k=5
**Sequential:** 6.10 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 3.60 ms | 1.69x | 5.90 ms | 1.03x |
| 4 | 3.00 ms | 2.03x | 4.40 ms | 1.39x |
| 8 | 2.20 ms | 2.77x | 4.80 ms | 1.27x |
| 16 | 3.50 ms | 1.74x | 2.30 ms | 2.65x |

**Observații:** Horizontal funcționează mai bine, cu speedup maxim 2.77x la 8 threads

---

### Test 4: N=10000, M=10, k=5
**Sequential:** 4.10 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 2.00 ms | 2.05x | 2.50 ms | 1.64x |
| 4 | 2.00 ms | 2.05x | 2.00 ms | 2.05x |
| 8 | 2.00 ms | 2.05x | 2.60 ms | 1.58x |
| 16 | 3.00 ms | 1.37x | 2.40 ms | 1.71x |

**Observații:** Speedup constant 2.05x pentru Horizontal (2-8 threads)

---

### Test 5: N=10000, M=10000, k=5
**Sequential:** 3711.10 ms

| Threads | Horizontal | Speedup | Vertical | Speedup |
|---------|------------|---------|----------|---------|
| 2 | 1837.60 ms | 2.02x | 2447.90 ms | 1.52x |
| 4 | 1862.50 ms | 1.99x | 1869.00 ms | 1.99x |
| 8 | 1870.10 ms | 1.98x | 1860.80 ms | 1.99x |
| 16 | 1901.90 ms | 1.95x | 1843.50 ms | 2.01x |

**Observații:** Performanță stabilă ~2.0x speedup indiferent de numărul de threads

---

## Comparație Java vs C++

### Performanță Secvențială (N=10000, M=10000, k=5)
- **Java:** 4470.50 ms
- **C++:** 3711.10 ms
- **Diferență:** C++ este cu ~17% mai rapid

## Limitari hardware

- **cpu** 4 cores fizice


### Concluzii Generale

- In toate cazurile CPP are timpul mai mic decat cel secvential
- CPP pare sa aibe un cap la 2.00x (posibil sa fie un bottleneck)
- Java are rezultate mult mai bune in unele cazuri, insa uneori timpul este mai mare decat cel secvential
- Laptopul meu are 4 core-uri si observ ca atunci cand sunt 4 thread-uri timpul e cel mai bun
- Partitionarea orizontala are rezultate mai bune
- 