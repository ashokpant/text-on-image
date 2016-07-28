#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

enum ColorMode {
    DEFAULT, GLOBAL, COMPLEX, BEST
};

string winName;
Mat img;
Mat dst;
string text;
ColorMode colorMode;
int contrastUp;
int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
double fontScale = 2;
int fontThickness = 3;

double *colorDetectionByHue(Mat img) {
    /*%**************************************************
    %This function auto recognize the 6 zones from image 'img' using hue with
    %saturation and value thresholds.
    %
    %Hue (degree) for different color:
    %  red = 0
    %  yellow = 60
    %  green = 120
    %  cyan = 180
    %  blue = 240
    %  magenta = 300
    %
    %**************************************************
    %Output:
    %**************************************************
    % .black .white
    % .red .yellow .green .cyan .blue .magenta
    %
    %***************************************************/

    //Threshold values
    double sThresh[] = {0.1, 1};
    double vThresh[] = {0.1, 1};

    // color recognition
    Mat hsvI;
    cvtColor(img, hsvI, CV_BGR2HSV);
    Mat hueI, satI, valI;

    hsvI = hsvI / 255;

    Mat channel[3];
    split(hsvI, channel);
    hueI = channel[0] * 360;
    satI = channel[1];
    valI = channel[2];

    Mat threshI = (satI >= sThresh[0]) & (satI <= sThresh[1]) & (valI >= vThresh[0]) & (valI <= vThresh[1]);

    Mat black = (valI < vThresh[0]);
    Mat white = (satI < sThresh[0]) & (valI >= vThresh[0]);
    Mat red = ((hueI <= 30) | (hueI > 330)) & threshI;
    Mat yellow = ((hueI > 30) & (hueI <= 90)) & threshI;
    Mat green = ((hueI > 90) & (hueI <= 150)) & threshI;
    Mat cyan = ((hueI > 150) & (hueI <= 210)) & threshI;
    Mat blue = ((hueI > 210) & (hueI <= 270)) & threshI;
    Mat magenta = ((hueI > 270) & (hueI <= 330)) & threshI;

    static double output[8];
    output[0] = mean(black)[0];
    output[1] = mean(white)[0];
    output[2] = mean(red)[0];
    output[3] = mean(yellow)[0];
    output[4] = mean(green)[0];
    output[5] = mean(cyan)[0];
    output[6] = mean(blue)[0];
    output[7] = mean(magenta)[0];

    return output;
}

int getIndexOfLargestElement(double *arr, int size) {
    int largestIndex = 0;
    for (int index = largestIndex; index < size; index++) {
        if (*(arr + largestIndex) < *(arr + index)) {
            largestIndex = index;
        }
    }
    return largestIndex;
}

