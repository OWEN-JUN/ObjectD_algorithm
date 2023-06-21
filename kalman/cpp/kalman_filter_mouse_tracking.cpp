//
//  kalman_filter_2var.cpp
//  test
//
//  Created by owen on 2023/06/09.
//

#include <stdio.h>
#include <Eigen/Dense>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <random>
#include <time.h>
using namespace cv;
using namespace std;
using namespace Eigen;

Mat img;
MatrixXd A(4, 4);
MatrixXd X(4,1);
MatrixXd P(4,4);
MatrixXd sX(2,1);
MatrixXd Q(4,4);
MatrixXd R(2,2);
MatrixXd H(2,4);
//MatrixXd X_hat;
//MatrixXd x_hat(4);



string get_shape(MatrixXd x)
{
    std::ostringstream oss;
    oss  << "(" << x.rows() << ", " << x.cols() << ")";
    return oss.str();
}

void predict() {
    X = A * X;
    P = A * P * A.transpose() + Q;
}
void update(const Eigen::MatrixXd& z) {
    Eigen::MatrixXd y = z - H * X;
    Eigen::MatrixXd S = H * P * H.transpose() + R;
    Eigen::MatrixXd K = P * H.transpose() * S.inverse();

    X = X + K * y;
    P = (Eigen::MatrixXd::Identity(4, 4) - K * H) * P;
}


void mouse_kalman(int event, int x, int y, int flag, void *userdata)
{
//    if(event == EVENT_MOUSEMOVE && (flag & cv::EVENT_FLAG_LBUTTON)){
    if(event == EVENT_MOUSEMOVE && !(flag & cv::EVENT_FLAG_LBUTTON)){

        circle(img, Point(x,y), 3, Scalar(0,255,0));

        random_device rd;
        mt19937 gen(rd());
        normal_distribution<double> dist(0.0, 10);
        MatrixXd sX(2,1);
        sX << x+ dist(gen), y+ dist(gen);
        circle(img, Point(sX(0,0),sX(1,0)), 6, Scalar(255,0,0), FILLED);
        predict();

        update(sX);
        Point predict_position = cv::Point(X(0,0), X(1,0));
        circle(img, predict_position, 6, Scalar(0,0,255));
    }
    else if (flag & cv::EVENT_FLAG_LBUTTON){
        cout << "sensor Loss" << endl;

        predict();
        circle(img, Point(X(0,0),X(1,0)), 3, Scalar(0,0,255));


    }






    //    kalman_F(X, );

}



int main(void)
{
    
    
    H << 1, 0, 0, 0,
         0, 1, 0, 0;
    X << 0,0,0,0;

    P.diagonal() << 1, 1,10,10;
    Q.diagonal() << pow(0.01,2), pow(0.01,2), pow(1,2), pow(1,2);
    R.diagonal() << 10,10;

//    cout << dist(gen) << endl;
    int width = 1000;
    int height = 1000;

    img = Mat(width, height, CV_8UC3, Scalar(0,0,0));
    namedWindow("paint");

    
    double ticks = 0;
    while(1)
    {
        double precTick = ticks;
        ticks = (double) cv::getTickCount();

        double dT = (ticks - precTick) / cv::getTickFrequency();
        
        A << 1, 0, dT, 0,
             0, 1, 0, dT,
             0, 0, 1, 0,
             0, 0, 0, 1;
        imshow("paint",img);
        setMouseCallback("paint", mouse_kalman, 0);


        int key = waitKey(1);
        if (key == 27)
            break;
    }

    return 0;

}





