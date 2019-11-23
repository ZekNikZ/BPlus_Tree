#include "BPlusTreeRenderer.h"

BPlusTreeRenderer::BPlusTreeRenderer() {
    charImages = {
            {'A', BitmapImage("A.bmp")},
            {'a', BitmapImage("lowerA.bmp")},
            {'B', BitmapImage("B.bmp")},
            {'b', BitmapImage("lowerB.bmp")},
            {'C', BitmapImage("C.bmp")},
            {'c', BitmapImage("lowerC.bmp")},
            {'D', BitmapImage("D.bmp")},
            {'d', BitmapImage("lowerD.bmp")},
            {'E', BitmapImage("E.bmp")},
            {'e', BitmapImage("lowerE.bmp")},
            {'F', BitmapImage("F.bmp")},
            {'f', BitmapImage("lowerF.bmp")},
            {'G', BitmapImage("G.bmp")},
            {'g', BitmapImage("lowerG.bmp")},
            {'H', BitmapImage("H.bmp")},
            {'h', BitmapImage("lowerH.bmp")},
            {'I', BitmapImage("I.bmp")},
            {'i', BitmapImage("lowerI.bmp")},
            {'J', BitmapImage("J.bmp")},
            {'j', BitmapImage("lowerJ.bmp")},
            {'K', BitmapImage("K.bmp")},
            {'k', BitmapImage("lowerK.bmp")},
            {'L', BitmapImage("L.bmp")},
            {'l', BitmapImage("lowerL.bmp")},
            {'M', BitmapImage("M.bmp")},
            {'m', BitmapImage("lowerM.bmp")},
            {'N', BitmapImage("N.bmp")},
            {'n', BitmapImage("lowerN.bmp")},
            {'O', BitmapImage("O.bmp")},
            {'o', BitmapImage("lowerO.bmp")},
            {'P', BitmapImage("P.bmp")},
            {'p', BitmapImage("lowerP.bmp")},
            {'Q', BitmapImage("Q.bmp")},
            {'q', BitmapImage("lowerQ.bmp")},
            {'R', BitmapImage("R.bmp")},
            {'r', BitmapImage("lowerR.bmp")},
            {'S', BitmapImage("S.bmp")},
            {'s', BitmapImage("lowerS.bmp")},
            {'T', BitmapImage("T.bmp")},
            {'t', BitmapImage("lowerT.bmp")},
            {'U', BitmapImage("U.bmp")},
            {'u', BitmapImage("lowerU.bmp")},
            {'V', BitmapImage("V.bmp")},
            {'v', BitmapImage("lowerV.bmp")},
            {'W', BitmapImage("W.bmp")},
            {'w', BitmapImage("lowerW.bmp")},
            {'X', BitmapImage("X.bmp")},
            {'x', BitmapImage("lowerX.bmp")},
            {'Y', BitmapImage("Y.bmp")},
            {'y', BitmapImage("lowerY.bmp")},
            {'Z', BitmapImage("Z.bmp")},
            {'z', BitmapImage("lowerZ.bmp")},
            {'0', BitmapImage("0.bmp")},
            {'1', BitmapImage("1.bmp")},
            {'2', BitmapImage("2.bmp")},
            {'3', BitmapImage("3.bmp")},
            {'4', BitmapImage("4.bmp")},
            {'5', BitmapImage("5.bmp")},
            {'6', BitmapImage("6.bmp")},
            {'7', BitmapImage("7.bmp")},
            {'8', BitmapImage("8.bmp")},
            {'9', BitmapImage("9.bmp")},
    };
}