int *getTextColor(Mat img, Point start, ColorMode colorMode, Size textSize, int contrastUp) {
    static int color[3];
    int RGB_MAX = 255;
    int RGB_MIN = 0;

    Point rectStart = start;
    int padWidth = 10;
    int padHeight = 20;

    Size rectSize = textSize;
    rectSize = Size(rectSize.width + padWidth * 2, rectSize.height + padHeight * 2);
    if (rectSize.height < 1 || rectSize.height > img.rows)rectSize.height = img.rows;
    if (rectSize.width < 1 || rectSize.width > img.rows)rectSize.width = img.cols;

    if (rectStart.x + rectSize.width > img.cols) {
        rectStart.x = img.cols - rectSize.width < 0 ? 0 : img.cols - rectSize.width;
    }

    if (rectStart.y + rectSize.height > img.rows) {
        rectStart.y = img.rows - rectSize.height < 0 ? 0 : img.rows - rectSize.height;
    }

    Rect rect = Rect(rectStart.x, rectStart.y, rectSize.width, rectSize.height);

    if (colorMode == ColorMode::DEFAULT) {
        // Default Color
        color[0] = 0;
        color[1] = 128;
        color[2] = 255;
    } else if (colorMode == ColorMode::BEST) {
        //Best Color
        Mat temp(img, rect);
        Scalar tMean = mean(temp);
        color[0] = (int) (RGB_MAX - tMean[2] + contrastUp);
        color[1] = (int) (RGB_MAX - tMean[1] + contrastUp);
        color[2] = (int) (RGB_MAX - tMean[0] + contrastUp);
    } else if (colorMode == ColorMode::GLOBAL) {
        //Global Color
        Scalar tMean = mean(img);
        color[0] = (int) (RGB_MAX - tMean[2] + contrastUp);
        color[1] = (int) (RGB_MAX - tMean[1] + contrastUp);
        color[2] = (int) (RGB_MAX - tMean[0] + contrastUp);
    } else if (colorMode == ColorMode::COMPLEX) {
        //More Sophisticated Algorithm
        //	% Black= [0 0 0]
        //% White= [255 255 255]
        //% Red =[255 0 0]
        //% Yellow=[255 255 0]
        //% Green =[0 128 0]
        //% Cyan= [0 255 255]
        //% Blue=[0 0 255]
        //% Magenta=[255 0 255]
        double *output = colorDetectionByHue(img);

        int ind = getIndexOfLargestElement(output, 8);
        Scalar tMean = mean(img);
        if (ind == 1) {//Black
            // color=[0 200 0];
            color[0] = (int) (RGB_MAX - tMean[2]);
            color[1] = (int) (tMean[1] + contrastUp);
            color[2] = (int) (tMean[0] + contrastUp);
        } else if (ind == 2) {//White
            //color=[0 128 180];
            color[0] = (int) (tMean[2] + contrastUp);
            color[1] = (int) (RGB_MAX - tMean[1]);
            color[2] = (int) (RGB_MAX - tMean[0]);
        } else if (ind == 3) {//Red
            color[0] = (int) (RGB_MAX - tMean[2] + contrastUp);
            color[1] = (int) (tMean[1] + contrastUp);
            color[2] = (int) (tMean[0] + contrastUp);
        } else if (ind == 4) {//Yellow
            color[0] = (int) (RGB_MAX - tMean[2]);
            color[1] = (int) (RGB_MAX - tMean[1]);
            color[2] = (int) (tMean[0] + contrastUp);
        } else if (ind == 5) {//Green
            color[0] = (int) (tMean[2] + contrastUp);
            color[1] = (int) (RGB_MAX - tMean[1]);
            color[2] = (int) (RGB_MAX - tMean[0]);
        } else if (ind == 6) {//Cyan
            color[0] = (int) (RGB_MAX - tMean[2]);
            color[1] = (int) (tMean[1] + contrastUp);
            color[1] = (int) (tMean[0] + contrastUp);
        } else if (ind == 7) {//Blue
            color[0] = (int) (tMean[2] + contrastUp);
            color[1] = (int) (tMean[1] + contrastUp);
            color[1] = (int) (RGB_MAX - tMean[0]);
        } else if (ind == 8) {//magenta
            color[0] = (int) (RGB_MAX - tMean[2]);
            color[1] = (int) (tMean[1] + contrastUp);
            color[1] = (int) (RGB_MAX - tMean[0]);
        }
    }

    return color;
}

Mat writeTextOnImage(Mat &img, string text, Point start, ColorMode colorMode, int contrastUp) {
    Mat dst = img.clone();
    Size textSize = getTextSize(text, fontFace,
                                fontScale, fontThickness, 0);
    int *color = getTextColor(dst, start, colorMode, textSize, contrastUp);
    putText(dst, text, start, fontFace, fontScale, CV_RGB(*(color + 0), *(color + 1), *(color + 2)), fontThickness);
    return dst;
}

void onMouse(int event, int x, int y, int flags, void *userData) {
    if ((event == EVENT_MOUSEMOVE && flags == 40/*CTRL*/) || (event == EVENT_LBUTTONDOWN)) {
        Point *p = (Point *) userData;
        p->x = x;
        p->y = y;
        dst = writeTextOnImage(img, text, *p, colorMode, contrastUp);
        imshow(winName, dst);
    } else {
        if (!dst.empty())
            imshow(winName, dst);
    }
}

int main(int argc, char *argv[]) {
    string imageName;
    if (argc == 2) {
        imageName = argv[1];
        text = "Love from Nepal!";
    } else if (argc == 3) {
        imageName = argv[1];
        text = argv[2];
    } else {
        cout << "Uses: " << argv[0] << " <image_path> [<text>]" << endl;
        exit(EXIT_SUCCESS);
    }

    colorMode = ColorMode::BEST;
    contrastUp = 20;

    winName = "Text on Image";
    cvNamedWindow(winName.c_str(), CV_WINDOW_AUTOSIZE);

    img = imread(imageName);

    if (img.empty()) {
        cout << "Invalid image: " << imageName << endl;
        exit(EXIT_SUCCESS);
    }
    imshow(winName, img);

    Point point;
    setMouseCallback(winName, onMouse, (void *) &point);

    for (; ;) {
        int c = waitKey(0);
        if ((c & 255) == 27) {
            cout << "Exiting ..." << endl;
            break;
        }
    }

    return 0;
}
