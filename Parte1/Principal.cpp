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
        Mat ventanaGaussiano;
        Mat ventanaBlur;
        Mat ventanaMediana;

        Mat bordeSobelGris;
        Mat bordeLaplaceGris;

        namedWindow("Video Original", WINDOW_AUTOSIZE);
        namedWindow("Video Original Gris", WINDOW_AUTOSIZE);
        namedWindow("Video Sal", WINDOW_AUTOSIZE);
        namedWindow("Video Pimienta", WINDOW_AUTOSIZE);
        namedWindow("Video Mediana", WINDOW_AUTOSIZE);
        namedWindow("Video Blur", WINDOW_AUTOSIZE);
        namedWindow("Video Gaussiano", WINDOW_AUTOSIZE);

        namedWindow("Bordes Sobel Gris", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Gris", WINDOW_AUTOSIZE);

        namedWindow("Bordes Sobel Gaussiano", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Gaussiano", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Blur", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Blur", WINDOW_AUTOSIZE);
        namedWindow("Bordes Sobel Mediana", WINDOW_AUTOSIZE);
        namedWindow("Bordes Laplacianos Mediana", WINDOW_AUTOSIZE);

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

            ventanaGaussiano = Filtro(ventanaGris,fgaussiano,1);
            ventanaBlur= Filtro(ventanaGris, fblur,2);
            ventanaMediana = Filtro(ventanaGris, fmediana,3);

            //Bordes de Laplace
            bordeLaplaceGris = BordesLaplace(ventanaGris);

            //Bordes de Sobel
            bordeSobelGris = BordesSobel(ventanaGris);

            createTrackbar("Sal","Video Original",&sal, 100, functionTrackbar, NULL);
            createTrackbar("Pimienta","Video Original",&pimienta, 100, functionTrackbar, NULL);
            createTrackbar("Mediana","Video Original",&fmediana, 100, functionTrackbar, NULL);
            createTrackbar("Blur","Video Original",&fblur, 100, functionTrackbar, NULL);
            createTrackbar("Gaussiano","Video Original",&fgaussiano, 100, functionTrackbar, NULL);

            //Mostramos los resultados
            imshow("Video Original",ventana);
            imshow("Video Original Gris",ventanaGris);
            imshow("Video Sal",ventanaRuidoSal);
            imshow("Video Pimienta",ventanaRuidoPimienta);
            imshow("Video Mediana",ventanaMediana);
            imshow("Video Blur",ventanaBlur);
            imshow("Video Gaussiano",ventanaGaussiano);

            imshow("Bordes Sobel Gris", bordeLaplaceGris);
            imshow("Bordes Laplacianos Gris", bordeSobelGris);

            imshow("Bordes Sobel Gaussiano", BordesSobel(ventanaGaussiano));
            imshow("Bordes Laplacianos Gaussiano", BordesLaplace(ventanaGaussiano));
            imshow("Bordes Sobel Blur", BordesSobel(ventanaBlur));
            imshow("Bordes Laplacianos Blur", BordesLaplace(ventanaBlur));
            imshow("Bordes Sobel Mediana", BordesSobel(ventanaMediana));
            imshow("Bordes Laplacianos Mediana", BordesLaplace(ventanaMediana));

            if(waitKey(23)==27)
                break;
        }
        destroyAllWindows();
    }
    return 0;
}