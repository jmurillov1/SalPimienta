#include <iostream>
#include <cstdlib>
#include <math.h>

#include <opencv2/core/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/video/video.hpp>

#include <opencv2/videoio/videoio.hpp>

using namespace std;
using namespace cv;

int sal = 5;
int pimienta = 5;
int fmediana = 5;
int fblur = 5;
int fgaussiano = 5;

void functionTrackbar(int v, void *p){
}

Mat SalPimienta(Mat imagen, int valor, int tipo){
    Mat res = imagen.clone();
    double percent=valor/100.0;
    int total = (int) (percent*res.rows*res.cols);
    int cont=0, fila=0, columna=0;
    while(cont < total){
        res.at<uchar>(rand() % res.rows,rand() % res.cols) = (tipo==1) ? 255:0;
        cont++;
    }
    return res;
}

Mat Filtro(Mat imagen, int valor, int tipo){
    valor = valor % 2 ==0 ? valor - 1 : valor;
    valor = valor < 1 ? 1 : valor;
    Mat res;
    if (tipo==1){
        GaussianBlur(imagen, res, Size(valor,valor),2,2);
        return res;
    }else if (tipo==2){
        blur(imagen, res, Size(valor,valor));
        return res;
    }else if (tipo==3){
        medianBlur(imagen, res, valor);
    }
    return res;
}

Mat BordesSobel(Mat imagen){
    Mat gX, gY, gXAbs, gYAbs, bordeSobel, bordeSobelAbs;
    Sobel(imagen, gX, CV_16S, 1, 0, 3);
    Sobel(imagen, gY, CV_16S, 0, 1, 3);
    Sobel(imagen, bordeSobel, CV_16S, 1, 1, 3);
    convertScaleAbs(gX,gXAbs);
    convertScaleAbs(gY,gYAbs);
    convertScaleAbs(bordeSobel,bordeSobelAbs);
    return bordeSobelAbs;
}

Mat BordesLaplace(Mat imagen){
    Mat bordeLaplace, bordeLaplaceAbs;
    Laplacian(imagen, bordeLaplace, CV_16S, 3);
    convertScaleAbs(bordeLaplace, bordeLaplaceAbs);
    return bordeLaplaceAbs;
}

int main (int arcg, char *argv[]){

    VideoCapture video("video.mp4");

    if (video.isOpened()){

        //Declaración de variables a usar
        Mat ventana;
        Mat ventanaGris;
        Mat ventanaRuidoSal;
        Mat ventanaRuidoPimienta;

        Mat salGaussiano;
        Mat pimientaGaussiano;
        Mat salBlur;
        Mat pimientaBlur;
        Mat salMediana;
        Mat pimientaMediana;

        namedWindow("Video Original", WINDOW_AUTOSIZE);
        namedWindow("Video Original Gris", WINDOW_AUTOSIZE);

        namedWindow("Video Gaussiano Sal", WINDOW_AUTOSIZE);
        namedWindow("Video Gaussiano Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Video Blur Sal", WINDOW_AUTOSIZE);
        namedWindow("Video Blur Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Video Mediana Sal", WINDOW_AUTOSIZE);
        namedWindow("Video Mediana Pimienta", WINDOW_AUTOSIZE);

        namedWindow("Bordes Sobel Gaussiano Sal", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Gaussiano Sal", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Gaussiano Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Gaussiano Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Blur Sal", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Blur Sal", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Blur Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Blur Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Mediana Sal", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Mediana Sal", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Mediana Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Mediana Pimienta", WINDOW_AUTOSIZE);

        while(3==3){
            //Asignamos el video a una ventana
            video >> ventana;
            //Cambiamos tamaño de video
            resize(ventana, ventana, Size(), 0.3,0.3);
            //Transformamos a Escala de Gris
            cvtColor(ventana, ventanaGris, COLOR_BGR2GRAY);

            //Ruido Sal
            ventanaRuidoSal = SalPimienta(ventanaGris, sal, 1);

            //Ruido Pimienta
            ventanaRuidoPimienta = SalPimienta(ventanaGris, pimienta, 0);

            //Filtros SAL y PIMIENTA
            Mat salGaussiano = Filtro(ventanaRuidoSal,fgaussiano,1);
            Mat pimientaGaussiano = Filtro(ventanaRuidoPimienta,fgaussiano,1);
            Mat salBlur = Filtro(ventanaRuidoSal, fblur,2);
            Mat pimientaBlur = Filtro(ventanaRuidoPimienta, fblur,2);
            Mat salMediana = Filtro(ventanaRuidoSal, fmediana,3);
            Mat pimientaMediana = Filtro(ventanaRuidoPimienta, fmediana,3);

            // Creaci´on de los Trackbar Necesarios
            createTrackbar("Sal","Video Original",&sal, 100, functionTrackbar, NULL);
            createTrackbar("Pimienta","Video Original",&pimienta, 100, functionTrackbar, NULL);
            createTrackbar("Mediana","Video Original",&fmediana, 100, functionTrackbar, NULL);
            createTrackbar("Blur","Video Original",&fblur, 100, functionTrackbar, NULL);
            createTrackbar("Gaussiano","Video Original",&fgaussiano, 100, functionTrackbar, NULL);

            //Mostramos los resultados
            imshow("Video Original",ventana);
            imshow("Video Original Gris",ventanaGris);

            //Mostramos los filtros
            imshow("Video Gaussiano Sal", salGaussiano);
            imshow("Video Gaussiano Pimienta", pimientaGaussiano);
            imshow("Video Blur Sal", salBlur);
            imshow("Video Blur Pimienta", pimientaBlur);
            imshow("Video Mediana Sal", salMediana);
            imshow("Video Mediana Pimienta", pimientaMediana);
            
            // Mostramos los Bordes
            imshow("Bordes Sobel Gaussiano Sal", BordesSobel(salGaussiano));
            imshow("Bordes Laplacianos Gaussiano Sal", BordesLaplace(salGaussiano));
            imshow("Bordes Sobel Gaussiano Pimienta", BordesSobel(pimientaGaussiano));
            imshow("Bordes Laplacianos Gaussiano Pimienta", BordesLaplace(pimientaGaussiano));
            imshow("Bordes Sobel Blur Sal", BordesSobel(salBlur));
            imshow("Bordes Laplacianos Blur Sal", BordesLaplace(salBlur));
            imshow("Bordes Sobel Blur Pimienta", BordesSobel(pimientaBlur));
            imshow("Bordes Laplacianos Blur Pimienta", BordesLaplace(pimientaBlur));
            imshow("Bordes Sobel Mediana Sal", BordesSobel(salMediana));
            imshow("Bordes Laplacianos Mediana Sal", BordesLaplace(salMediana));
            imshow("Bordes Sobel Mediana Pimienta", BordesSobel(pimientaMediana));
            imshow("Bordes Laplacianos Mediana Pimienta", BordesLaplace(pimientaMediana));
            
            if(ventana.rows == 0 || ventana.cols == 0)
                break;
            
            if(waitKey(23)==27)
                break;
        }
        destroyAllWindows();
    }
    return 0;
}