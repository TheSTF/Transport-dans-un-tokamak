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