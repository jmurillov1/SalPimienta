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

int main (int arcg, char *argv[]){
    // En OpenCV las imágenes a color se representan como BGR:

    // Leemos la imagen y la guardamos en una matriz llamada "imagen"
    Mat catedral = imread("Catedral-San-Basilio.png");
    resize(catedral, catedral, Size(), 0.7,0.7);

    // Creamos una ventana donde se mostrará la imagen

    Mat resultado;
    Vec3b pixel;

    if (resultado.empty()){
        resultado = Mat(Size(catedral.cols,catedral.rows), CV_8UC3, Scalar(0,0,0));
    }

    for (double i=-(catedral.cols/2); i < (catedral.cols/2) ; i=i+0.08){
        double res = 30*sin(i);
        circle(resultado, Point(i+(catedral.cols/2), ((catedral.rows/2)+15)-(res)), 1, Scalar(255, 255, 255), 1);
    }
    
    //

    /*for (int i = 0; i < catedral.rows; i++){
        for (int j = 0; j < catedral.cols; j++){
            pixel = catedral.at<Vec3b>(i,j);
            if (i < 200){
                resultado.at<Vec3b>(i,j) = catedral.at<Vec3b>(i,j);
            }
        }
    }*/

    namedWindow("Original", WINDOW_AUTOSIZE);
    //namedWindow("Triangulo", WINDOW_AUTOSIZE);
    namedWindow("Resultado", WINDOW_AUTOSIZE);

    imshow("Original", catedral);
    //imshow("Triangulo", catedralt);
    imshow("Resultado", resultado);
    // Guardar la imagen
    // imwrite("Catedral-San-Basilio.png", imagen);
    
    // Quedamos en espera de que se presione alguna tecla
    waitKey(0);
    destroyAllWindows();

    return 0;
}