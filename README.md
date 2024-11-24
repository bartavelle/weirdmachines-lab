# Weird machines - machine virtuelle

Ce TP consiste à exploiter une machine virtuelle simpliste. Le code source est fourni, dans le fichier `interpreter.c`.

La machine virtuelle est une *stack machine*, basée sur un système de pile qui sera familier aux développeurs Forth, ou aux utilisateurs de calculatrices de marque HP.

Le programme à exécuter est le premier argument du programme, et un secret est passé en second argument.
Le but du TP est d'exécuter un programme qui fera fuiter le contenu du secret.

## Caractéristiques de la machine virtuelle

* pile de 8 entiers de 64 bits
* mémoire de 32 entiers de 64 bits

Durant ce TP, il est possible de recompiler, de débugger et de modifier `interpreter.c`, même si ce n'est *a priori* pas nécessaire.

## Spécifications du langage

Les programmes sont constitutés de caractères ASCII, ayant la signification suivante:

* `:` : noop, aucune opération
* `Q` : fin de programme
* `+` : opération binaire addition
* `*` : opération binaire multiplication
* `-` : opération binaire soustraction
* `/` : opération binaire division
* `S` : échange des deux premiers éléments de la pile
* `P` : retire le premier élément de la pile, et l'affiche au format décimal
* `X` : retire le premier élément de la pile, et l'affiche au format hexadécimal
* `D` : duplique le premier élément de la pile
* `I` : continue à parcourir le programme tant que des chiffres de `0` à `9` sont présents. Pousse la valeur décimale des chiffres sur la pile.
* `?` : non documenté, affiche des informations de *debug*
* `0` : retire le premier élément de la pile, et le stocke à l'adresse mémoire 0
* `Z` : pousse la valeur à l'adresse mémoire 0 dans la pile
* `>` : retire le premier élément de la pile, qui servira d'addresse. Retire le second élément de la pile, et le stocke à l'adresse précédemment obtenue.
* `<` : retire le premier élément de la pile, qui servira d'addresse. Lit la valeur mémoire à l'adresse précédemment obtenue, et le pousse sur la pile.

Les opérations binaires retirent le premier élément de la pile, qui servira d'opérande *de droite*.
Elles retirent encore le premier élément de la pile, qui servira d'opérande *de gauche*.

## Exemples

### Soustraction
```
I30I10-
```

Le premier élément de la pile est `20`.

### utilisation de la mémoire

```
I40:0
```

La pile n'a pas changé, mais la mémoire à l'adresse 0 contient la valeur `40`. Notez l'utilisation de `:` pour séparer la valeur immediate de l'opération `0`.

```
I4I765>
```

La pile n'a pas changé, mais la mémoire à l'adresse 4 contient la valeur `765`.

```
I20I4<*I2S>
```

La pile n'a pas changé, mais la mémoire à l'adresse 2 contient la valeur stockée en mémoire à l'adresse 4 multipliée par 20.

# TP

## Identifier la faille

La pile peut contenir 8 valeurs. Que se passe t'il lorsque l'on la remplit?
Expliquer ce que fait le programme suivant:

```
I1I2I3I4I5I6I7I8I456
```

## Position relative des éléments en mémoire

Lorsqu'il s'exécute, le programme affiche l'adresse où sont stockés le pointeur `secret`, l'état de la machine virtuelle, ainsi que sa mémoire.
A chaque exécution, ces adresses varient, mais certains invariants peuvent être découverts. Les décrire.

## Obtenir l'adresse de la machine virtuelle

Il est possible de stocker le pointeur `this` de la structure `stt` dans la position 0 de la mémoire. Comment?

## Calculer la position du pointeur `secret`

Il est possible de calculer la position de `secret` en fonction de celle de `stt`. Comment ?

## Faire pointer la mémoire de la machine virtuelle vers le secret

Il est possible d'écraser le pointeur `memory` pour le faire pointer vers le secret. Comment ?

## Conclusion

Écrire un programme qui permettra d'afficher les 64 premiers bits du secret. Quel est le plus court programme permettant d'y arriver?

Cet exercice est inspiré d'un exploit réel : https://j00ru.vexillium.org/slides/2015/recon.pdf

## 
