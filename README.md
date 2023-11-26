# 🐣 Rapport de Christian et Théo

- [🐣 Rapport de Christian et Théo](#-rapport-de-christian-et-théo)
  - [NIVEAU 1](#niveau-1)
    - [Ne gardez que le vert 🟢](#ne-gardez-que-le-vert-)
    - [Échanger les canaux 🔀](#échanger-les-canaux-)
    - [Noir \& Blanc ⚫⚪](#noir--blanc-)
    - [Négatif](#négatif)
    - [Dégradé](#dégradé)
  - [NIVEAU 2](#niveau-2)
    - [Miroir 🪞](#miroir-)
    - [Image bruitée](#image-bruitée)
    - [Rotation de 90°](#rotation-de-90)
    - [RGB split](#rgb-split)
    - [Luminosité](#luminosité)
  - [NIVEAU 3](#niveau-3)
    - [Disque](#disque)
      - [Cercle](#cercle)
      - [Rosace](#rosace)
    - [Mosaïque](#mosaïque)
      - [Mosaïque miroir](#mosaïque-miroir)
    - [Glitch](#glitch)
    - [Fractale de Mandelbrot](#fractale-de-mandelbrot)
    - [Vortex](#vortex)
    - [Tramage](#tramage)
    - [Normalisation de l'histogramme](#normalisation-de-lhistogramme)
  - [NIVEAU 4](#niveau-4)
    - [Convolutions](#convolutions)
      - [Netteté, Contours, etc.](#netteté-contours-etc)
      - [Filtres séparables](#filtres-séparables)
      - [Différence gaussienne](#différence-gaussienne)
    - [Tri de pixels](#tri-de-pixels)
  - [NIVEAU 5](#niveau-5)
    - [Filtre de Kuwahara (effet peinture à huile) 🖌️](#filtre-de-kuwahara-effet-peinture-à-huile-️)
    - [Diamond Square 🔷🟦](#diamond-square-)


## NIVEAU 1

### Ne gardez que le vert 🟢

| ![image](./images/logo.png) | ![image](./output/allGreen.png) |
| :-------------------------: | :-----------------------------: |
|            Avant            |              Après              |

**Méthode :** On parcourt un à un tous les pixels de l'image, puis on fixe la valeur du rouge et du bleu à 0 pour chacun d'entre eux : seuls le vert et le noir ressortent donc.

### Échanger les canaux 🔀

| ![image](./images/logo.png) | ![image](./output/inverseCanals.png) |
| :-------------------------: | :----------------------------------: |
|            Avant            |                Après                 |

**Méthode :** On sélectionne tous les pixels un par un de la même manière que pour l'exercice précédent, sauf qu'on échange cette fois-ci les valeurs de rouge et de bleu pour chacun d'entre eux grâce à un *swap*.

### Noir & Blanc ⚫⚪	

| ![image](./images/logo.png) | ![image](./output/blackAndWhite.png) |
| :-------------------------: | :----------------------------------: |
|            Avant            |                Après                 |

**Méthode :** On attribue à chaque pixel la moyenne de ses valeurs de rouge, de vert et de bleu de sorte à obtenir une image colorée en niveaux de gris.

### Négatif

| ![image](./images/logo.png) | ![image](./output/negatif.png) |
| :-------------------------: | :----------------------------: |
|            Avant            |             Après              |

**Méthode :** On parcours tous les pixels, puis on obtient de nouvelles valeurs de rouge, de vert et de bleu pour chacun d'entre eux en soustrayant à 1 les valeurs d'origine.

### Dégradé

| ![image](./output/degrade.png) |
| :----------------------------: |
|            Dégradé             |

**Méthode :** On parcourt les colonnes une à une. Pour chacune d'entre elles, on définit une valeur allant du noir au blanc en fonction de la position du *x* sur la largeur totale de l'image.

**Piège :** Il faut bien penser à convertir les *x* en *float* grâce à un *static_cast* pour définir les niveaux de gris.

---

## NIVEAU 2

### Miroir 🪞

| ![image](./images/logo.png) | ![image](./output/mirror.png) |
| :-------------------------: | :---------------------------: |
|            Avant            |             Après             |

**Méthode :** On parcourt les pixels à *x* distance de l'extrémité gauche de l'image pour les échanger avec ceux à *x* distance de la droite (*image.width() - 1 - x*).

**Piège :** Il faut seulement parcourir la moitié gauche (*image.width()/2*) de l'image : on échangerait sinon des pixels déjà déplacés.

### Image bruitée

| ![image](./images/logo.png) | ![image](./output/noise.png) |
| :-------------------------: | :--------------------------: |
|            Avant            |            Après             |

**Méthode :** Pour chaque pixel, on génère un nombre aléatoire allant de 0 à *n*, *n* étant inversement proportionnel à la quantité de bruit que l'on désire. Si *n* = 0, alors les valeurs de rouge, de vert et de bleu du pixel sélectionné prennent une valeur aléatoire comprise entre 0 et 1 pour générer une nouvelle couleur.

### Rotation de 90°

| ![image](./images/logo.png) | ![image](./output/rotate.png) |
| :-------------------------: | :---------------------------: |
|            Avant            |             Après             |

**Méthode :** On crée une nouvelle image aux proportions inversées, puis on parcourt l'image d'origine pour copier ses pixels sur la nouvelle image en appliquant la rotation.

**Piège :** Puisqu'on parcourt les pixels de l'image d'origine de coordonnées *{x, y}*, il faut donc copier ceux de coordonnées *{x, image.height() - y - 1}* (à cause de la rotation) aux points *{y, x}* sur la nouvelle image (à cause d'inversion de proportions).

### RGB split

| ![image](./images/logo.png) | ![image](./output/RGBSplit.png) |
| :-------------------------: | :-----------------------------: |
|            Avant            |              Après              |

**Méthode :** On crée une nouvelle image sur laquelle on va copier les pixels de l'image d'origine en décalant certaines couleurs.

**Piège :** Il faut prendre en compte les dépassements et gérer donc les cas où on demanderait à un pixel de prendre la couleur d'un autre qui serait en dehors de l'image.

### Luminosité

| ![image](./images/photo.jpg) | ![image](./output/photo_eclaircie.png) | ![image](./output/photo_assombrie.png) |
| :--------------------------: | :------------------------------------: | :------------------------------------: |
|            Normal            |                Éclairci                |                Assombri                |

**Méthode :** On parcourt les pixels un à un pour :
- augmenter leurs valeurs de rouge, de vert et de bleu grâce à une puissance supérieure à 1 ;
- diminuer ces valeurs à l'aide d'une puissance comprise entre 0 et 1.

---

## NIVEAU 3

### Disque

![image](./output/disk.png)

**Méthode :** On sélectionne chaque pixel de l'image et on calcule la distance qui le sépare du centre. Si cette distance est égale ou inférieure au rayon du disque alors on rend le pixel blanc.

#### Cercle

![image](./output/circle.png)

**Méthode :** On reprend le même principe que pour l'exercice précédant, sauf que la distance séparant le pixel du centre doit être comprise entre *rayon* et *rayon + thickness*.

#### Rosace

![image](./output/rosace.png)

**Méthode :** On trace le premier cercle au centre de l'image, puis on trace les autres autour de celui-ci en transformant leurs origines respectives de coordonnées (*x*, *y*) sous la forme (*r cos(angle)*, *r sin(angle)*). La valeur des angles est définie par une boucle : *angle = 2 i pi / 6*.

**Piège :** En passant de coordonnées cartésiennes à coordonnées polaires, il ne faut pas oublier de resituer celles-ci par rapport au centre de l'image. Par exemple, `center_x = image.width()/2 + r cos(angle)`.

### Mosaïque

![image](./output/mosaique.png)

**Méthode :** Si on veut *n répétition*, on crée une mosaïque *n fois* plus grande que notre image de départ. On parcourt cette mosaïque et on reproduit l'image de départ. Pour trouver l'équivalent de x et y par rapport à l'image de départ, on fait x/y % largeur/hauteur de l'image de départ. 

**Piège :** Il faut bien prendre l'équivalent x et y de notre image de départ sinon on risque de sortir de l'image.

#### Mosaïque miroir

![image](./output/mosaiqueMirroir.png)

**Méthode :** Il suffit qu'à chaque colonne paire, on reproduit l'image de départ en partant de la droite (image.width() - newX -1 ). Et on fait le même principe lorsque nous sommes dans une ligne paire.

**Piège :** Si t'en trouves

### Glitch

| ![image](./images/logo.png) | ![image](./output/glitch1.png) | ![image](./output/glitch2.png) |
| :-------------------------: | :----------------------------: | :----------------------------: |
|            Avant            |       Après (méthode 1)        |       Après (méthode 2)        |

**Méthode 1 :** A chaque pixel, on génère un nombre aléatoire compris entre 0.f et *0.33 par exemple*. On a donc 33% de chance qu'il y ait un glitch sur le pixel. Ainsi, on sélectionne un second pixel aléatoirement qui sert de base pour créer un rectangle aux dimensions elles-aussi aléatoires. On échange ensuite les pixels de ce rectangle avec le pixel d'origine et ses voisins.

**Méthode 2 :** On choisi un nombre *n de glitch* et on va générer *n fois* deux points aléatoirements dans l'image pour venir ensuite échanger les pixels.

**Piège :** Il faut gérer les dépassements pour que le rectangle formé n'aille pas au-delà des extrémités de l'image.

### Fractale de Mandelbrot

![image](./output/fractale.png)

**Méthode :** A chaque pixel, on associe un nombre complexe *x + iy* qu'on place ensuite dans un intervalle compris entre -2 et 2. On crée ensuite une boucle qui itère *n* fois `i++` et `z = z * z + c`. Si *abs(z)* ne dépasse jamais 2 d'ici la fin de la boucle (quand *i = n*), alors le pixel associé devient noir. Sinon, les niveaux de rouge, de vert et de bleu du pixel parcouru prennent la valeur *i/n*.

**Piège :** Il faut convertir *x* et *y* en *float* lorsque qu'on place les nombres complexes dans un intervalle compris entre -2 et 2.

### Vortex

| ![image](./images/logo.png) | ![image](./output/vortex.png) |
| :-------------------------: | :---------------------------: |
|            Avant            |             Après             |

**Méthode :** Pour chaque pixel, on calcule les nouvelles coordoonées de ce pixel en appliquant une rotation. Cette rotation dépend du centre de l'image et d'un angle. Pour calculer cette angle, on prend la distance du point (x,y) par rapport au centre de l'image. Ainsi, plus le pixel est loin du centre, plus l'angle est fort.

**Piège :** Ne pas oublier de diviser la distance obtenue par un certain nombre car sinon on risque d'avoir un vortex avec une rotation extrêmement forte.

### Tramage

| ![image](./images/photo.jpg) | ![image](./output/dithering.png) |
| :--------------------------: | :------------------------------: |
|            Avant             |              Après               |

**Méthode :** Il faut d'abord passer la photo en noir et blanc. Ensuite, pour chaque pixel de l'image, on va récupérer la valeur correspondante dans la matrice de Bayer et on calcule la couleur de sortie en fonction (*couleur + (1\*bayer_value*)). Si cette couleur est inférieur à un seuil (1/2), la couleur finale sera du noir, sinon du blanc.

**Piège :** Pour écrire le seuil (ici 1/2), il faut faire attention au type dans la division. En effet, 1/2 != 1/2.f en c++...

### Normalisation de l'histogramme

| ![image](./images/photo_faible_contraste.jpg) | ![image](./output/normalisation.png) |
| :-------------------------------------------: | :----------------------------------: |
|                     Avant                     |                Après                 |

**Méthode :** On parcourt une première fois l'image pour trouver le pixel le moins lumineux et le pixel le plus lumineux. Après avoir transformé la courbe de l'histogramme en fonction affine, on peut ensuite parcourir une seconde fois l'image cette fois-ci en modifiant les valeurs.

---

## NIVEAU 4

### Convolutions

| ![image](./images/logo.png) | ![image](./output/convolution.png) |
| :-------------------------: | :--------------------------------: |
|            Avant            |               Après                |

**Méthode :** Pour faire du flou avec la convolution, on va prendre pour chaque pixel, une moyenne de la couleur avec les pixel adjacents. Cela se fait en fonction du Kernel, si on a un kernel de Blur 5x5, on va faire la moyenne des couleurs sur un carré de 5x5 pour chaque pixel de notre image.

**Piège :** Encore une fois, il faut faire attention au type lors des divisions. (notamment ici dans les boucles)
Il faut aussi faire attention aux indices négatifs avec nos tableaux...

#### Netteté, Contours, etc.

| ![image](./images/logo.png) | ![image](./output/exercice19Outline.png) | ![image](./output/exercice19Emboss.png) | ![image](./output/exercice19Sharpen.png) |
| :-------------------------: | :--------------------------------------: | :-------------------------------------: | :--------------------------------------: |
|            Avant            |                 Outline                  |                 Emboss                  |                 Sharpen                  |

**Méthode :** Avec notre fonction de convolution générique, il suffit d'appliquer des kernels différents pour obtenir ces résultats.


#### Filtres séparables

| ![image](./images/logo.png) | ![image](./output/convolutionFiltreSepare.png) |
| :-------------------------: | :--------------------------------------------: |
|            Avant            |                     Après                      |

**Méthode :** Le résultat ne change pas d'une convolution basique mais c'est le temps de calcul qui va varier. On lance deux fois la convolution avec deux kernels différents. Par exemple, pour du blur 3x3 : *{{1,1,1}} et {{1},{1},{1}}* (en effet la multiplication de ces deux matrices revient à une matrice 3x3 remplie de 1);


#### Différence gaussienne

| ![image](./images/photo.jpg) | ![image](./output/differenceGaussian.png) |
| :--------------------------: | :---------------------------------------: |
|            Avant             |                   Après                   |

**Méthode :** On fait la convolution deux fois sur des images distinctes avec un *sigma* différent. Puis pour chaque pixel, on vient faire la différence.

**Difficulté:** Nous avons pas réussi à utiliser le T pour avoir le même rendu que le sujet, toutefois cela fonctionne quand même ! 😃 

### Tri de pixels

| ![image](./images/logo.png) | ![image](./output/pouet_tri.png) |
| :-------------------------: | :------------------------------: |
|            Avant            |              Après               |

**Méthode :** On crée une fonction mesurant la luminosité d'un pixel puis on ordonne les images du début à la fin par luminosité décroissante.

**Remarque :** Il nous reste à adapter la fonction pour trier les pixels horizontalement.

---

## NIVEAU 5

### Filtre de Kuwahara (effet peinture à huile) 🖌️

| ![image](./images/photo.jpg) | ![image](./output/kuwahara.png) |
| :--------------------------: | :-----------------------------: |
|            Avant             |              Après              |

**Méthode :**  Un peu comme le principe de la convolution, on fait des moyennes de couleur. Pour chaque pixel, on va prendre les pixels adjacents et les diviser en 4 régions. Pour chaque région, on calcule la couleur moyenne et l'écart type (la distance avec la moyenne). Le pixel prendra alors la couleur moyenne de la région dont l'écart type est le plus bas.

**Difficulté :** Il est facile de faire cette algorithme de manière assez brute mais il est plus complexe de réussir à optimiser le code avec des boucles...


### Diamond Square 🔷🟦

| ![image](./output/diamond_square.png) |
| :-----------------------------------: |
|            Diamond Square             |

**Remarque :** On a essayé de reproduire le code de la vidéo dans les grandes lignes mais le résultat n'est pas exactement celui attendu.



<details><summary>Résultats intermédiaire et/ou intéressant,drôle !</summary>

| ![image](./output/exercice15creepy.png) |
| :-------------------------------------: |
|            Mosaique miroir              |

| ![image](./output/exercice16PixelWar.png) |
| :-------------------------------------:   |
|            Pixel war                      |

| ![image](./output/exercice17çatouuurne.png) |
| :------------------------------------------:|
|     Résultat intermédiaire au vortex        |

**Remarque :** Cela peut s'expliquer par le fait que l'angle de rotation n'était pas en fonction de la distance jusqu'au centre et on peut voir aussi que l'image de départ reste encore. (on a pas mis de noir lorsqu'on sort de l'image...)

| ![image](./output/exercice17cestartistique.png) |
| :------------------------------------------:|
|     Résultat intermédiaire au vortex 2       |

**Remarque :** Le rendu est vraiment stylé ! 😆 A mon avis, il y avait encore un soucis par rapport à l'angle.

| ![image](./output/exercice17Sharingan.png) |
| :------------------------------------------:|
|     Résultat intermédiaire au vortex 3     |

**Remarque :** Ah bah d'accord, là, on a carrément un sharingan 😆

</details>