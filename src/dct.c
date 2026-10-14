#include <stdio.h>
#include "dct.h"
#include "fast-dct-8.h"


/*
DCT optimisée en O(n²log n) utilisant l'algorithme de DCT rapide type II en 1D
créé par Arai, Agui, Nakajima en 1988. https://web.stanford.edu/class/ee398a/handouts/lectures/07-TransformCoding.pdf#page=30
et du Projet Nayuki. https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
*/

/* 
 * Fast discrete cosine transform algorithms (C)
 * 
 * Copyright (c) 2021 Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */


 
void applique_dct(float vecteur_bloc[64]) {

    // on applique la dct sur chaque ligne
    for (int i = 0; i < 8; i++) {
        FastDct8_transform_modifiee(&(vecteur_bloc[i * 8]), 1);
    }

    // on applique la dct sur chaque colonne
    for (int j = 0; j < 8; j++) {
        FastDct8_transform_modifiee(&(vecteur_bloc[j]), 8);
    }
}