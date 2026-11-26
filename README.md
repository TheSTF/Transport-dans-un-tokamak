# tokamak-transport

[![CI](https://github.com/thestf/transport-dans-un-tokamak/actions/workflows/ci.yml/badge.svg)](https://github.com/thestf/transport-dans-un-tokamak/actions/workflows/ci.yml)
![C11](https://img.shields.io/badge/C-C11%20strict-blue)
![Python](https://img.shields.io/badge/Post--traitement-NumPy%20%2F%20Matplotlib-informational)

Solveur **C11** de transport thermique 2D axisymétrique en coupe poloïdale $(R, Z)$ pour un plasma de tokamak, accompagné d'une suite de vérification numérique et d'un post-traitement Python.

> **Question scientifique.** Ce projet ne cherche pas à « démontrer la fusion » : il construit un solveur de diffusion thermique vérifié, le calibre sur la loi d'échelle empirique **IPB98(y,2)** utilisée pour dimensionner ITER, et répond à une question quantitative : *quelle température centrale une machine compacte ($R_0 = 1{,}0\text{ m}$, $B_0 = 5{,}0\text{ T}$, $I_p = 1{,}0\text{ MA}$) peut-elle atteindre, et pourquoi n'atteint-elle pas 150 millions de degrés ?*

---

## Machine de référence et ordres de grandeur (0D)

La géométrie étudiée correspond à un tokamak compact à fort champ (comparable à FTU) en deutérium pur ($M = 2$, $Z_{\text{eff}} = 1$, $n = 5 \times 10^{19}\text{ m}^{-3}$).

| Paramètre / Diagnostic | Valeur (machine projet) | Critère / Référence |
| :--- | :---: | :--- |
| Géométrie ($R_0$, $a$, $\kappa$, $V$) | $1{,}0\text{ m}$, $0{,}4\text{ m}$, $1{,}7$, $5{,}37\text{ m}^3$ | Section elliptique imposée (niveau A) |
| Champ et courant ($B_0$, $I_p$) | $5{,}0\text{ T}$, $1{,}0\text{ MA}$ | $q_{95} \approx 10{,}0$ ($> 3$, OK) |
| Fraction de Greenwald $f_{GW}$ | $0{,}25$ | $< 0{,}85$ (OK) |
| Limite de Troyon $\beta_{\max}$ | $1{,}40\,\%$ | $\beta \approx 0{,}36\,\%$ à $10\text{ MW}$ ($\beta_N \approx 0{,}7$, OK) |
| Seuil mode H $P_{LH}$ (Martin 2008) | $\approx 2{,}0\text{ MW}$ | Mode H plausible à $10\text{ MW}$ |
| Confinement $\tau_E$ (IPB98 à $10\text{ MW}$) | $0{,}0287\text{ s}$ | Chaîne vérifiée sur ITER : $3{,}68\text{ s}$ (publié : $3{,}7\text{ s}$) |
| Température centrale $T_0$ à $10\text{ MW}$ | $\approx 4{,}4\text{ keV}$ ($\approx 51\text{ MK}$) | $nT_0\tau_E \approx 6 \times 10^{18}\text{ keV}\cdot\text{s}\cdot\text{m}^{-3}$ ($\sim 500\times$ sous Lawson) |
| Puissance pour $T_0 = 150\text{ MK}$ ($12{,}9\text{ keV}$) | $\approx 300\text{ MW}$ ($\approx 55\text{ MW/m}^3$) | **Conclusion :** limité par le confinement (la taille), pas par $\beta$ |

---

## Modèle physique et hypothèses (Niveau A)

Équation de transport thermique résolue en coordonnées cylindriques axisymétriques ($\partial / \partial \varphi = 0$) par **volumes finis conservatifs** sur grille régulière $(R, Z)$ :

$$3n\frac{\partial T}{\partial t} = \frac{1}{R}\frac{\partial}{\partial R}\!\left(R K \frac{\partial T}{\partial R}\right) + \frac{\partial}{\partial Z}\!\left(K \frac{\partial T}{\partial Z}\right) + S_{\text{ohm}} + S_{\text{NBI}}, \qquad K = 2n\chi$$

* **Sources** : chauffage ohmique (résistivité de Spitzer, $T \ge T_{\text{bord}}$) et injection de neutres (profil gaussien centré sur l'axe magnétique, $\sigma = a/3$).
* **Condition aux limites** : $T = T_{\text{bord}} = 0{,}1\text{ keV}$ hors du masque elliptique plasma (dernière surface magnétique fermée).
* **Hypothèses assumées** : $T_e = T_i = T$, densité $n$ uniforme et fixe, diffusivité $\chi$ isotrope constante calibrée sur $\text{IPB98}(y,2)$ ($H_{98} = 1$ par construction). Les unités SI sont utilisées partout dans le solveur (seule $T$ est stockée en $\text{keV}$, via des conversions centralisées et testées).

---

## Compilation et utilisation

### Prérequis

* Compilateur C11 (`gcc` ou `clang`) et `make`
* Python 3 avec `numpy` et `matplotlib` pour le post-traitement :

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r scripts/requirements.txt
```

### Commandes principales

| Commande | Action |
| :--- | :--- |
| `make` | Compile la bibliothèque et l'exécutable `tokamak_cli` (`-std=c11 -Wall -Wextra -Werror -O2`) |
| `make test` | Compile et exécute la suite de tests unitaires et de vérification |
| `make asan` | Exécute les tests sous `AddressSanitizer` + `UndefinedBehaviorSanitizer` |
| `make figures` | Lance la simulation et génère les figures Python |
| `make clean` | Supprime les objets et binaires compilés |

---

## Architecture du dépôt

Le cœur numérique est conçu comme une bibliothèque C sans dépendance graphique, utilisable en ligne de commande (`tokamak_cli`) puis par un visualiseur SDL2 (après v1.0).

```text
.
├── include/            # En-têtes publics (constants, params, diagnostics, grid, io, ...)
├── src/                # Implémentation de la bibliothèque de calcul
├── apps/               # Exécutable CLI (tokamak_cli.c)
├── tests/              # Tests unitaires et solutions manufacturées (test_s1.c, ...)
├── scripts/            # Post-traitement Python et requirements.txt
├── configs/            # Fichiers de configuration (machine.cfg, iter.cfg — S4)
├── docs/               # Cahier des charges et documentation scientifique
├── .github/workflows/  # Intégration continue (build, tests, ASan)
└── Makefile
```

---

## Avancement et critères d'acceptation

Le développement suit un plan incrémental avec critères quantitatifs (détails dans le cahier des charges v2.1) :

| Critère | Vérification | Seuil | Statut |
| :---: | :--- | :---: | :---: |
| **S1** | Diagnostics 0D ($\tau_E$ ITER $= 3{,}68\text{ s}$), grille, masque ($V \to 5{,}37\text{ m}^3$), export CSV | $\pm 1\,\%$ / $\pm 2\,\%$ | **Validé** |
| **CA1** | Ordre de convergence spatial (solution manufacturée sur $33$, $65$, $129$ points) | $2{,}0 \pm 0{,}2$ | En cours (S2) |
| **CA2** | Résidu du bilan d'énergie $|dW/dt - (P_{\text{chauff}} - P_{\text{pertes}})| / P_{\text{chauff}}$ | $< 1\,\%$ | Planifié (S3) |
| **CA3** | Profil stationnaire vs solution analytique cylindrique ($R_0 = 100\text{ m}$) | écart max $< 2\,\%$ | Planifié (S3) |
| **CA4** | Calibration de $\chi$ sur IPB98 ($H_{98} = 1{,}00 \pm 0{,}10$) et mesure de $T_0$ | valeur + incertitude | Planifié (S3) |
| **CA5** | Affichage des 5 diagnostics 0D ($f_{GW}$, $\beta_N$, $q_{95}$, $nT\tau_E$, $P_{LH}$) avec statut OK/KO | 5 valeurs | Planifié (S3) |
| **CA6** | `-Wall -Wextra -Werror`, ASan/UBSan + Valgrind propres, $\ge 8$ tests, hash Git dans les sorties | $0$ erreur | En cours (S1 à S4) |
| **CA7** | Équilibre magnétique Grad-Shafranov (SOR) vs Solov'ev ($65 \times 65$) | erreur $< 1\,\%$, ordre 2 | Niveau B (2027) |

---

## Figures clés

- [x] **S1 — Contrôle géométrique** : masque elliptique du plasma sur grille $(R, Z)$ (`scripts/plot_mask.py`).
- [ ] **Fig. 1** — Carte de température $T(R, Z)$ avec contour du plasma.
- [ ] **Fig. 2** — Erreur en fonction du pas de maille (log-log) avec droite de pente 2 (CA1).
- [ ] **Fig. 3** — Profil radial simulé superposé à la solution analytique cylindrique (CA3).
- [ ] **Fig. 4** — Évolution de $W(t)$ et résidu du bilan d'énergie (CA2).
- [ ] **Fig. 5** — Température centrale $T_0(P)$ comparée au seuil $150\text{ MK}$ et aux limites opérationnelles.
- [ ] **Fig. 6** — Réponse du régulateur PI à un échelon de consigne avec saturation et anti-windup.

---

## Références principales

* J. Wesson, *Tokamaks*, 4e éd., Oxford University Press, 2011.
* ITER Physics Expert Groups, *ITER Physics Basis*, Nucl. Fusion 39 (1999) 2137 — loi d'échelle IPB98(y,2).
* M. Greenwald, *Density limits in toroidal plasmas*, Plasma Phys. Control. Fusion 44 (2002) R27.
* F. Troyon et al., *MHD-limits to plasma confinement*, Plasma Phys. Control. Fusion 26 (1984) 209.
* Y. R. Martin et al., *Power requirement for accessing the H-mode in ITER*, J. Phys. Conf. Ser. 123 (2008) 012033.

---

**Auteur** : Bassirima Sanogo — Élève-ingénieur IESE3, Polytech Grenoble.