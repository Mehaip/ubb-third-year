# PPD - Tema1 - Rezultate Java

## N=10, M=10, k=3

| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 0.10 | - |
| p=4 Horizontal | 2.60 | 0.04x |
| p=4 Vertical | 2.00 | 0.05x |


---

## N=1000, M=1000, k=5

| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 105.70 | - |
| p=2 Horizontal | 34.20 | 3.09x |
| p=2 Vertical | 35.80 | 2.95x |
| p=4 Horizontal | 21.90 | 4.83x |
| p=4 Vertical | 24.50 | 4.31x |
| p=8 Horizontal | 22.60 | 4.68x |
| p=8 Vertical | 24.50 | 4.31x |
| p=16 Horizontal | 23.00 | 4.60x |
| p=16 Vertical | 25.40 | 4.16x |

**Observații**: Speedupul cel mai mare este la Horizontal p = 4

---

## N=10, M=10000, k=5

| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 4.30 | - |
| p=2 Horizontal | 3.70 | 1.16x |
| p=2 Vertical | 4.10 | 1.05x |
| p=4 Horizontal | 3.60 | 1.19x |
| p=4 Vertical | 3.60 | 1.19x |
| p=8 Horizontal | 4.40 | 0.98x |
| p=8 Vertical | 4.10 | 1.05x |
| p=16 Horizontal | 5.00 | 0.86x |
| p=16 Vertical | 4.80 | 0.90x |

**Observații**: Matrice îngustă și foarte înaltă. Nu are sens sa facem paralel, iar cand sunt
multe thread-uri timpul creste

---

## N=10000, M=10, k=5

| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 4.20 | - |
| p=2 Horizontal | 3.40 | 1.24x |
| p=2 Vertical | 4.00 | 1.05x |
| p=4 Horizontal | 3.50 | 1.20x |
| p=4 Vertical | 4.10 | 1.02x |
| p=8 Horizontal | 4.10 | 1.02x |
| p=8 Vertical | 4.90 | 0.86x |
| p=16 Horizontal | 4.80 | 0.88x |
| p=16 Vertical | 5.20 | 0.81x |

**Observații**: la fel

---

## N=10000, M=10000, k=5

| Configurație | Timp mediu (ms) | Speedup |
|--------------|-----------------|---------|
| Sequential | 4470.50 | - |
| p=2 Horizontal | 2097.90 | 2.13x |
| p=2 Vertical | 2196.00 | 2.04x |
| p=4 Horizontal | 2091.70 | 2.14x |
| p=4 Vertical | 2204.80 | 2.03x |
| p=8 Horizontal | 2075.70 | 2.15x |
| p=8 Vertical | 2198.50 | 2.03x |
| p=16 Horizontal | 2347.30 | 1.90x |
| p=16 Vertical | 2249.30 | 1.99x |

**Observații**: Pentru matricea foarte mare, speedup-ul este constant în jurul valorii de 2.15x (8 threads Horizontal), dar nu crește proporțional cu numărul de thread-uri. Overhead-ul de sincronizare devine semnificativ.

---

## Concluzii generale

- **Horizontal vs Vertical**: În majoritatea cazurilor, Horizontal oferă performanțe mai bune
- **Număr optim de thread-uri**: 4-8 threads pentru matrici medii și mari (pentru ca am 4 coreuri)
- **Limitări**: Pentru matrici foarte mici sau cu aspect ratio ciudat (inalte/lungi), paralelizarea nu aduce beneficii
- **Scalabilitate**: Speedup-ul nu crește liniar cu numărul de thread-uri din cauza overhead-ului