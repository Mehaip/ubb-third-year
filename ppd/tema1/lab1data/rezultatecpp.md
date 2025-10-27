# PPD - Tema1 - Rezultate C++

## N=10, M=10, k=3

### Vector Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 0.00 | - |
| p=4 Horizontal | 0.00 | -nan |
| p=4 Vertical | 0.00 | -nan |

### Static Array Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 0.00 | - |
| p=4 Horizontal | 0.00 | -nan |
| p=4 Vertical | 0.00 | -nan |


---

## N=1000, M=1000, k=5

### Vector Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 45.30 | - |
| p=2 Horizontal | 27.50 | 1.65x |
| p=2 Vertical | 21.40 | 2.12x |
| p=4 Horizontal | 20.10 | 2.25x |
| p=4 Vertical | 21.60 | 2.10x |
| p=8 Horizontal | 19.90 | 2.28x |
| p=8 Vertical | 20.70 | 2.19x |
| p=16 Horizontal | 21.40 | 2.12x |
| p=16 Vertical | 24.50 | 1.85x |

### Static Array Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 50.70 | - |
| p=2 Horizontal | 23.70 | 2.14x |
| p=2 Vertical | 29.10 | 1.74x |
| p=4 Horizontal | 24.50 | 2.07x |
| p=4 Vertical | 25.80 | 1.97x |
| p=8 Horizontal | 24.20 | 2.10x |
| p=8 Vertical | 22.90 | 2.21x |
| p=16 Horizontal | 23.00 | 2.20x |
| p=16 Vertical | 17.30 | 2.93x |

**Observații**: 
- Vector Implementation: Speedup optim cu 8 threads Horizontal (2.28x)
- Static Array: Speedup optim cu 16 threads Vertical (2.93x)
- Vector este ~10% mai rapid secvențial decât Static Array

---

## N=10, M=10000, k=5

### Vector Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 3.20 | - |
| p=2 Horizontal | 1.80 | 1.78x |
| p=2 Vertical | 1.40 | 2.29x |
| p=4 Horizontal | 1.80 | 1.78x |
| p=4 Vertical | 1.50 | 2.13x |
| p=8 Horizontal | 2.20 | 1.45x |
| p=8 Vertical | 2.10 | 1.52x |
| p=16 Horizontal | 2.10 | 1.52x |
| p=16 Vertical | 2.00 | 1.60x |

### Static Array Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 4.20 | - |
| p=2 Horizontal | 2.00 | 2.10x |
| p=2 Vertical | 2.00 | 2.10x |
| p=4 Horizontal | 2.00 | 2.10x |
| p=4 Vertical | 2.40 | 1.75x |
| p=8 Horizontal | 2.20 | 1.91x |
| p=8 Vertical | 1.00 | 4.20x |
| p=16 Horizontal | 2.30 | 1.83x |
| p=16 Vertical | 1.00 | 4.20x |

**Observații**: 
- Static Array cu 8-16 threads Vertical obține speedup f bun (4.20x)
- Vector Implementation: Speedup optim cu 2 threads Vertical (2.29x)

---

## N=10000, M=10, k=5

### Vector Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 3.10 | - |
| p=2 Horizontal | 1.40 | 2.21x |
| p=2 Vertical | 2.00 | 1.55x |
| p=4 Horizontal | 2.00 | 1.55x |
| p=4 Vertical | 2.10 | 1.48x |
| p=8 Horizontal | 3.00 | 1.03x |
| p=8 Vertical | 3.10 | 1.00x |
| p=16 Horizontal | 2.10 | 1.48x |
| p=16 Vertical | 2.10 | 1.48x |

### Static Array Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 7.60 | - |
| p=2 Horizontal | 2.10 | 3.62x |
| p=2 Vertical | 3.00 | 2.53x |
| p=4 Horizontal | 2.40 | 3.17x |
| p=4 Vertical | 4.50 | 1.69x |
| p=8 Horizontal | 3.20 | 2.37x |
| p=8 Vertical | 2.40 | 3.17x |
| p=16 Horizontal | 2.60 | 2.92x |
| p=16 Vertical | 2.90 | 2.62x |

**Observații**: 
- Static Array cu 2 threads Horizontal obține cel mai bun speedup (3.62x)
- Vector este semnificativ mai rapid secvențial (~2.4x) decât Static Array

---

## N=10000, M=10000, k=5

### Vector Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 3869.40 | - |
| p=2 Horizontal | 2043.60 | 1.89x |
| p=2 Vertical | 2466.00 | 1.57x |
| p=4 Horizontal | 2477.90 | 1.56x |
| p=4 Vertical | 2193.80 | 1.76x |
| p=8 Horizontal | 1883.20 | 2.05x |
| p=8 Vertical | 2059.30 | 1.88x |
| p=16 Horizontal | 1967.20 | 1.97x |
| p=16 Vertical | 1934.90 | 2.00x |

### Static Array Implementation
| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 4479.80 | - |
| p=2 Horizontal | 2381.00 | 1.88x |
| p=2 Vertical | 2501.10 | 1.79x |
| p=4 Horizontal | 2267.10 | 1.98x |
| p=4 Vertical | 2309.70 | 1.94x |
| p=8 Horizontal | 2340.90 | 1.91x |
| p=8 Vertical | 2488.40 | 1.80x |
| p=16 Horizontal | 2160.10 | 2.07x |
| p=16 Vertical | 2386.00 | 1.88x |

**Observații**: 
- Vector Implementation: Speedup optim cu 8 threads Horizontal (2.05x)
- Static Array: Speedup optim cu 16 threads Horizontal (2.07x)
- Vector este ~15% mai rapid secvențial decât Static Array

---

## Concluzii generale

### Comparație Vector vs Static Array
- **Performanță secvențială**: Vector Implementation este mai rapid în majoritatea cazurilor
- **Scalabilitate**: Ambele implementări au pattern-uri similare de scalare
- **Speedup maxim**: Similar între cele două implementări (2.0-2.3x pentru matrici mari)

L 10000x10000 observ un bottleneck pt valoare 2.00