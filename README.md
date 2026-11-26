# Tokamak Transport

Simulation numérique du transport de plasma dans un tokamak.

## Commandes principales

- `make` : compile l'exécutable principal `tokamak_cli`
- `make test` : lance la suite de tests unitaires
- `make asan` : compile avec l'AddressSanitizer pour détecter les fuites mémoire
- `make figures` : génère les graphiques via les scripts Python

## Résultats principaux
*(Résultats et figures clés )*
1. Carte T(R,Z) avec le contour du plasma.

2. Erreur en fonction du pas, en log-log, avec la pente 2.

3. Profil simulé superposé à la solution analytique.

4. W(t) et résidu du bilan d'énergie.

5. T0 en fonction de P, avec la ligne 150 MK.
6. Réponse du PI à un échelon de consigne.